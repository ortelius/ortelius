'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var alloc = require('./alloc.js');

function xor(a, b) {
  if (a.length !== b.length) {
    throw new Error('Inputs should have the same length');
  }
  const result = alloc.allocUnsafe(a.length);
  for (let i = 0; i < a.length; i++) {
    result[i] = a[i] ^ b[i];
  }
  return result;
}

exports.xor = xor;
