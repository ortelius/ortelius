import { normaliseInput } from './files/normalise-input-multiple.js';
import { nanoid } from 'nanoid';
import { modeToString } from './mode-to-string.js';
import mergeOpts from 'merge-options';
import toStream from 'it-to-stream';
import debug from 'debug';
import itPeekable from 'it-peekable';
const merge = mergeOpts.bind({ ignoreUndefined: true });
const log = debug('ipfs:core-utils:multipart-request');
export async function multipartRequest(source, abortController, headers = {}, boundary = `-----------------------------${ nanoid() }`) {
  async function* streamFiles(source) {
    try {
      let index = 0;
      for await (const {content, path, mode, mtime} of source) {
        let fileSuffix = '';
        const type = content ? 'file' : 'dir';
        if (index > 0) {
          yield '\r\n';
          fileSuffix = `-${ index }`;
        }
        let fieldName = type + fileSuffix;
        const qs = [];
        if (mode !== null && mode !== undefined) {
          qs.push(`mode=${ modeToString(mode) }`);
        }
        if (mtime != null) {
          const {secs, nsecs} = mtime;
          qs.push(`mtime=${ secs }`);
          if (nsecs != null) {
            qs.push(`mtime-nsecs=${ nsecs }`);
          }
        }
        if (qs.length) {
          fieldName = `${ fieldName }?${ qs.join('&') }`;
        }
        yield `--${ boundary }\r\n`;
        yield `Content-Disposition: form-data; name="${ fieldName }"; filename="${ encodeURIComponent(path || '') }"\r\n`;
        yield `Content-Type: ${ content ? 'application/octet-stream' : 'application/x-directory' }\r\n`;
        yield '\r\n';
        if (content) {
          yield* content;
        }
        index++;
      }
    } catch (err) {
      log(err);
      abortController.abort();
    } finally {
      yield `\r\n--${ boundary }--\r\n`;
    }
  }
  const peekable = itPeekable(normaliseInput(source));
  const {value, done} = await peekable.peek();
  if (!done) {
    peekable.push(value);
  }
  return {
    parts: null,
    total: -1,
    headers: merge(headers, { 'Content-Type': `multipart/form-data; boundary=${ boundary }` }),
    body: toStream(streamFiles(peekable))
  };
}