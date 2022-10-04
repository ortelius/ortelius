'use strict';

var ipfsUnixfs = require('ipfs-unixfs');
var persist = require('../../utils/persist.js');
var dagPb = require('@ipld/dag-pb');
var rawCodec = require('multiformats/codecs/raw');

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

var dagPb__namespace = /*#__PURE__*/_interopNamespace(dagPb);
var rawCodec__namespace = /*#__PURE__*/_interopNamespace(rawCodec);

async function* bufferImporter(file, block, options) {
  for await (let buffer of file.content) {
    yield async () => {
      options.progress(buffer.length, file.path);
      let unixfs;
      const opts = {
        codec: dagPb__namespace,
        cidVersion: options.cidVersion,
        hasher: options.hasher,
        onlyHash: options.onlyHash
      };
      if (options.rawLeaves) {
        opts.codec = rawCodec__namespace;
        opts.cidVersion = 1;
      } else {
        unixfs = new ipfsUnixfs.UnixFS({
          type: options.leafType,
          data: buffer
        });
        buffer = dagPb__namespace.encode({
          Data: unixfs.marshal(),
          Links: []
        });
      }
      return {
        cid: await persist(buffer, block, opts),
        unixfs,
        size: buffer.length
      };
    };
  }
}

module.exports = bufferImporter;
