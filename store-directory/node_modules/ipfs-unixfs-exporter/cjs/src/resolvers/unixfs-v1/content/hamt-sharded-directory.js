'use strict';

var dagPb = require('@ipld/dag-pb');

const hamtShardedDirectoryContent = (cid, node, unixfs, path, resolve, depth, blockstore) => {
  function yieldHamtDirectoryContent(options = {}) {
    return listDirectory(node, path, resolve, depth, blockstore, options);
  }
  return yieldHamtDirectoryContent;
};
async function* listDirectory(node, path, resolve, depth, blockstore, options) {
  const links = node.Links;
  for (const link of links) {
    const name = link.Name != null ? link.Name.substring(2) : null;
    if (name) {
      const result = await resolve(link.Hash, name, `${ path }/${ name }`, [], depth + 1, blockstore, options);
      yield result.entry;
    } else {
      const block = await blockstore.get(link.Hash);
      node = dagPb.decode(block);
      for await (const file of listDirectory(node, path, resolve, depth, blockstore, options)) {
        yield file;
      }
    }
  }
}

module.exports = hamtShardedDirectoryContent;
