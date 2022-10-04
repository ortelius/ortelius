'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var errCode = require('err-code');
var cid = require('multiformats/cid');
var index = require('./resolvers/index.js');
var last = require('it-last');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);
var last__default = /*#__PURE__*/_interopDefaultLegacy(last);

const toPathComponents = (path = '') => {
  return (path.trim().match(/([^\\^/]|\\\/)+/g) || []).filter(Boolean);
};
const cidAndRest = path => {
  if (path instanceof Uint8Array) {
    return {
      cid: cid.CID.decode(path),
      toResolve: []
    };
  }
  const cid$1 = cid.CID.asCID(path);
  if (cid$1) {
    return {
      cid: cid$1,
      toResolve: []
    };
  }
  if (typeof path === 'string') {
    if (path.indexOf('/ipfs/') === 0) {
      path = path.substring(6);
    }
    const output = toPathComponents(path);
    return {
      cid: cid.CID.parse(output[0]),
      toResolve: output.slice(1)
    };
  }
  throw errCode__default["default"](new Error(`Unknown path type ${ path }`), 'ERR_BAD_PATH');
};
async function* walkPath(path, blockstore, options = {}) {
  let {cid, toResolve} = cidAndRest(path);
  let name = cid.toString();
  let entryPath = name;
  const startingDepth = toResolve.length;
  while (true) {
    const result = await index(cid, name, entryPath, toResolve, startingDepth, blockstore, options);
    if (!result.entry && !result.next) {
      throw errCode__default["default"](new Error(`Could not resolve ${ path }`), 'ERR_NOT_FOUND');
    }
    if (result.entry) {
      yield result.entry;
    }
    if (!result.next) {
      return;
    }
    toResolve = result.next.toResolve;
    cid = result.next.cid;
    name = result.next.name;
    entryPath = result.next.path;
  }
}
async function exporter(path, blockstore, options = {}) {
  const result = await last__default["default"](walkPath(path, blockstore, options));
  if (!result) {
    throw errCode__default["default"](new Error(`Could not resolve ${ path }`), 'ERR_NOT_FOUND');
  }
  return result;
}
async function* recursive(path, blockstore, options = {}) {
  const node = await exporter(path, blockstore, options);
  if (!node) {
    return;
  }
  yield node;
  if (node.type === 'directory') {
    for await (const child of recurse(node, options)) {
      yield child;
    }
  }
  async function* recurse(node, options) {
    for await (const file of node.content(options)) {
      yield file;
      if (file instanceof Uint8Array) {
        continue;
      }
      if (file.type === 'directory') {
        yield* recurse(file, options);
      }
    }
  }
}

exports.exporter = exporter;
exports.recursive = recursive;
exports.walkPath = walkPath;
