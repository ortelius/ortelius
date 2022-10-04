'use strict';

var cid = require('multiformats/cid');
var errCode = require('err-code');
var dagCbor = require('@ipld/dag-cbor');

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
var dagCbor__namespace = /*#__PURE__*/_interopNamespace(dagCbor);

const resolve = async (cid$1, name, path, toResolve, resolve, depth, blockstore, options) => {
  const block = await blockstore.get(cid$1);
  const object = dagCbor__namespace.decode(block);
  let subObject = object;
  let subPath = path;
  while (toResolve.length) {
    const prop = toResolve[0];
    if (prop in subObject) {
      toResolve.shift();
      subPath = `${ subPath }/${ prop }`;
      const subObjectCid = cid.CID.asCID(subObject[prop]);
      if (subObjectCid) {
        return {
          entry: {
            type: 'object',
            name,
            path,
            cid: cid$1,
            node: block,
            depth,
            size: block.length,
            content: async function* () {
              yield object;
            }
          },
          next: {
            cid: subObjectCid,
            name: prop,
            path: subPath,
            toResolve
          }
        };
      }
      subObject = subObject[prop];
    } else {
      throw errCode__default["default"](new Error(`No property named ${ prop } found in cbor node ${ cid$1 }`), 'ERR_NO_PROP');
    }
  }
  return {
    entry: {
      type: 'object',
      name,
      path,
      cid: cid$1,
      node: block,
      depth,
      size: block.length,
      content: async function* () {
        yield object;
      }
    }
  };
};

module.exports = resolve;
