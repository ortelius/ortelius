import errCode from 'err-code';
import browserStreamToIt from 'browser-readablestream-to-it';
import itPeekable from 'it-peekable';
import map from 'it-map';
import {
  isBytes,
  isBlob,
  isReadableStream,
  isFileObject
} from './utils.js';
import {
  parseMtime,
  parseMode
} from 'ipfs-unixfs';
export async function* normaliseCandidateMultiple(input, normaliseContent) {
  if (typeof input === 'string' || input instanceof String || isBytes(input) || isBlob(input) || input._readableState) {
    throw errCode(new Error('Unexpected input: single item passed - if you are using ipfs.addAll, please use ipfs.add instead'), 'ERR_UNEXPECTED_INPUT');
  }
  if (isReadableStream(input)) {
    input = browserStreamToIt(input);
  }
  if (Symbol.iterator in input || Symbol.asyncIterator in input) {
    const peekable = itPeekable(input);
    const {value, done} = await peekable.peek();
    if (done) {
      yield* [];
      return;
    }
    peekable.push(value);
    if (Number.isInteger(value)) {
      throw errCode(new Error('Unexpected input: single item passed - if you are using ipfs.addAll, please use ipfs.add instead'), 'ERR_UNEXPECTED_INPUT');
    }
    if (value._readableState) {
      yield* map(peekable, value => toFileObject({ content: value }, normaliseContent));
      return;
    }
    if (isBytes(value)) {
      yield toFileObject({ content: peekable }, normaliseContent);
      return;
    }
    if (isFileObject(value) || value[Symbol.iterator] || value[Symbol.asyncIterator] || isReadableStream(value) || isBlob(value)) {
      yield* map(peekable, value => toFileObject(value, normaliseContent));
      return;
    }
  }
  if (isFileObject(input)) {
    throw errCode(new Error('Unexpected input: single item passed - if you are using ipfs.addAll, please use ipfs.add instead'), 'ERR_UNEXPECTED_INPUT');
  }
  throw errCode(new Error('Unexpected input: ' + typeof input), 'ERR_UNEXPECTED_INPUT');
}
async function toFileObject(input, normaliseContent) {
  const {path, mode, mtime, content} = input;
  const file = {
    path: path || '',
    mode: parseMode(mode),
    mtime: parseMtime(mtime)
  };
  if (content) {
    file.content = await normaliseContent(content);
  } else if (!path) {
    file.content = await normaliseContent(input);
  }
  return file;
}