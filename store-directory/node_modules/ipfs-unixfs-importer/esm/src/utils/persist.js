import { CID } from 'multiformats/cid';
import * as dagPb from '@ipld/dag-pb';
import { sha256 } from 'multiformats/hashes/sha2';
const persist = async (buffer, blockstore, options) => {
  if (!options.codec) {
    options.codec = dagPb;
  }
  if (!options.hasher) {
    options.hasher = sha256;
  }
  if (options.cidVersion === undefined) {
    options.cidVersion = 1;
  }
  if (options.codec === dagPb && options.hasher !== sha256) {
    options.cidVersion = 1;
  }
  const multihash = await options.hasher.digest(buffer);
  const cid = CID.create(options.cidVersion, options.codec.code, multihash);
  if (!options.onlyHash) {
    await blockstore.put(cid, buffer, { signal: options.signal });
  }
  return cid;
};
export default persist;