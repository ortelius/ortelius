import { encode, code } from '@ipld/dag-cbor'
import { sha256 } from 'multiformats/hashes/sha2'
import { CID } from 'multiformats/cid'
import { Block } from 'multiformats/block'

/** @typedef {['/carbites/1', CID[], CID[]]} RootNode */

const MAX_SIZE = 1024 * 1024 * 4

/**
 * Make a carbites root node. Format: ['/carbites/1', roots, blocks]
 * @param {Iterable<import('multiformats/cid').CID>} roots
 * @param {Iterable<import('multiformats/cid').CID>} blocks
 * @returns {Promise<import('multiformats/block').Block<RootNode>>}
 */
export async function mkRootNode (roots, blocks) {
  /** @type {RootNode} */
  const value = ['/carbites/1', Array.from(roots), Array.from(blocks)]
  const bytes = encode(value)
  // FIXME: Given a V1 CID of ~36 bytes and the default IPFS chunk size of
  // 262,144 bytes you'd need to be splitting at 30GiB or more to experience
  // this error.
  if (bytes.length >= MAX_SIZE) {
    throw new Error('root node too big. The root node is bigger than 4MiB: the biggest message IPFS will bitswap. Split the CAR into smaller chunks.')
  }
  const hash = await sha256.digest(bytes)
  const cid = CID.create(1, code, hash)
  return new Block({ cid, bytes, value })
}

/**
 * @param {any} node
 * @returns {boolean}
 */
export function isRootNode (node) {
  if (!Array.isArray(node)) return false
  if (node.length !== 3) return false
  if (node[0] !== '/carbites/1') return false
  if (!Array.isArray(node[1])) return false
  if (!Array.isArray(node[2])) return false
  return true
}
