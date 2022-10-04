'use strict';

var BufferList = require('bl/BufferList.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var BufferList__default = /*#__PURE__*/_interopDefaultLegacy(BufferList);

async function* fixedSizeChunker(source, options) {
  let bl = new BufferList__default["default"]();
  let currentLength = 0;
  let emitted = false;
  const maxChunkSize = options.maxChunkSize;
  for await (const buffer of source) {
    bl.append(buffer);
    currentLength += buffer.length;
    while (currentLength >= maxChunkSize) {
      yield bl.slice(0, maxChunkSize);
      emitted = true;
      if (maxChunkSize === bl.length) {
        bl = new BufferList__default["default"]();
        currentLength = 0;
      } else {
        const newBl = new BufferList__default["default"]();
        newBl.append(bl.shallowSlice(maxChunkSize));
        bl = newBl;
        currentLength -= maxChunkSize;
      }
    }
  }
  if (!emitted || currentLength) {
    yield bl.slice(0, currentLength);
  }
}

module.exports = fixedSizeChunker;
