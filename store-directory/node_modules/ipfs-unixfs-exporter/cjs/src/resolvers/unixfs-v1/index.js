'use strict';

var errCode = require('err-code');
var ipfsUnixfs = require('ipfs-unixfs');
var findCidInShard = require('../../utils/find-cid-in-shard.js');
var dagPb = require('@ipld/dag-pb');
var file = require('./content/file.js');
var directory = require('./content/directory.js');
var hamtShardedDirectory = require('./content/hamt-sharded-directory.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);

const findLinkCid = (node, name) => {
  const link = node.Links.find(link => link.Name === name);
  return link && link.Hash;
};
const contentExporters = {
  raw: file,
  file: file,
  directory: directory,
  'hamt-sharded-directory': hamtShardedDirectory,
  metadata: (cid, node, unixfs, path, resolve, depth, blockstore) => {
    return () => [];
  },
  symlink: (cid, node, unixfs, path, resolve, depth, blockstore) => {
    return () => [];
  }
};
const unixFsResolver = async (cid, name, path, toResolve, resolve, depth, blockstore, options) => {
  const block = await blockstore.get(cid, options);
  const node = dagPb.decode(block);
  let unixfs;
  let next;
  if (!name) {
    name = cid.toString();
  }
  if (node.Data == null) {
    throw errCode__default["default"](new Error('no data in PBNode'), 'ERR_NOT_UNIXFS');
  }
  try {
    unixfs = ipfsUnixfs.UnixFS.unmarshal(node.Data);
  } catch (err) {
    throw errCode__default["default"](err, 'ERR_NOT_UNIXFS');
  }
  if (!path) {
    path = name;
  }
  if (toResolve.length) {
    let linkCid;
    if (unixfs && unixfs.type === 'hamt-sharded-directory') {
      linkCid = await findCidInShard(node, toResolve[0], blockstore);
    } else {
      linkCid = findLinkCid(node, toResolve[0]);
    }
    if (!linkCid) {
      throw errCode__default["default"](new Error('file does not exist'), 'ERR_NOT_FOUND');
    }
    const nextName = toResolve.shift();
    const nextPath = `${ path }/${ nextName }`;
    next = {
      cid: linkCid,
      toResolve,
      name: nextName || '',
      path: nextPath
    };
  }
  return {
    entry: {
      type: unixfs.isDirectory() ? 'directory' : 'file',
      name,
      path,
      cid,
      content: contentExporters[unixfs.type](cid, node, unixfs, path, resolve, depth, blockstore),
      unixfs,
      depth,
      node,
      size: unixfs.fileSize()
    },
    next
  };
};

module.exports = unixFsResolver;
