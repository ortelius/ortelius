export = Convert;
/**
 * @param {string} proto
 * @param {string | Uint8Array} a
 */
declare function Convert(proto: string, a: string | Uint8Array): any;
declare namespace Convert {
    /**
     * Convert [code,Uint8Array] to string
     *
     * @param {number|string} proto
     * @param {Uint8Array} buf
     * @returns {string}
     */
    function toString(proto: string | number, buf: Uint8Array): string;
    function toBytes(proto: string | number, str: string): any;
}
//# sourceMappingURL=convert.d.ts.map