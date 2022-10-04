'use strict';

const directoryContent = (cid, node, unixfs, path, resolve, depth, blockstore) => {
  async function* yieldDirectoryContent(options = {}) {
    const offset = options.offset || 0;
    const length = options.length || node.Links.length;
    const links = node.Links.slice(offset, length);
    for (const link of links) {
      const result = await resolve(link.Hash, link.Name || '', `${ path }/${ link.Name || '' }`, [], depth + 1, blockstore, options);
      if (result.entry) {
        yield result.entry;
      }
    }
  }
  return yieldDirectoryContent;
};

module.exports = directoryContent;
