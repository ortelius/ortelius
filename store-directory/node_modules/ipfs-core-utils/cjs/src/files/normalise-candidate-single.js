'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var errCode = require('err-code');
var browserStreamToIt = require('browser-readablestream-to-it');
var itPeekable = require('it-peekable');
var utils = require('./utils.js');
var ipfsUnixfs = require('ipfs-unixfs');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);
var browserStreamToIt__default = /*#__PURE__*/_interopDefaultLegacy(browserStreamToIt);
var itPeekable__default = /*#__PURE__*/_interopDefaultLegacy(itPeekable);

async function* normaliseCandidateSingle(input, normaliseContent) {
  if (input === null || input === undefined) {
    throw errCode__default["default"](new Error(`Unexpected input: ${ input }`), 'ERR_UNEXPECTED_INPUT');
  }
  if (typeof input === 'string' || input instanceof String) {
    yield toFileObject(input.toString(), normaliseContent);
    return;
  }
  if (utils.isBytes(input) || utils.isBlob(input)) {
    yield toFileObject(input, normaliseContent);
    return;
  }
  if (utils.isReadableStream(input)) {
    input = browserStreamToIt__default["default"](input);
  }
  if (Symbol.iterator in input || Symbol.asyncIterator in input) {
    const peekable = itPeekable__default["default"](input);
    const {value, done} = await peekable.peek();
    if (done) {
      yield { content: [] };
      return;
    }
    peekable.push(value);
    if (Number.isInteger(value) || utils.isBytes(value) || typeof value === 'string' || value instanceof String) {
      yield toFileObject(peekable, normaliseContent);
      return;
    }
    throw errCode__default["default"](new Error('Unexpected input: multiple items passed - if you are using ipfs.add, please use ipfs.addAll instead'), 'ERR_UNEXPECTED_INPUT');
  }
  if (utils.isFileObject(input)) {
    yield toFileObject(input, normaliseContent);
    return;
  }
  throw errCode__default["default"](new Error('Unexpected input: cannot convert "' + typeof input + '" into ImportCandidate'), 'ERR_UNEXPECTED_INPUT');
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

exports.normaliseCandidateSingle = normaliseCandidateSingle;
