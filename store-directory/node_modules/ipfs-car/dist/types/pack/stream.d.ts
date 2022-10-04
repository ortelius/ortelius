/// <reference types="node" />
import { Writable } from 'stream';
import type { PackProperties } from './index';
export interface PackToStreamProperties extends PackProperties {
    input: string | Iterable<string> | AsyncIterable<string>;
    writable: Writable;
}
export declare function packToStream({ input, writable, blockstore: userBlockstore, hasher, maxChunkSize, maxChildrenPerNode, wrapWithDirectory, rawLeaves }: PackToStreamProperties): Promise<{
    root: import("multiformats").CID;
}>;
