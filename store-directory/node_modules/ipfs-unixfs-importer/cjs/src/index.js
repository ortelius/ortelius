'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var parallelBatch = require('it-parallel-batch');
var options = require('./options.js');
var index = require('./dag-builder/index.js');
var treeBuilder = require('./tree-builder.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var parallelBatch__default = /*#__PURE__*/_interopDefaultLegacy(parallelBatch);

async function* importer(source, blockstore, options$1 = {}) {
  const opts = options(options$1);
  let dagBuilder;
  if (typeof options$1.dagBuilder === 'function') {
    dagBuilder = options$1.dagBuilder;
  } else {
    dagBuilder = index;
  }
  let treeBuilder$1;
  if (typeof options$1.treeBuilder === 'function') {
    treeBuilder$1 = options$1.treeBuilder;
  } else {
    treeBuilder$1 = treeBuilder;
  }
  let candidates;
  if (Symbol.asyncIterator in source || Symbol.iterator in source) {
    candidates = source;
  } else {
    candidates = [source];
  }
  for await (const entry of treeBuilder$1(parallelBatch__default["default"](dagBuilder(candidates, blockstore, opts), opts.fileImportConcurrency), blockstore, opts)) {
    yield {
      cid: entry.cid,
      path: entry.path,
      unixfs: entry.unixfs,
      size: entry.size
    };
  }
}

exports.importer = importer;
