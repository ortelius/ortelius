'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var hasher = require('multiformats/hashes/hasher');
var multiformats = require('multiformats');
var mur = require('murmurhash3js-revisited');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var mur__default = /*#__PURE__*/_interopDefaultLegacy(mur);

function fromNumberTo32BitBuf(number) {
  const bytes = new Array(4);
  for (let i = 0; i < 4; i++) {
    bytes[i] = number & 255;
    number = number >> 8;
  }
  return new Uint8Array(bytes);
}
const murmur332 = hasher.from({
  name: 'murmur3-32',
  code: 35,
  encode: input => fromNumberTo32BitBuf(mur__default["default"].x86.hash32(input))
});
const murmur3128 = hasher.from({
  name: 'murmur3-128',
  code: 34,
  encode: input => multiformats.bytes.fromHex(mur__default["default"].x64.hash128(input))
});

exports.murmur3128 = murmur3128;
exports.murmur332 = murmur332;
