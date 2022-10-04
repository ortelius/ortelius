'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var memory = require('ipfs-car/blockstore/memory');

const fetch = globalThis.fetch;
const FormData = globalThis.FormData;
const Headers = globalThis.Headers;
const Request = globalThis.Request;
const Response = globalThis.Response;
const Blob = globalThis.Blob;
const File = globalThis.File;
const ReadableStream = globalThis.ReadableStream;
const Blockstore = memory.MemoryBlockStore;

exports.Blob = Blob;
exports.Blockstore = Blockstore;
exports.File = File;
exports.FormData = FormData;
exports.Headers = Headers;
exports.ReadableStream = ReadableStream;
exports.Request = Request;
exports.Response = Response;
exports.fetch = fetch;
//# sourceMappingURL=platform.web.cjs.map
