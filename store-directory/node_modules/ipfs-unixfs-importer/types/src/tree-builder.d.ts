export default treeBuilder;
export type ImportResult = import('./types').ImportResult;
export type InProgressImportResult = import('./types').InProgressImportResult;
export type ImporterOptions = import('./types').ImporterOptions;
export type Blockstore = import('interface-blockstore').Blockstore;
export type TreeBuilder = (source: AsyncIterable<InProgressImportResult>, blockstore: Blockstore, options: ImporterOptions) => AsyncIterable<ImportResult>;
/**
 * @type {TreeBuilder}
 */
declare function treeBuilder(source: AsyncIterable<import("./types").InProgressImportResult>, block: import("interface-blockstore").Blockstore, options: import("./types").ImporterOptions): AsyncIterable<import("./types").ImportResult>;
//# sourceMappingURL=tree-builder.d.ts.map