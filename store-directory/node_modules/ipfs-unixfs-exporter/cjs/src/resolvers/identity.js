'use strict';

var errCode = require('err-code');
var extractDataFromBlock = require('../utils/extract-data-from-block.js');
var validateOffsetAndLength = require('../utils/validate-offset-and-length.js');
var mh = require('multiformats/hashes/digest');

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
var mh__namespace = /*#__PURE__*/_interopNamespace(mh);

const rawContent = node => {
  async function* contentGenerator(options = {}) {
    const {offset, length} = validateOffsetAndLength(node.length, options.offset, options.length);
    yield extractDataFromBlock(node, 0, offset, offset + length);
  }
  return contentGenerator;
};
const resolve = async (cid, name, path, toResolve, resolve, depth, blockstore, options) => {
  if (toResolve.length) {
    throw errCode__default["default"](new Error(`No link named ${ path } found in raw node ${ cid }`), 'ERR_NOT_FOUND');
  }
  const buf = await mh__namespace.decode(cid.multihash.bytes);
  return {
    entry: {
      type: 'identity',
      name,
      path,
      cid,
      content: rawContent(buf.digest),
      depth,
      size: buf.digest.length,
      node: buf.digest
    }
  };
};

module.exports = resolve;
