'use strict';

var dirFlat = require('./dir-flat.js');
var flatToShard = require('./flat-to-shard.js');
var dir = require('./dir.js');
var toPathComponents = require('./utils/to-path-components.js');

async function addToTree(elem, tree, options) {
  const pathElems = toPathComponents(elem.path || '');
  const lastIndex = pathElems.length - 1;
  let parent = tree;
  let currentPath = '';
  for (let i = 0; i < pathElems.length; i++) {
    const pathElem = pathElems[i];
    currentPath += `${ currentPath ? '/' : '' }${ pathElem }`;
    const last = i === lastIndex;
    parent.dirty = true;
    parent.cid = undefined;
    parent.size = undefined;
    if (last) {
      await parent.put(pathElem, elem);
      tree = await flatToShard(null, parent, options.shardSplitThreshold, options);
    } else {
      let dir$1 = await parent.get(pathElem);
      if (!dir$1 || !(dir$1 instanceof dir)) {
        dir$1 = new dirFlat({
          root: false,
          dir: true,
          parent: parent,
          parentKey: pathElem,
          path: currentPath,
          dirty: true,
          flat: true,
          mtime: dir$1 && dir$1.unixfs && dir$1.unixfs.mtime,
          mode: dir$1 && dir$1.unixfs && dir$1.unixfs.mode
        }, options);
      }
      await parent.put(pathElem, dir$1);
      parent = dir$1;
    }
  }
  return tree;
}
async function* flushAndYield(tree, blockstore) {
  if (!(tree instanceof dir)) {
    if (tree && tree.unixfs && tree.unixfs.isDirectory()) {
      yield tree;
    }
    return;
  }
  yield* tree.flush(blockstore);
}
async function* treeBuilder(source, block, options) {
  let tree = new dirFlat({
    root: true,
    dir: true,
    path: '',
    dirty: true,
    flat: true
  }, options);
  for await (const entry of source) {
    if (!entry) {
      continue;
    }
    tree = await addToTree(entry, tree, options);
    if (!entry.unixfs || !entry.unixfs.isDirectory()) {
      yield entry;
    }
  }
  if (options.wrapWithDirectory) {
    yield* flushAndYield(tree, block);
  } else {
    for await (const unwrapped of tree.eachChildSeries()) {
      if (!unwrapped) {
        continue;
      }
      yield* flushAndYield(unwrapped.child, block);
    }
  }
}

module.exports = treeBuilder;
