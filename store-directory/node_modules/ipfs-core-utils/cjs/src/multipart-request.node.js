'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var normaliseInputMultiple = require('./files/normalise-input-multiple.js');
var nanoid = require('nanoid');
var modeToString = require('./mode-to-string.js');
var mergeOpts = require('merge-options');
var toStream = require('it-to-stream');
var debug = require('debug');
var itPeekable = require('it-peekable');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var mergeOpts__default = /*#__PURE__*/_interopDefaultLegacy(mergeOpts);
var toStream__default = /*#__PURE__*/_interopDefaultLegacy(toStream);
var debug__default = /*#__PURE__*/_interopDefaultLegacy(debug);
var itPeekable__default = /*#__PURE__*/_interopDefaultLegacy(itPeekable);

const merge = mergeOpts__default["default"].bind({ ignoreUndefined: true });
const log = debug__default["default"]('ipfs:core-utils:multipart-request');
async function multipartRequest(source, abortController, headers = {}, boundary = `-----------------------------${ nanoid.nanoid() }`) {
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
          qs.push(`mode=${ modeToString.modeToString(mode) }`);
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
  const peekable = itPeekable__default["default"](normaliseInputMultiple.normaliseInput(source));
  const {value, done} = await peekable.peek();
  if (!done) {
    peekable.push(value);
  }
  return {
    parts: null,
    total: -1,
    headers: merge(headers, { 'Content-Type': `multipart/form-data; boundary=${ boundary }` }),
    body: toStream__default["default"](streamFiles(peekable))
  };
}

exports.multipartRequest = multipartRequest;
