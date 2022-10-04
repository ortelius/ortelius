export = wrapHash;
/**
 * @param {(value: Uint8Array) => Promise<Uint8Array>} hashFn
 */
declare function wrapHash(hashFn: (value: Uint8Array) => Promise<Uint8Array>): (value: InfiniteHash | Uint8Array) => InfiniteHash;
declare namespace wrapHash {
    export { InfiniteHash };
}
declare class InfiniteHash {
    /**
     *
     * @param {Uint8Array} value
     * @param {(value: Uint8Array) => Promise<Uint8Array>} hashFn
     */
    constructor(value: Uint8Array, hashFn: (value: Uint8Array) => Promise<Uint8Array>);
    _value: Uint8Array;
    _hashFn: (value: Uint8Array) => Promise<Uint8Array>;
    _depth: number;
    _availableBits: number;
    _currentBufferIndex: number;
    /** @type {ConsumableBuffer[]} */
    _buffers: ConsumableBuffer[];
    /**
     * @param {number} bits
     */
    take(bits: number): Promise<number>;
    /**
     * @param {number} bits
     */
    untake(bits: number): void;
    _produceMoreBits(): Promise<void>;
}
import ConsumableBuffer = require("./consumable-buffer");
//# sourceMappingURL=consumable-hash.d.ts.map