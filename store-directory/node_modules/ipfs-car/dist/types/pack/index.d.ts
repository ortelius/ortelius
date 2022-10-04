import type { ImportCandidateStream, ImportCandidate } from 'ipfs-core-types/src/utils';
import type { MultihashHasher } from 'multiformats/hashes/interface';
export type { ImportCandidateStream };
import { Blockstore } from '../blockstore/index';
export interface PackProperties {
    input: ImportCandidateStream | ImportCandidate;
    blockstore?: Blockstore;
    maxChunkSize?: number;
    maxChildrenPerNode?: number;
    wrapWithDirectory?: boolean;
    hasher?: MultihashHasher;
    /**
     * Use raw codec for leaf nodes. Default: true.
     */
    rawLeaves?: boolean;
}
export declare function pack({ input, blockstore: userBlockstore, hasher, maxChunkSize, maxChildrenPerNode, wrapWithDirectory, rawLeaves }: PackProperties): Promise<{
    root: import("multiformats").CID;
    out: AsyncIterable<Uint8Array>;
}>;
