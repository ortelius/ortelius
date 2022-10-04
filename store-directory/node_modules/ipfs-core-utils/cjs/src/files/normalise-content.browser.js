'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var errCode = require('err-code');
var itPeekable = require('it-peekable');
var browserStreamToIt = require('browser-readablestream-to-it');
var all = require('it-all');
var utils = require('./utils.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);
var itPeekable__default = /*#__PURE__*/_interopDefaultLegacy(itPeekable);
var browserStreamToIt__default = /*#__PURE__*/_interopDefaultLegacy(browserStreamToIt);
var all__default = /*#__PURE__*/_interopDefaultLegacy(all);

async function normaliseContent(input) {
  if (utils.isBytes(input)) {
    return new Blob([input]);
  }
  if (typeof input === 'string' || input instanceof String) {
    return new Blob([input.toString()]);
  }
  if (utils.isBlob(input)) {
    return input;
  }
  if (utils.isReadableStream(input)) {
    input = browserStreamToIt__default["default"](input);
  }
  if (Symbol.iterator in input || Symbol.asyncIterator in input) {
    const peekable = itPeekable__default["default"](input);
    const {value, done} = await peekable.peek();
    if (done) {
      return itToBlob(peekable);
    }
    peekable.push(value);
    if (Number.isInteger(value)) {
      return new Blob([Uint8Array.from(await all__default["default"](peekable))]);
    }
    if (utils.isBytes(value) || typeof value === 'string' || value instanceof String) {
      return itToBlob(peekable);
    }
  }
  throw errCode__default["default"](new Error(`Unexpected input: ${ input }`), 'ERR_UNEXPECTED_INPUT');
}
async function itToBlob(stream) {
  const parts = [];
  for await (const chunk of stream) {
    parts.push(chunk);
  }
  return new Blob(parts);
}

exports.normaliseContent = normaliseContent;
