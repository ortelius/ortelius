'use strict';

var dagPb = require('@ipld/dag-pb');
var ipfsUnixfs = require('ipfs-unixfs');
var dir = require('./dir.js');
var persist = require('./utils/persist.js');
var hamtSharding = require('hamt-sharding');

class DirSharded extends dir {
  constructor(props, options) {
    super(props, options);
    this._bucket = hamtSharding.createHAMT({
      hashFn: options.hamtHashFn,
      bits: options.hamtBucketBits
    });
  }
  async put(name, value) {
    await this._bucket.put(name, value);
  }
  get(name) {
    return this._bucket.get(name);
  }
  childCount() {
    return this._bucket.leafCount();
  }
  directChildrenCount() {
    return this._bucket.childrenCount();
  }
  onlyChild() {
    return this._bucket.onlyChild();
  }
  async *eachChildSeries() {
    for await (const {key, value} of this._bucket.eachLeafSeries()) {
      yield {
        key,
        child: value
      };
    }
  }
  async *flush(blockstore) {
    for await (const entry of flush(this._bucket, blockstore, this, this.options)) {
      yield {
        ...entry,
        path: this.path
      };
    }
  }
}
async function* flush(bucket, blockstore, shardRoot, options) {
  const children = bucket._children;
  const links = [];
  let childrenSize = 0;
  for (let i = 0; i < children.length; i++) {
    const child = children.get(i);
    if (!child) {
      continue;
    }
    const labelPrefix = i.toString(16).toUpperCase().padStart(2, '0');
    if (child instanceof hamtSharding.Bucket) {
      let shard;
      for await (const subShard of await flush(child, blockstore, null, options)) {
        shard = subShard;
      }
      if (!shard) {
        throw new Error('Could not flush sharded directory, no subshard found');
      }
      links.push({
        Name: labelPrefix,
        Tsize: shard.size,
        Hash: shard.cid
      });
      childrenSize += shard.size;
    } else if (typeof child.value.flush === 'function') {
      const dir = child.value;
      let flushedDir;
      for await (const entry of dir.flush(blockstore)) {
        flushedDir = entry;
        yield flushedDir;
      }
      const label = labelPrefix + child.key;
      links.push({
        Name: label,
        Tsize: flushedDir.size,
        Hash: flushedDir.cid
      });
      childrenSize += flushedDir.size;
    } else {
      const value = child.value;
      if (!value.cid) {
        continue;
      }
      const label = labelPrefix + child.key;
      const size = value.size;
      links.push({
        Name: label,
        Tsize: size,
        Hash: value.cid
      });
      childrenSize += size;
    }
  }
  const data = Uint8Array.from(children.bitField().reverse());
  const dir = new ipfsUnixfs.UnixFS({
    type: 'hamt-sharded-directory',
    data,
    fanout: bucket.tableSize(),
    hashType: options.hamtHashCode,
    mtime: shardRoot && shardRoot.mtime,
    mode: shardRoot && shardRoot.mode
  });
  const node = {
    Data: dir.marshal(),
    Links: links
  };
  const buffer = dagPb.encode(dagPb.prepare(node));
  const cid = await persist(buffer, blockstore, options);
  const size = buffer.length + childrenSize;
  yield {
    cid,
    unixfs: dir,
    size
  };
}

module.exports = DirSharded;
