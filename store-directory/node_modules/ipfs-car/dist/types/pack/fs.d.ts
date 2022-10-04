import type { PackProperties } from './index';
export interface PackToFsProperties extends PackProperties {
    input: string | Iterable<string> | AsyncIterable<string>;
    output?: string;
}
export declare function packToFs({ input, output, blockstore: userBlockstore, hasher, maxChunkSize, maxChildrenPerNode, wrapWithDirectory, rawLeaves }: PackToFsProperties): Promise<{
    root: import("multiformats").CID;
    filename: string;
}>;
