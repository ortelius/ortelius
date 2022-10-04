/**
 * Returns a `Uint8Array` of the requested size. Referenced memory will
 * be initialized to 0.
 *
 * @param {number} [size]
 * @returns {Uint8Array}
 */
export function alloc(size?: number | undefined): Uint8Array;
/**
 * Where possible returns a Uint8Array of the requested size that references
 * uninitialized memory. Only use if you are certain you will immediately
 * overwrite every value in the returned `Uint8Array`.
 *
 * @param {number} [size]
 * @returns {Uint8Array}
 */
export function allocUnsafe(size?: number | undefined): Uint8Array;
//# sourceMappingURL=alloc.d.ts.map