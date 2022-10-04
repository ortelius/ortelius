import {
  encode,
  code
} from '@ipld/dag-cbor';
import { sha256 } from 'multiformats/hashes/sha2';
import { CID } from 'multiformats/cid';
import { Block } from 'multiformats/block';
const MAX_SIZE = 1024 * 1024 * 4;
export async function mkRootNode(roots, blocks) {
  const value = [
    '/carbites/1',
    Array.from(roots),
    Array.from(blocks)
  ];
  const bytes = encode(value);
  if (bytes.length >= MAX_SIZE) {
    throw new Error('root node too big. The root node is bigger than 4MiB: the biggest message IPFS will bitswap. Split the CAR into smaller chunks.');
  }
  const hash = await sha256.digest(bytes);
  const cid = CID.create(1, code, hash);
  return new Block({
    cid,
    bytes,
    value
  });
}
export function isRootNode(node) {
  if (!Array.isArray(node))
    return false;
  if (node.length !== 3)
    return false;
  if (node[0] !== '/carbites/1')
    return false;
  if (!Array.isArray(node[1]))
    return false;
  if (!Array.isArray(node[2]))
    return false;
  return true;
}