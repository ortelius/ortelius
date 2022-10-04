/**
 * @param {any} obj
 * @returns {obj is ArrayBufferView|ArrayBuffer}
 */
export function isBytes(obj: any): obj is ArrayBufferView | ArrayBuffer;
/**
 * @param {any} obj
 * @returns {obj is globalThis.Blob}
 */
export function isBlob(obj: any): obj is Blob;
/**
 * An object with a path or content property
 *
 * @param {any} obj
 * @returns {obj is import('ipfs-core-types/src/utils').ImportCandidate}
 */
export function isFileObject(obj: any): obj is import("ipfs-core-types/src/utils").ImportCandidate;
export function isReadableStream(value: any): value is ReadableStream<any>;
//# sourceMappingURL=utils.d.ts.map