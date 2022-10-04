/**
 * @typedef {import('interface-blockstore').Blockstore} Blockstore
 * @typedef {import('./types').ImportCandidate} ImportCandidate
 * @typedef {import('./types').UserImporterOptions} UserImporterOptions
 * @typedef {import('./types').ImporterOptions} ImporterOptions
 * @typedef {import('./types').Directory} Directory
 * @typedef {import('./types').File} File
 * @typedef {import('./types').ImportResult} ImportResult
 *
 * @typedef {import('./types').Chunker} Chunker
 * @typedef {import('./types').DAGBuilder} DAGBuilder
 * @typedef {import('./types').TreeBuilder} TreeBuilder
 * @typedef {import('./types').BufferImporter} BufferImporter
 * @typedef {import('./types').ChunkValidator} ChunkValidator
 * @typedef {import('./types').Reducer} Reducer
 * @typedef {import('./types').ProgressHandler} ProgressHandler
 */
/**
 * @param {AsyncIterable<ImportCandidate> | Iterable<ImportCandidate> | ImportCandidate} source
 * @param {Blockstore} blockstore
 * @param {UserImporterOptions} options
 */
export function importer(source: AsyncIterable<ImportCandidate> | Iterable<ImportCandidate> | ImportCandidate, blockstore: Blockstore, options?: UserImporterOptions): AsyncGenerator<{
    cid: import("multiformats").CID;
    path: string | undefined;
    unixfs: import("ipfs-unixfs").UnixFS | undefined;
    size: number;
}, void, unknown>;
export type Blockstore = import('interface-blockstore').Blockstore;
export type ImportCandidate = import('./types').ImportCandidate;
export type UserImporterOptions = import('./types').UserImporterOptions;
export type ImporterOptions = import('./types').ImporterOptions;
export type Directory = import('./types').Directory;
export type File = import('./types').File;
export type ImportResult = import('./types').ImportResult;
export type Chunker = import('./types').Chunker;
export type DAGBuilder = import('./types').DAGBuilder;
export type TreeBuilder = import('./types').TreeBuilder;
export type BufferImporter = import('./types').BufferImporter;
export type ChunkValidator = import('./types').ChunkValidator;
export type Reducer = import('./types').Reducer;
export type ProgressHandler = import('./types').ProgressHandler;
//# sourceMappingURL=index.d.ts.map