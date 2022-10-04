'use strict';

var cid = require('multiformats/cid');
var dagPb = require('@ipld/dag-pb');
var sha2 = require('multiformats/hashes/sha2');

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

const persist = async (buffer, blockstore, options) => {
  if (!options.codec) {
    options.codec = dagPb__namespace;
  }
  if (!options.hasher) {
    options.hasher = sha2.sha256;
  }
  if (options.cidVersion === undefined) {
    options.cidVersion = 1;
  }
  if (options.codec === dagPb__namespace && options.hasher !== sha2.sha256) {
    options.cidVersion = 1;
  }
  const multihash = await options.hasher.digest(buffer);
  const cid$1 = cid.CID.create(options.cidVersion, options.codec.code, multihash);
  if (!options.onlyHash) {
    await blockstore.put(cid$1, buffer, { signal: options.signal });
  }
  return cid$1;
};

module.exports = persist;
