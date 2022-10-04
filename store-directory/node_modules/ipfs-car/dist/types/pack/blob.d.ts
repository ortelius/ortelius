import type { ImportCandidateStream } from 'ipfs-core-types/src/utils';
export type { ImportCandidateStream };
import type { PackProperties } from './index';
export declare function packToBlob({ input, blockstore: userBlockstore, hasher, maxChunkSize, maxChildrenPerNode, wrapWithDirectory, rawLeaves }: PackProperties): Promise<{
    root: import("multiformats").CID;
    car: Blob;
}>;
