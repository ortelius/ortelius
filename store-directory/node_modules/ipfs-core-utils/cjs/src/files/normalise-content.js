'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var errCode = require('err-code');
var fromString = require('uint8arrays/from-string');
var browserStreamToIt = require('browser-readablestream-to-it');
var blobToIt = require('blob-to-it');
var itPeekable = require('it-peekable');
var all = require('it-all');
var map = require('it-map');
var utils = require('./utils.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);
var browserStreamToIt__default = /*#__PURE__*/_interopDefaultLegacy(browserStreamToIt);
var blobToIt__default = /*#__PURE__*/_interopDefaultLegacy(blobToIt);
var itPeekable__default = /*#__PURE__*/_interopDefaultLegacy(itPeekable);
var all__default = /*#__PURE__*/_interopDefaultLegacy(all);
var map__default = /*#__PURE__*/_interopDefaultLegacy(map);

async function* toAsyncIterable(thing) {
  yield thing;
}
async function normaliseContent(input) {
  if (utils.isBytes(input)) {
    return toAsyncIterable(toBytes(input));
  }
  if (typeof input === 'string' || input instanceof String) {
    return toAsyncIterable(toBytes(input.toString()));
  }
  if (utils.isBlob(input)) {
    return blobToIt__default["default"](input);
  }
  if (utils.isReadableStream(input)) {
    input = browserStreamToIt__default["default"](input);
  }
  if (Symbol.iterator in input || Symbol.asyncIterator in input) {
    const peekable = itPeekable__default["default"](input);
    const {value, done} = await peekable.peek();
    if (done) {
      return toAsyncIterable(new Uint8Array(0));
    }
    peekable.push(value);
    if (Number.isInteger(value)) {
      return toAsyncIterable(Uint8Array.from(await all__default["default"](peekable)));
    }
    if (utils.isBytes(value) || typeof value === 'string' || value instanceof String) {
      return map__default["default"](peekable, toBytes);
    }
  }
  throw errCode__default["default"](new Error(`Unexpected input: ${ input }`), 'ERR_UNEXPECTED_INPUT');
}
function toBytes(chunk) {
  if (chunk instanceof Uint8Array) {
    return chunk;
  }
  if (ArrayBuffer.isView(chunk)) {
    return new Uint8Array(chunk.buffer, chunk.byteOffset, chunk.byteLength);
  }
  if (chunk instanceof ArrayBuffer) {
    return new Uint8Array(chunk);
  }
  if (Array.isArray(chunk)) {
    return Uint8Array.from(chunk);
  }
  return fromString.fromString(chunk.toString());
}

exports.normaliseContent = normaliseContent;
