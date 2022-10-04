'use strict';

var BufferList = require('bl/BufferList.js');
var rabinWasm = require('rabin-wasm');
var errCode = require('err-code');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var BufferList__default = /*#__PURE__*/_interopDefaultLegacy(BufferList);
var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);

async function* rabinChunker(source, options) {
  let min, max, avg;
  if (options.minChunkSize && options.maxChunkSize && options.avgChunkSize) {
    avg = options.avgChunkSize;
    min = options.minChunkSize;
    max = options.maxChunkSize;
  } else if (!options.avgChunkSize) {
    throw errCode__default["default"](new Error('please specify an average chunk size'), 'ERR_INVALID_AVG_CHUNK_SIZE');
  } else {
    avg = options.avgChunkSize;
    min = avg / 3;
    max = avg + avg / 2;
  }
  if (min < 16) {
    throw errCode__default["default"](new Error('rabin min must be greater than 16'), 'ERR_INVALID_MIN_CHUNK_SIZE');
  }
  if (max < min) {
    max = min;
  }
  if (avg < min) {
    avg = min;
  }
  const sizepow = Math.floor(Math.log2(avg));
  for await (const chunk of rabin(source, {
      min: min,
      max: max,
      bits: sizepow,
      window: options.window,
      polynomial: options.polynomial
    })) {
    yield chunk;
  }
}
async function* rabin(source, options) {
  const r = await rabinWasm.create(options.bits, options.min, options.max, options.window);
  const buffers = new BufferList__default["default"]();
  for await (const chunk of source) {
    buffers.append(chunk);
    const sizes = r.fingerprint(chunk);
    for (let i = 0; i < sizes.length; i++) {
      const size = sizes[i];
      const buf = buffers.slice(0, size);
      buffers.consume(size);
      yield buf;
    }
  }
  if (buffers.length) {
    yield buffers.slice(0);
  }
}

module.exports = rabinChunker;
