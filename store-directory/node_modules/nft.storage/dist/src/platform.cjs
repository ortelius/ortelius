'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var fetch = require('@web-std/fetch');
var formData = require('@web-std/form-data');
var blob = require('@web-std/blob');
var file = require('@web-std/file');
var fs = require('ipfs-car/blockstore/fs');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var fetch__default = /*#__PURE__*/_interopDefaultLegacy(fetch);



Object.defineProperty(exports, 'Headers', {
  enumerable: true,
  get: function () {
    return fetch.Headers;
  }
});
Object.defineProperty(exports, 'Request', {
  enumerable: true,
  get: function () {
    return fetch.Request;
  }
});
Object.defineProperty(exports, 'Response', {
  enumerable: true,
  get: function () {
    return fetch.Response;
  }
});
Object.defineProperty(exports, 'fetch', {
  enumerable: true,
  get: function () {
    return fetch__default['default'];
  }
});
Object.defineProperty(exports, 'FormData', {
  enumerable: true,
  get: function () {
    return formData.FormData;
  }
});
Object.defineProperty(exports, 'ReadableStream', {
  enumerable: true,
  get: function () {
    return blob.ReadableStream;
  }
});
Object.defineProperty(exports, 'Blob', {
  enumerable: true,
  get: function () {
    return file.Blob;
  }
});
Object.defineProperty(exports, 'File', {
  enumerable: true,
  get: function () {
    return file.File;
  }
});
Object.defineProperty(exports, 'Blockstore', {
  enumerable: true,
  get: function () {
    return fs.FsBlockStore;
  }
});
//# sourceMappingURL=platform.cjs.map
