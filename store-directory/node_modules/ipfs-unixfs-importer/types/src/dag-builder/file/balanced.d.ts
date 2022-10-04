export default balanced;
export type FileDAGBuilder = import('../../types').FileDAGBuilder;
/**
 * @typedef {import('../../types').FileDAGBuilder} FileDAGBuilder
 */
/**
 * @type {FileDAGBuilder}
 */
declare function balanced(source: AsyncIterable<import("../../types").InProgressImportResult> | Iterable<import("../../types").InProgressImportResult>, reduce: import("../../types").Reducer, options: import("../../types").ImporterOptions): Promise<import("../../types").InProgressImportResult>;
//# sourceMappingURL=balanced.d.ts.map