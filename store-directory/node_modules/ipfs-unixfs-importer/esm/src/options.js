import mergeOptions from 'merge-options';
import { sha256 } from 'multiformats/hashes/sha2';
import { murmur3128 } from '@multiformats/murmur3';
async function hamtHashFn(buf) {
  return (await murmur3128.encode(buf)).slice(0, 8).reverse();
}
const defaultOptions = {
  chunker: 'fixed',
  strategy: 'balanced',
  rawLeaves: false,
  onlyHash: false,
  reduceSingleLeafToSelf: true,
  hasher: sha256,
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
export default (options = {}) => {
  const defaults = mergeOptions.bind({ ignoreUndefined: true });
  return defaults(defaultOptions, options);
};