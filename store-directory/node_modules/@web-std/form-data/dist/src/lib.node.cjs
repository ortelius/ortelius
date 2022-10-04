'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var formData = require('./form-data.cjs');

// @ts-check

// Electron-renderer should get the browser implementation instead of node
// which is why we check global first.
const FormData =
  typeof globalThis.FormData === "function"
    ? globalThis.FormData
    : formData.FormData;

exports.FormData = FormData;
//# sourceMappingURL=lib.node.cjs.map
