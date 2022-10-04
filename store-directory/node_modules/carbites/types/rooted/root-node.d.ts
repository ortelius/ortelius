/**
 * Make a carbites root node. Format: ['/carbites/1', roots, blocks]
 * @param {Iterable<import('multiformats/cid').CID>} roots
 * @param {Iterable<import('multiformats/cid').CID>} blocks
 * @returns {Promise<import('multiformats/block').Block<RootNode>>}
 */
export function mkRootNode(roots: Iterable<import('multiformats/cid').CID>, blocks: Iterable<import('multiformats/cid').CID>): Promise<import('multiformats/block').Block<RootNode>>;
/**
 * @param {any} node
 * @returns {boolean}
 */
export function isRootNode(node: any): boolean;
export type RootNode = ['/carbites/1', CID[], CID[]];
import { CID } from "multiformats/cid";
//# sourceMappingURL=root-node.d.ts.map