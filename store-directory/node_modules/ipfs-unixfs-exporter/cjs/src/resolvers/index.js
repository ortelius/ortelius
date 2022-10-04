'use strict';

var errCode = require('err-code');
var dagPb = require('@ipld/dag-pb');
var dagCbor = require('@ipld/dag-cbor');
var raw = require('multiformats/codecs/raw');
var identity = require('multiformats/hashes/identity');
var index = require('./unixfs-v1/index.js');
var raw$1 = require('./raw.js');
var dagCbor$1 = require('./dag-cbor.js');
var identity$1 = require('./identity.js');

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
var dagCbor__namespace = /*#__PURE__*/_interopNamespace(dagCbor);
var raw__namespace = /*#__PURE__*/_interopNamespace(raw);

const resolvers = {
  [dagPb__namespace.code]: index,
  [raw__namespace.code]: raw$1,
  [dagCbor__namespace.code]: dagCbor$1,
  [identity.identity.code]: identity$1
};
function resolve(cid, name, path, toResolve, depth, blockstore, options) {
  const resolver = resolvers[cid.code];
  if (!resolver) {
    throw errCode__default["default"](new Error(`No resolver for code ${ cid.code }`), 'ERR_NO_RESOLVER');
  }
  return resolver(cid, name, path, toResolve, resolve, depth, blockstore, options);
}

module.exports = resolve;
