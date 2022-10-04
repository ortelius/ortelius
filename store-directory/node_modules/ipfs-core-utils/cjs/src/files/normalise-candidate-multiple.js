'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var errCode = require('err-code');
var browserStreamToIt = require('browser-readablestream-to-it');
var itPeekable = require('it-peekable');
var map = require('it-map');
var utils = require('./utils.js');
var ipfsUnixfs = require('ipfs-unixfs');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);
var browserStreamToIt__default = /*#__PURE__*/_interopDefaultLegacy(browserStreamToIt);
var itPeekable__default = /*#__PURE__*/_interopDefaultLegacy(itPeekable);
var map__default = /*#__PURE__*/_interopDefaultLegacy(map);

async function* normaliseCandidateMultiple(input, normaliseContent) {
  if (typeof input === 'string' || input instanceof String || utils.isBytes(input) || utils.isBlob(input) || input._readableState) {
    throw errCode__default["default"](new Error('Unexpected input: single item passed - if you are using ipfs.addAll, please use ipfs.add instead'), 'ERR_UNEXPECTED_INPUT');
  }
  if (utils.isReadableStream(input)) {
    input = browserStreamToIt__default["default"](input);
  }
  if (Symbol.iterator in input || Symbol.asyncIterator in input) {
    const peekable = itPeekable__default["default"](input);
    const {value, done} = await peekable.peek();
    if (done) {
      yield* [];
      return;
    }
    peekable.push(value);
    if (Number.isInteger(value)) {
      throw errCode__default["default"](new Error('Unexpected input: single item passed - if you are using ipfs.addAll, please use ipfs.add instead'), 'ERR_UNEXPECTED_INPUT');
    }
    if (value._readableState) {
      yield* map__default["default"](peekable, value => toFileObject({ content: value }, normaliseContent));
      return;
    }
    if (utils.isBytes(value)) {
      yield toFileObject({ content: peekable }, normaliseContent);
      return;
    }
    if (utils.isFileObject(value) || value[Symbol.iterator] || value[Symbol.asyncIterator] || utils.isReadableStream(value) || utils.isBlob(value)) {
      yield* map__default["default"](peekable, value => toFileObject(value, normaliseContent));
      return;
    }
  }
  if (utils.isFileObject(input)) {
    throw errCode__default["default"](new Error('Unexpected input: single item passed - if you are using ipfs.addAll, please use ipfs.add instead'), 'ERR_UNEXPECTED_INPUT');
  }
  throw errCode__default["default"](new Error('Unexpected input: ' + typeof input), 'ERR_UNEXPECTED_INPUT');
}
async function toFileObject(input, normaliseContent) {
  const {path, mode, mtime, content} = input;
  const file = {
    path: path || '',
    mode: ipfsUnixfs.parseMode(mode),
    mtime: ipfsUnixfs.parseMtime(mtime)
  };
  if (content) {
    file.content = await normaliseContent(content);
  } else if (!path) {
    file.content = await normaliseContent(input);
  }
  return file;
}

exports.normaliseCandidateMultiple = normaliseCandidateMultiple;
