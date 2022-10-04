import type { UnixFS, Mtime } from 'ipfs-unixfs';
import type { CID, CIDVersion } from 'multiformats/cid';
import type { MultihashHasher } from 'multiformats/hashes/interface';
import type { BlockCodec } from 'multiformats/codecs/interface';
import type { Blockstore } from 'interface-blockstore';
export interface ImportCandidate {
    path?: string;
    content?: AsyncIterable<Uint8Array> | Iterable<Uint8Array> | Uint8Array;
    mtime?: Mtime;
    mode?: number;
}
export interface File {
    content: AsyncIterable<Uint8Array>;
    path?: string;
    mtime?: Mtime;
    mode?: number;
}
export interface Directory {
    path?: string;
    mtime?: Mtime;
    mode?: number;
}
export interface ImportResult {
    cid: CID;
    size: number;
    path?: string;
    unixfs?: UnixFS;
}
export interface InProgressImportResult extends ImportResult {
    single?: boolean;
}
export declare type ChunkerType = 'fixed' | 'rabin';
export interface ProgressHandler {
    (chunkSize: number, path?: string): void;
}
export interface HamtHashFn {
    (value: Uint8Array): Promise<Uint8Array>;
}
export interface Chunker {
    (source: AsyncIterable<Uint8Array>, options: ImporterOptions): AsyncIterable<Uint8Array>;
}
export interface DAGBuilder {
    (source: AsyncIterable<ImportCandidate> | Iterable<ImportCandidate>, blockstore: Blockstore, options: ImporterOptions): AsyncIterable<() => Promise<InProgressImportResult>>;
}
export interface TreeBuilder {
    (source: AsyncIterable<InProgressImportResult>, blockstore: Blockstore, options: ImporterOptions): AsyncIterable<ImportResult>;
}
export interface BufferImporter {
    (file: File, blockstore: Blockstore, options: ImporterOptions): AsyncIterable<() => Promise<InProgressImportResult>>;
}
export interface ChunkValidator {
    (source: AsyncIterable<Uint8Array>, options: ImporterOptions): AsyncIterable<Uint8Array>;
}
export interface UnixFSV1DagBuilder<T> {
    (item: T, blockstore: Blockstore, options: ImporterOptions): Promise<InProgressImportResult>;
}
export interface Reducer {
    (leaves: InProgressImportResult[]): Promise<InProgressImportResult>;
}
export interface FileDAGBuilder {
    (source: AsyncIterable<InProgressImportResult> | Iterable<InProgressImportResult>, reducer: Reducer, options: ImporterOptions): Promise<InProgressImportResult>;
}
export interface UserImporterOptions {
    strategy?: 'balanced' | 'flat' | 'trickle';
    rawLeaves?: boolean;
    onlyHash?: boolean;
    reduceSingleLeafToSelf?: boolean;
    hasher?: MultihashHasher;
    leafType?: 'file' | 'raw';
    cidVersion?: CIDVersion;
    progress?: ProgressHandler;
    shardSplitThreshold?: number;
    fileImportConcurrency?: number;
    blockWriteConcurrency?: number;
    minChunkSize?: number;
    maxChunkSize?: number;
    avgChunkSize?: number;
    window?: number;
    polynomial?: number;
    maxChildrenPerNode?: number;
    layerRepeat?: number;
    wrapWithDirectory?: boolean;
    recursive?: boolean;
    hidden?: boolean;
    timeout?: number;
    hamtHashFn?: HamtHashFn;
    hamtBucketBits?: number;
    hamtHashCode?: number;
    chunker?: ChunkerType | Chunker;
    dagBuilder?: DAGBuilder;
    treeBuilder?: TreeBuilder;
    bufferImporter?: BufferImporter;
    chunkValidator?: ChunkValidator;
}
export interface ImporterOptions {
    strategy: 'balanced' | 'flat' | 'trickle';
    rawLeaves: boolean;
    onlyHash: boolean;
    reduceSingleLeafToSelf: boolean;
    hasher: MultihashHasher;
    leafType: 'file' | 'raw';
    cidVersion: CIDVersion;
    progress: ProgressHandler;
    shardSplitThreshold: number;
    fileImportConcurrency: number;
    blockWriteConcurrency: number;
    minChunkSize: number;
    maxChunkSize: number;
    avgChunkSize: number;
    window: number;
    polynomial: number;
    maxChildrenPerNode: number;
    layerRepeat: number;
    wrapWithDirectory: boolean;
    recursive: boolean;
    hidden: boolean;
    timeout?: number;
    hamtHashFn: HamtHashFn;
    hamtBucketBits: number;
    hamtHashCode: number;
    chunker: ChunkerType | Chunker;
    dagBuilder?: DAGBuilder;
    treeBuilder?: TreeBuilder;
    bufferImporter?: BufferImporter;
    chunkValidator?: ChunkValidator;
}
export interface TrickleDagNode {
    children: InProgressImportResult[];
    depth: number;
    maxDepth: number;
    maxChildren: number;
    data?: InProgressImportResult[];
    parent?: TrickleDagNode;
    cid?: CID;
    size?: number;
    unixfs?: UnixFS;
}
export interface PersistOptions {
    codec?: BlockCodec<any, any>;
    hasher: MultihashHasher;
    cidVersion: CIDVersion;
    onlyHash: boolean;
    signal?: AbortSignal;
}
//# sourceMappingURL=types.d.ts.map