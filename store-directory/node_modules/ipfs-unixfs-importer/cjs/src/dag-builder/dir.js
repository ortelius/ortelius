'use strict';

var ipfsUnixfs = require('ipfs-unixfs');
var persist = require('../utils/persist.js');
var dagPb = require('@ipld/dag-pb');

const dirBuilder = async (item, blockstore, options) => {
  const unixfs = new ipfsUnixfs.UnixFS({
    type: 'directory',
    mtime: item.mtime,
    mode: item.mode
  });
  const buffer = dagPb.encode(dagPb.prepare({ Data: unixfs.marshal() }));
  const cid = await persist(buffer, blockstore, options);
  const path = item.path;
  return {
    cid,
    path,
    unixfs,
    size: buffer.length
  };
};

module.exports = dirBuilder;
