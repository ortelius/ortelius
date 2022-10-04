'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var cid = require('multiformats/cid');
var errCode = require('err-code');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);

const IPFS_PREFIX = '/ipfs/';
function toCidAndPath(string) {
  if (string instanceof Uint8Array) {
    try {
      string = cid.CID.decode(string);
    } catch (err) {
      throw errCode__default["default"](err, 'ERR_INVALID_CID');
    }
  }
  let cid$1 = cid.CID.asCID(string);
  if (cid$1) {
    return {
      cid: cid$1,
      path: undefined
    };
  }
  string = string.toString();
  if (string.startsWith(IPFS_PREFIX)) {
    string = string.substring(IPFS_PREFIX.length);
  }
  const parts = string.split('/');
  let path;
  try {
    cid$1 = cid.CID.parse(parts.shift() || '');
  } catch (err) {
    throw errCode__default["default"](err, 'ERR_INVALID_CID');
  }
  if (parts.length) {
    path = `/${ parts.join('/') }`;
  }
  return {
    cid: cid$1,
    path
  };
}

exports.toCidAndPath = toCidAndPath;
