import errCode from 'err-code';
import itPeekable from 'it-peekable';
import browserStreamToIt from 'browser-readablestream-to-it';
import all from 'it-all';
import {
  isBytes,
  isBlob,
  isReadableStream
} from './utils.js';
export async function normaliseContent(input) {
  if (isBytes(input)) {
    return new Blob([input]);
  }
  if (typeof input === 'string' || input instanceof String) {
    return new Blob([input.toString()]);
  }
  if (isBlob(input)) {
    return input;
  }
  if (isReadableStream(input)) {
    input = browserStreamToIt(input);
  }
  if (Symbol.iterator in input || Symbol.asyncIterator in input) {
    const peekable = itPeekable(input);
    const {value, done} = await peekable.peek();
    if (done) {
      return itToBlob(peekable);
    }
    peekable.push(value);
    if (Number.isInteger(value)) {
      return new Blob([Uint8Array.from(await all(peekable))]);
    }
    if (isBytes(value) || typeof value === 'string' || value instanceof String) {
      return itToBlob(peekable);
    }
  }
  throw errCode(new Error(`Unexpected input: ${ input }`), 'ERR_UNEXPECTED_INPUT');
}
async function itToBlob(stream) {
  const parts = [];
  for await (const chunk of stream) {
    parts.push(chunk);
  }
  return new Blob(parts);
}