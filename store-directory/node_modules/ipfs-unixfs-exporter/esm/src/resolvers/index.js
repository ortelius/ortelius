import errCode from 'err-code';
import * as dagPb from '@ipld/dag-pb';
import * as dagCbor from '@ipld/dag-cbor';
import * as raw from 'multiformats/codecs/raw';
import { identity } from 'multiformats/hashes/identity';
import dagPbResolver from './unixfs-v1/index.js';
import rawResolver from './raw.js';
import dagCborResolver from './dag-cbor.js';
import identifyResolver from './identity.js';
const resolvers = {
  [dagPb.code]: dagPbResolver,
  [raw.code]: rawResolver,
  [dagCbor.code]: dagCborResolver,
  [identity.code]: identifyResolver
};
function resolve(cid, name, path, toResolve, depth, blockstore, options) {
  const resolver = resolvers[cid.code];
  if (!resolver) {
    throw errCode(new Error(`No resolver for code ${ cid.code }`), 'ERR_NO_RESOLVER');
  }
  return resolver(cid, name, path, toResolve, resolve, depth, blockstore, options);
}
export default resolve;