import parallelBatch from 'it-parallel-batch';
import defaultOptions from './options.js';
import dagBuilderFn from './dag-builder/index.js';
import treeBuilderFn from './tree-builder.js';
export async function* importer(source, blockstore, options = {}) {
  const opts = defaultOptions(options);
  let dagBuilder;
  if (typeof options.dagBuilder === 'function') {
    dagBuilder = options.dagBuilder;
  } else {
    dagBuilder = dagBuilderFn;
  }
  let treeBuilder;
  if (typeof options.treeBuilder === 'function') {
    treeBuilder = options.treeBuilder;
  } else {
    treeBuilder = treeBuilderFn;
  }
  let candidates;
  if (Symbol.asyncIterator in source || Symbol.iterator in source) {
    candidates = source;
  } else {
    candidates = [source];
  }
  for await (const entry of treeBuilder(parallelBatch(dagBuilder(candidates, blockstore, opts), opts.fileImportConcurrency), blockstore, opts)) {
    yield {
      cid: entry.cid,
      path: entry.path,
      unixfs: entry.unixfs,
      size: entry.size
    };
  }
}