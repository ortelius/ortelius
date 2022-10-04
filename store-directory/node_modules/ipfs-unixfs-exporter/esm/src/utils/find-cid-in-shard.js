import {
  Bucket,
  createHAMT
} from 'hamt-sharding';
import { decode } from '@ipld/dag-pb';
import { murmur3128 } from '@multiformats/murmur3';
const hashFn = async function (buf) {
  return (await murmur3128.encode(buf)).slice(0, 8).reverse();
};
const addLinksToHamtBucket = (links, bucket, rootBucket) => {
  return Promise.all(links.map(link => {
    if (link.Name == null) {
      throw new Error('Unexpected Link without a Name');
    }
    if (link.Name.length === 2) {
      const pos = parseInt(link.Name, 16);
      return bucket._putObjectAt(pos, new Bucket({
        hash: rootBucket._options.hash,
        bits: rootBucket._options.bits
      }, bucket, pos));
    }
    return rootBucket.put(link.Name.substring(2), true);
  }));
};
const toPrefix = position => {
  return position.toString(16).toUpperCase().padStart(2, '0').substring(0, 2);
};
const toBucketPath = position => {
  let bucket = position.bucket;
  const path = [];
  while (bucket._parent) {
    path.push(bucket);
    bucket = bucket._parent;
  }
  path.push(bucket);
  return path.reverse();
};
const findShardCid = async (node, name, blockstore, context, options) => {
  if (!context) {
    const rootBucket = createHAMT({ hashFn });
    context = {
      rootBucket,
      hamtDepth: 1,
      lastBucket: rootBucket
    };
  }
  await addLinksToHamtBucket(node.Links, context.lastBucket, context.rootBucket);
  const position = await context.rootBucket._findNewBucketAndPos(name);
  let prefix = toPrefix(position.pos);
  const bucketPath = toBucketPath(position);
  if (bucketPath.length > context.hamtDepth) {
    context.lastBucket = bucketPath[context.hamtDepth];
    prefix = toPrefix(context.lastBucket._posAtParent);
  }
  const link = node.Links.find(link => {
    if (link.Name == null) {
      return false;
    }
    const entryPrefix = link.Name.substring(0, 2);
    const entryName = link.Name.substring(2);
    if (entryPrefix !== prefix) {
      return false;
    }
    if (entryName && entryName !== name) {
      return false;
    }
    return true;
  });
  if (!link) {
    return null;
  }
  if (link.Name != null && link.Name.substring(2) === name) {
    return link.Hash;
  }
  context.hamtDepth++;
  const block = await blockstore.get(link.Hash, options);
  node = decode(block);
  return findShardCid(node, name, blockstore, context, options);
};
export default findShardCid;