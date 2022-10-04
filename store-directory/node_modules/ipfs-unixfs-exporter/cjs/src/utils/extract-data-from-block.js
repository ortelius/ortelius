'use strict';

function extractDataFromBlock(block, blockStart, requestedStart, requestedEnd) {
  const blockLength = block.length;
  const blockEnd = blockStart + blockLength;
  if (requestedStart >= blockEnd || requestedEnd < blockStart) {
    return new Uint8Array(0);
  }
  if (requestedEnd >= blockStart && requestedEnd < blockEnd) {
    block = block.slice(0, requestedEnd - blockStart);
  }
  if (requestedStart >= blockStart && requestedStart < blockEnd) {
    block = block.slice(requestedStart - blockStart);
  }
  return block;
}

module.exports = extractDataFromBlock;
