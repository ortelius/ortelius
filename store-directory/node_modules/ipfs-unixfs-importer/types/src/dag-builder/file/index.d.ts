export default fileBuilder;
export type Blockstore = import('interface-blockstore').Blockstore;
export type File = import('../../types').File;
export type ImporterOptions = import('../../types').ImporterOptions;
export type Reducer = import('../../types').Reducer;
export type DAGBuilder = import('../../types').DAGBuilder;
export type FileDAGBuilder = import('../../types').FileDAGBuilder;
/**
 * @type {import('../../types').UnixFSV1DagBuilder<File>}
 */
declare function fileBuilder(file: import("../../types").File, block: import("interface-blockstore").Blockstore, options: import("../../types").ImporterOptions): Promise<import("../../types").InProgressImportResult>;
//# sourceMappingURL=index.d.ts.map