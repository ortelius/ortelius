export = ConsumableBuffer;
declare class ConsumableBuffer {
    /**
     * @param {Uint8Array} value
     */
    constructor(value: Uint8Array);
    _value: Uint8Array;
    _currentBytePos: number;
    _currentBitPos: number;
    availableBits(): number;
    totalBits(): number;
    /**
     * @param {number} bits
     */
    take(bits: number): number;
    /**
     * @param {number} bits
     */
    untake(bits: number): void;
    _haveBits(): boolean;
}
//# sourceMappingURL=consumable-buffer.d.ts.map