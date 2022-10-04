export default rabinChunker;
export type RabinOptions = {
    min: number;
    max: number;
    bits: number;
    window: number;
    polynomial: number;
};
/**
 * @typedef {object} RabinOptions
 * @property {number} min
 * @property {number} max
 * @property {number} bits
 * @property {number} window
 * @property {number} polynomial
 */
/**
 * @type {import('../types').Chunker}
 */
declare function rabinChunker(source: AsyncIterable<Uint8Array>, options: import("../types").ImporterOptions): AsyncIterable<Uint8Array>;
//# sourceMappingURL=rabin.d.ts.map