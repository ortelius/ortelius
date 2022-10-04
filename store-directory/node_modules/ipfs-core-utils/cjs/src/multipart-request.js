'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var env_js = require('ipfs-utils/src/env.js');
var multipartRequest_node = require('./multipart-request.node.js');
var multipartRequest_browser = require('./multipart-request.browser.js');
var nanoid = require('nanoid');

async function multipartRequest(source, abortController, headers = {}, boundary = `-----------------------------${ nanoid.nanoid() }`) {
  let req = multipartRequest_node.multipartRequest;
  if (env_js.isElectronRenderer) {
    req = multipartRequest_browser.multipartRequest;
  }
  return req(source, abortController, headers, boundary);
}

exports.multipartRequest = multipartRequest;
