'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

function isBytes(obj) {
  return ArrayBuffer.isView(obj) || obj instanceof ArrayBuffer;
}
function isBlob(obj) {
  return obj.constructor && (obj.constructor.name === 'Blob' || obj.constructor.name === 'File') && typeof obj.stream === 'function';
}
function isFileObject(obj) {
  return typeof obj === 'object' && (obj.path || obj.content);
}
const isReadableStream = value => value && typeof value.getReader === 'function';

exports.isBlob = isBlob;
exports.isBytes = isBytes;
exports.isFileObject = isFileObject;
exports.isReadableStream = isReadableStream;
