export default validateChunks;
export type ChunkValidator = import('../types').ChunkValidator;
/**
 * @typedef {import('../types').ChunkValidator} ChunkValidator
 */
/**
 * @type {ChunkValidator}
 */
declare function validateChunks(source: AsyncIterable<Uint8Array>): AsyncIterable<Uint8Array>;
//# sourceMappingURL=validate-chunks.d.ts.map