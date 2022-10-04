'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var dagCbor = require('@ipld/dag-cbor');
var sha2 = require('multiformats/hashes/sha2');
var cid = require('multiformats/cid');
var block = require('multiformats/block');

const MAX_SIZE = 1024 * 1024 * 4;
async function mkRootNode(roots, blocks) {
  const value = [
    '/carbites/1',
    Array.from(roots),
    Array.from(blocks)
  ];
  const bytes = dagCbor.encode(value);
  if (bytes.length >= MAX_SIZE) {
    throw new Error('root node too big. The root node is bigger than 4MiB: the biggest message IPFS will bitswap. Split the CAR into smaller chunks.');
  }
  const hash = await sha2.sha256.digest(bytes);
  const cid$1 = cid.CID.create(1, dagCbor.code, hash);
  return new block.Block({
    cid: cid$1,
    bytes,
    value
  });
}
function isRootNode(node) {
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

exports.isRootNode = isRootNode;
exports.mkRootNode = mkRootNode;
