export type UserBucketOptions = {
    hashFn: (value: Uint8Array) => Promise<Uint8Array>;
    bits?: number | undefined;
};
/**
 * @typedef {object} UserBucketOptions
 * @property {(value: Uint8Array) => Promise<Uint8Array>} hashFn
 * @property {number} [bits=8]
 */
/**
 * @param {UserBucketOptions} options
 */
export function createHAMT(options: UserBucketOptions): Bucket<any>;
import Bucket = require("./bucket");
export { Bucket };
//# sourceMappingURL=index.d.ts.map