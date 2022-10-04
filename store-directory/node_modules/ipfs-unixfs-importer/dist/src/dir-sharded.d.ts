export default DirSharded;
export type ImporterOptions = import('./types').ImporterOptions;
export type ImportResult = import('./types').ImportResult;
export type InProgressImportResult = import('./types').InProgressImportResult;
export type Blockstore = import('interface-blockstore').Blockstore;
export type DirProps = import('./dir').DirProps;
/**
 * @typedef {import('./types').ImporterOptions} ImporterOptions
 * @typedef {import('./types').ImportResult} ImportResult
 * @typedef {import('./types').InProgressImportResult} InProgressImportResult
 * @typedef {import('interface-blockstore').Blockstore} Blockstore
 */
/**
 * @typedef {import('./dir').DirProps} DirProps
 */
declare class DirSharded extends Dir {
    /** @type {Bucket<InProgressImportResult | Dir>} */
    _bucket: Bucket<InProgressImportResult | Dir>;
    childCount(): number;
    directChildrenCount(): number;
    onlyChild(): Bucket<import("./types").InProgressImportResult | Dir> | Bucket.BucketChild<import("./types").InProgressImportResult | Dir>;
}
import Dir from "./dir.js";
import { Bucket } from "hamt-sharding";
//# sourceMappingURL=dir-sharded.d.ts.map