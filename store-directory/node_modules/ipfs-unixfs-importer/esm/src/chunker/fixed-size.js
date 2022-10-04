import BufferList from 'bl/BufferList.js';
async function* fixedSizeChunker(source, options) {
  let bl = new BufferList();
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
        bl = new BufferList();
        currentLength = 0;
      } else {
        const newBl = new BufferList();
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
export default fixedSizeChunker;