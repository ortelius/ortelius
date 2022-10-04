'use strict';

var errCode = require('err-code');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);

const validateOffsetAndLength = (size, offset, length) => {
  if (!offset) {
    offset = 0;
  }
  if (offset < 0) {
    throw errCode__default["default"](new Error('Offset must be greater than or equal to 0'), 'ERR_INVALID_PARAMS');
  }
  if (offset > size) {
    throw errCode__default["default"](new Error('Offset must be less than the file size'), 'ERR_INVALID_PARAMS');
  }
  if (!length && length !== 0) {
    length = size - offset;
  }
  if (length < 0) {
    throw errCode__default["default"](new Error('Length must be greater than or equal to 0'), 'ERR_INVALID_PARAMS');
  }
  if (offset + length > size) {
    length = size - offset;
  }
  return {
    offset,
    length
  };
};

module.exports = validateOffsetAndLength;
