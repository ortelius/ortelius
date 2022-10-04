'use strict';

var extractDataFromBlock = require('../../../utils/extract-data-from-block.js');
var validateOffsetAndLength = require('../../../utils/validate-offset-and-length.js');
var ipfsUnixfs = require('ipfs-unixfs');
var errCode = require('err-code');
var dagPb = require('@ipld/dag-pb');
var dagCbor = require('@ipld/dag-cbor');
var raw = require('multiformats/codecs/raw');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

function _interopNamespace(e) {
  if (e && e.__esModule) return e;
  var n = Object.create(null);
  if (e) {
    Object.keys(e).forEach(function (k) {
      if (k !== 'default') {
        var d = Object.getOwnPropertyDescriptor(e, k);
        Object.defineProperty(n, k, d.get ? d : {
          enumerable: true,
          get: function () { return e[k]; }
        });
      }
    });
  }
  n["default"] = e;
  return Object.freeze(n);
}

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);
var dagPb__namespace = /*#__PURE__*/_interopNamespace(dagPb);
var dagCbor__namespace = /*#__PURE__*/_interopNamespace(dagCbor);
var raw__namespace = /*#__PURE__*/_interopNamespace(raw);

async function* emitBytes(blockstore, node, start, end, streamPosition = 0, options) {
  if (node instanceof Uint8Array) {
    const buf = extractDataFromBlock(node, streamPosition, start, end);
    if (buf.length) {
      yield buf;
    }
    streamPosition += buf.length;
    return streamPosition;
  }
  if (node.Data == null) {
    throw errCode__default["default"](new Error('no data in PBNode'), 'ERR_NOT_UNIXFS');
  }
  let file;
  try {
    file = ipfsUnixfs.UnixFS.unmarshal(node.Data);
  } catch (err) {
    throw errCode__default["default"](err, 'ERR_NOT_UNIXFS');
  }
  if (file.data && file.data.length) {
    const buf = extractDataFromBlock(file.data, streamPosition, start, end);
    if (buf.length) {
      yield buf;
    }
    streamPosition += file.data.length;
  }
  let childStart = streamPosition;
  for (let i = 0; i < node.Links.length; i++) {
    const childLink = node.Links[i];
    const childEnd = streamPosition + file.blockSizes[i];
    if (start >= childStart && start < childEnd || end > childStart && end <= childEnd || start < childStart && end > childEnd) {
      const block = await blockstore.get(childLink.Hash, { signal: options.signal });
      let child;
      switch (childLink.Hash.code) {
      case dagPb__namespace.code:
        child = await dagPb__namespace.decode(block);
        break;
      case raw__namespace.code:
        child = block;
        break;
      case dagCbor__namespace.code:
        child = await dagCbor__namespace.decode(block);
        break;
      default:
        throw Error(`Unsupported codec: ${ childLink.Hash.code }`);
      }
      for await (const buf of emitBytes(blockstore, child, start, end, streamPosition, options)) {
        streamPosition += buf.length;
        yield buf;
      }
    }
    streamPosition = childEnd;
    childStart = childEnd + 1;
  }
}
const fileContent = (cid, node, unixfs, path, resolve, depth, blockstore) => {
  function yieldFileContent(options = {}) {
    const fileSize = unixfs.fileSize();
    if (fileSize === undefined) {
      throw new Error('File was a directory');
    }
    const {offset, length} = validateOffsetAndLength(fileSize, options.offset, options.length);
    const start = offset;
    const end = offset + length;
    return emitBytes(blockstore, node, start, end, 0, options);
  }
  return yieldFileContent;
};

module.exports = fileContent;
