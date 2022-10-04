import errCode from 'err-code';
import browserStreamToIt from 'browser-readablestream-to-it';
import itPeekable from 'it-peekable';
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
export async function* normaliseCandidateSingle(input, normaliseContent) {
  if (input === null || input === undefined) {
    throw errCode(new Error(`Unexpected input: ${ input }`), 'ERR_UNEXPECTED_INPUT');
  }
  if (typeof input === 'string' || input instanceof String) {
    yield toFileObject(input.toString(), normaliseContent);
    return;
  }
  if (isBytes(input) || isBlob(input)) {
    yield toFileObject(input, normaliseContent);
    return;
  }
  if (isReadableStream(input)) {
    input = browserStreamToIt(input);
  }
  if (Symbol.iterator in input || Symbol.asyncIterator in input) {
    const peekable = itPeekable(input);
    const {value, done} = await peekable.peek();
    if (done) {
      yield { content: [] };
      return;
    }
    peekable.push(value);
    if (Number.isInteger(value) || isBytes(value) || typeof value === 'string' || value instanceof String) {
      yield toFileObject(peekable, normaliseContent);
      return;
    }
    throw errCode(new Error('Unexpected input: multiple items passed - if you are using ipfs.add, please use ipfs.addAll instead'), 'ERR_UNEXPECTED_INPUT');
  }
  if (isFileObject(input)) {
    yield toFileObject(input, normaliseContent);
    return;
  }
  throw errCode(new Error('Unexpected input: cannot convert "' + typeof input + '" into ImportCandidate'), 'ERR_UNEXPECTED_INPUT');
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