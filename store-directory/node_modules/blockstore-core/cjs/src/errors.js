'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var errCode = require('err-code');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);

function notFoundError(err) {
  err = err || new Error('Not Found');
  return errCode__default["default"](err, 'ERR_NOT_FOUND');
}
function abortedError(err) {
  err = err || new Error('Aborted');
  return errCode__default["default"](err, 'ERR_ABORTED');
}

exports.abortedError = abortedError;
exports.notFoundError = notFoundError;
