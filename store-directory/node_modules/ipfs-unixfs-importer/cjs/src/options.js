'use strict';

var mergeOptions = require('merge-options');
var sha2 = require('multiformats/hashes/sha2');
var murmur3 = require('@multiformats/murmur3');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var mergeOptions__default = /*#__PURE__*/_interopDefaultLegacy(mergeOptions);

async function hamtHashFn(buf) {
  return (await murmur3.murmur3128.encode(buf)).slice(0, 8).reverse();
}
const defaultOptions = {
  chunker: 'fixed',
  strategy: 'balanced',
  rawLeaves: false,
  onlyHash: false,
  reduceSingleLeafToSelf: true,
  hasher: sha2.sha256,
  leafType: 'file',
  cidVersion: 0,
  progress: () => () => {
  },
  shardSplitThreshold: 1000,
  fileImportConcurrency: 50,
  blockWriteConcurrency: 10,
  minChunkSize: 262144,
  maxChunkSize: 262144,
  avgChunkSize: 262144,
  window: 16,
  polynomial: 17437180132763652,
  maxChildrenPerNode: 174,
  layerRepeat: 4,
  wrapWithDirectory: false,
  recursive: false,
  hidden: false,
  timeout: undefined,
  hamtHashFn,
  hamtHashCode: 34,
  hamtBucketBits: 8
};
var defaultOptions$1 = (options = {}) => {
  const defaults = mergeOptions__default["default"].bind({ ignoreUndefined: true });
  return defaults(defaultOptions, options);
};

module.exports = defaultOptions$1;
