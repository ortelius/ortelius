export default findShardCid;
export type Blockstore = import('interface-blockstore').Blockstore;
export type CID = import('multiformats/cid').CID;
export type ExporterOptions = import('../types').ExporterOptions;
export type PBNode = import('@ipld/dag-pb').PBNode;
export type PBLink = import('@ipld/dag-pb').PBLink;
export type ShardTraversalContext = {
    hamtDepth: number;
    rootBucket: Bucket<boolean>;
    lastBucket: Bucket<boolean>;
};
/**
 * @typedef {object} ShardTraversalContext
 * @property {number} hamtDepth
 * @property {Bucket<boolean>} rootBucket
 * @property {Bucket<boolean>} lastBucket
 *
 * @param {PBNode} node
 * @param {string} name
 * @param {Blockstore} blockstore
 * @param {ShardTraversalContext} [context]
 * @param {ExporterOptions} [options]
 * @returns {Promise<CID|null>}
 */
declare function findShardCid(node: PBNode, name: string, blockstore: Blockstore, context?: ShardTraversalContext | undefined, options?: import("../types").ExporterOptions | undefined): Promise<CID | null>;
//# sourceMappingURL=find-cid-in-shard.d.ts.map