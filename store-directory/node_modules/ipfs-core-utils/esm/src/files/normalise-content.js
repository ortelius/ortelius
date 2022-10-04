import errCode from 'err-code';
import { fromString as uint8ArrayFromString } from 'uint8arrays/from-string';
import browserStreamToIt from 'browser-readablestream-to-it';
import blobToIt from 'blob-to-it';
import itPeekable from 'it-peekable';
import all from 'it-all';
import map from 'it-map';
import {
  isBytes,
  isReadableStream,
  isBlob
} from './utils.js';
async function* toAsyncIterable(thing) {
  yield thing;
}
export async function normaliseContent(input) {
  if (isBytes(input)) {
    return toAsyncIterable(toBytes(input));
  }
  if (typeof input === 'string' || input instanceof String) {
    return toAsyncIterable(toBytes(input.toString()));
  }
  if (isBlob(input)) {
    return blobToIt(input);
  }
  if (isReadableStream(input)) {
    input = browserStreamToIt(input);
  }
  if (Symbol.iterator in input || Symbol.asyncIterator in input) {
    const peekable = itPeekable(input);
    const {value, done} = await peekable.peek();
    if (done) {
      return toAsyncIterable(new Uint8Array(0));
    }
    peekable.push(value);
    if (Number.isInteger(value)) {
      return toAsyncIterable(Uint8Array.from(await all(peekable)));
    }
    if (isBytes(value) || typeof value === 'string' || value instanceof String) {
      return map(peekable, toBytes);
    }
  }
  throw errCode(new Error(`Unexpected input: ${ input }`), 'ERR_UNEXPECTED_INPUT');
}
function toBytes(chunk) {
  if (chunk instanceof Uint8Array) {
    return chunk;
  }
  if (ArrayBuffer.isView(chunk)) {
    return new Uint8Array(chunk.buffer, chunk.byteOffset, chunk.byteLength);
  }
  if (chunk instanceof ArrayBuffer) {
    return new Uint8Array(chunk);
  }
  if (Array.isArray(chunk)) {
    return Uint8Array.from(chunk);
  }
  return uint8ArrayFromString(chunk.toString());
}