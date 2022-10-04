'use strict';

var errCode = require('err-code');
var ipfsUnixfs = require('ipfs-unixfs');
var persist = require('../../utils/persist.js');
var dagPb = require('@ipld/dag-pb');
var parallelBatch = require('it-parallel-batch');
var rawCodec = require('multiformats/codecs/raw');
var flat = require('./flat.js');
var balanced = require('./balanced.js');
var trickle = require('./trickle.js');
var bufferImporter = require('./buffer-importer.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

function _interopNamespace(e) {
  if (e && e.__esModule) return e;
  var n = Object.create(null);
  if (e) {
    Object.keys(e).forEach(function (k) {
      if (k !== 'default') {
        var d = Object.getOwnPropertyDescriptor(e, k);
        Object.defineProperty(n, k, d.get ? d : {
          enumerable: true,
          get: function () { return e[k]; }
        });
      }
    });
  }
  n["default"] = e;
  return Object.freeze(n);
}

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);
var dagPb__namespace = /*#__PURE__*/_interopNamespace(dagPb);
var parallelBatch__default = /*#__PURE__*/_interopDefaultLegacy(parallelBatch);
var rawCodec__namespace = /*#__PURE__*/_interopNamespace(rawCodec);

const dagBuilders = {
  flat: flat,
  balanced: balanced,
  trickle: trickle
};
async function* buildFileBatch(file, blockstore, options) {
  let count = -1;
  let previous;
  let bufferImporter$1;
  if (typeof options.bufferImporter === 'function') {
    bufferImporter$1 = options.bufferImporter;
  } else {
    bufferImporter$1 = bufferImporter;
  }
  for await (const entry of parallelBatch__default["default"](bufferImporter$1(file, blockstore, options), options.blockWriteConcurrency)) {
    count++;
    if (count === 0) {
      previous = entry;
      continue;
    } else if (count === 1 && previous) {
      yield previous;
      previous = null;
    }
    yield entry;
  }
  if (previous) {
    previous.single = true;
    yield previous;
  }
}
const reduce = (file, blockstore, options) => {
  async function reducer(leaves) {
    if (leaves.length === 1 && leaves[0].single && options.reduceSingleLeafToSelf) {
      const leaf = leaves[0];
      if (file.mtime !== undefined || file.mode !== undefined) {
        let buffer = await blockstore.get(leaf.cid);
        leaf.unixfs = new ipfsUnixfs.UnixFS({
          type: 'file',
          mtime: file.mtime,
          mode: file.mode,
          data: buffer
        });
        buffer = dagPb.encode(dagPb.prepare({ Data: leaf.unixfs.marshal() }));
        leaf.cid = await persist(buffer, blockstore, {
          ...options,
          codec: dagPb__namespace,
          hasher: options.hasher,
          cidVersion: options.cidVersion
        });
        leaf.size = buffer.length;
      }
      return {
        cid: leaf.cid,
        path: file.path,
        unixfs: leaf.unixfs,
        size: leaf.size
      };
    }
    const f = new ipfsUnixfs.UnixFS({
      type: 'file',
      mtime: file.mtime,
      mode: file.mode
    });
    const links = leaves.filter(leaf => {
      if (leaf.cid.code === rawCodec__namespace.code && leaf.size) {
        return true;
      }
      if (leaf.unixfs && !leaf.unixfs.data && leaf.unixfs.fileSize()) {
        return true;
      }
      return Boolean(leaf.unixfs && leaf.unixfs.data && leaf.unixfs.data.length);
    }).map(leaf => {
      if (leaf.cid.code === rawCodec__namespace.code) {
        f.addBlockSize(leaf.size);
        return {
          Name: '',
          Tsize: leaf.size,
          Hash: leaf.cid
        };
      }
      if (!leaf.unixfs || !leaf.unixfs.data) {
        f.addBlockSize(leaf.unixfs && leaf.unixfs.fileSize() || 0);
      } else {
        f.addBlockSize(leaf.unixfs.data.length);
      }
      return {
        Name: '',
        Tsize: leaf.size,
        Hash: leaf.cid
      };
    });
    const node = {
      Data: f.marshal(),
      Links: links
    };
    const buffer = dagPb.encode(dagPb.prepare(node));
    const cid = await persist(buffer, blockstore, options);
    return {
      cid,
      path: file.path,
      unixfs: f,
      size: buffer.length + node.Links.reduce((acc, curr) => acc + curr.Tsize, 0)
    };
  }
  return reducer;
};
function fileBuilder(file, block, options) {
  const dagBuilder = dagBuilders[options.strategy];
  if (!dagBuilder) {
    throw errCode__default["default"](new Error(`Unknown importer build strategy name: ${ options.strategy }`), 'ERR_BAD_STRATEGY');
  }
  return dagBuilder(buildFileBatch(file, block, options), reduce(file, block, options), options);
}

module.exports = fileBuilder;
