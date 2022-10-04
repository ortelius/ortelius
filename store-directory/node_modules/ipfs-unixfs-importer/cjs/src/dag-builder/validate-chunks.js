'use strict';

var errCode = require('err-code');
var fromString = require('uint8arrays/from-string');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);

async function* validateChunks(source) {
  for await (const content of source) {
    if (content.length === undefined) {
      throw errCode__default["default"](new Error('Content was invalid'), 'ERR_INVALID_CONTENT');
    }
    if (typeof content === 'string' || content instanceof String) {
      yield fromString.fromString(content.toString());
    } else if (Array.isArray(content)) {
      yield Uint8Array.from(content);
    } else if (content instanceof Uint8Array) {
      yield content;
    } else {
      throw errCode__default["default"](new Error('Content was invalid'), 'ERR_INVALID_CONTENT');
    }
  }
}

module.exports = validateChunks;
