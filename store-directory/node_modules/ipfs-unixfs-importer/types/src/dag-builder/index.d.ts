export default dagBuilder;
export type File = import('../types').File;
export type Directory = import('../types').Directory;
export type DAGBuilder = import('../types').DAGBuilder;
export type Chunker = import('../types').Chunker;
export type ChunkValidator = import('../types').ChunkValidator;
/**
 * @type {DAGBuilder}
 */
declare function dagBuilder(source: AsyncIterable<import("../types").ImportCandidate> | Iterable<import("../types").ImportCandidate>, blockstore: import("interface-blockstore").Blockstore, options: import("../types").ImporterOptions): AsyncIterable<() => Promise<import("../types").InProgressImportResult>>;
//# sourceMappingURL=index.d.ts.map