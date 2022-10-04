import { isElectronRenderer } from 'ipfs-utils/src/env.js';
import { multipartRequest as multipartRequestNode } from './multipart-request.node.js';
import { multipartRequest as multipartRequestBrowser } from './multipart-request.browser.js';
import { nanoid } from 'nanoid';
export async function multipartRequest(source, abortController, headers = {}, boundary = `-----------------------------${ nanoid() }`) {
  let req = multipartRequestNode;
  if (isElectronRenderer) {
    req = multipartRequestBrowser;
  }
  return req(source, abortController, headers, boundary);
}