export default bufferImporter;
export type BufferImporter = import('../../types').BufferImporter;
/**
 * @typedef {import('../../types').BufferImporter} BufferImporter
 */
/**
 * @type {BufferImporter}
 */
declare function bufferImporter(file: import("../../types").File, block: import("interface-blockstore").Blockstore, options: import("../../types").ImporterOptions): AsyncIterable<() => Promise<import("../../types").InProgressImportResult>>;
//# sourceMappingURL=buffer-importer.d.ts.map