'use strict';

var dir = require('./dir.js');
var index = require('./file/index.js');
var errCode = require('err-code');
var rabin = require('../chunker/rabin.js');
var fixedSize = require('../chunker/fixed-size.js');
var validateChunks = require('./validate-chunks.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);

function isIterable(thing) {
  return Symbol.iterator in thing;
}
function isAsyncIterable(thing) {
  return Symbol.asyncIterator in thing;
}
function contentAsAsyncIterable(content) {
  try {
    if (content instanceof Uint8Array) {
      return async function* () {
        yield content;
      }();
    } else if (isIterable(content)) {
      return async function* () {
        yield* content;
      }();
    } else if (isAsyncIterable(content)) {
      return content;
    }
  } catch {
    throw errCode__default["default"](new Error('Content was invalid'), 'ERR_INVALID_CONTENT');
  }
  throw errCode__default["default"](new Error('Content was invalid'), 'ERR_INVALID_CONTENT');
}
async function* dagBuilder(source, blockstore, options) {
  for await (const entry of source) {
    if (entry.path) {
      if (entry.path.substring(0, 2) === './') {
        options.wrapWithDirectory = true;
      }
      entry.path = entry.path.split('/').filter(path => path && path !== '.').join('/');
    }
    if (entry.content) {
      let chunker;
      if (typeof options.chunker === 'function') {
        chunker = options.chunker;
      } else if (options.chunker === 'rabin') {
        chunker = rabin;
      } else {
        chunker = fixedSize;
      }
      let chunkValidator;
      if (typeof options.chunkValidator === 'function') {
        chunkValidator = options.chunkValidator;
      } else {
        chunkValidator = validateChunks;
      }
      const file = {
        path: entry.path,
        mtime: entry.mtime,
        mode: entry.mode,
        content: chunker(chunkValidator(contentAsAsyncIterable(entry.content), options), options)
      };
      yield () => index(file, blockstore, options);
    } else if (entry.path) {
      const dir$1 = {
        path: entry.path,
        mtime: entry.mtime,
        mode: entry.mode
      };
      yield () => dir(dir$1, blockstore, options);
    } else {
      throw new Error('Import candidate must have content or path or both');
    }
  }
}

module.exports = dagBuilder;
