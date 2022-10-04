export default trickleStream;
export type UnixFS = import('ipfs-unixfs').UnixFS;
export type ImporterOptions = import('../../types').ImporterOptions;
export type InProgressImportResult = import('../../types').InProgressImportResult;
export type TrickleDagNode = import('../../types').TrickleDagNode;
export type Reducer = import('../../types').Reducer;
export type FileDAGBuilder = import('../../types').FileDAGBuilder;
/**
 * @typedef {import('ipfs-unixfs').UnixFS} UnixFS
 * @typedef {import('../../types').ImporterOptions} ImporterOptions
 * @typedef {import('../../types').InProgressImportResult} InProgressImportResult
 * @typedef {import('../../types').TrickleDagNode} TrickleDagNode
 * @typedef {import('../../types').Reducer} Reducer
 * @typedef {import('../../types').FileDAGBuilder} FileDAGBuilder
 */
/**
 * @type {FileDAGBuilder}
 */
declare function trickleStream(source: AsyncIterable<import("../../types").InProgressImportResult> | Iterable<import("../../types").InProgressImportResult>, reduce: import("../../types").Reducer, options: import("../../types").ImporterOptions): Promise<import("../../types").InProgressImportResult>;
//# sourceMappingURL=trickle.d.ts.map