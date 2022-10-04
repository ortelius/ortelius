export default Dir;
export type ImporterOptions = import('./types').ImporterOptions;
export type ImportResult = import('./types').ImportResult;
export type InProgressImportResult = import('./types').InProgressImportResult;
export type Blockstore = import('interface-blockstore').Blockstore;
export type CID = import('multiformats/cid').CID;
export type DirProps = {
    root: boolean;
    dir: boolean;
    path: string;
    dirty: boolean;
    flat: boolean;
    parent?: Dir | undefined;
    parentKey?: string | undefined;
    unixfs?: import("ipfs-unixfs").UnixFS | undefined;
    mode?: number | undefined;
    mtime?: import("ipfs-unixfs/types/src/types").Mtime | undefined;
};
/**
 * @typedef {import('./types').ImporterOptions} ImporterOptions
 * @typedef {import('./types').ImportResult} ImportResult
 * @typedef {import('./types').InProgressImportResult} InProgressImportResult
 * @typedef {import('interface-blockstore').Blockstore} Blockstore
 * @typedef {import('multiformats/cid').CID} CID
 *
 * @typedef {object} DirProps
 * @property {boolean} root
 * @property {boolean} dir
 * @property {string} path
 * @property {boolean} dirty
 * @property {boolean} flat
 * @property {Dir} [parent]
 * @property {string} [parentKey]
 * @property {import('ipfs-unixfs').UnixFS} [unixfs]
 * @property {number} [mode]
 * @property {import('ipfs-unixfs').Mtime} [mtime]
 */
declare class Dir {
    /**
     * @param {DirProps} props
     * @param {ImporterOptions} options
     */
    constructor(props: DirProps, options: ImporterOptions);
    options: import("./types").ImporterOptions;
    root: boolean;
    dir: boolean;
    path: string;
    dirty: boolean;
    flat: boolean;
    parent: Dir | undefined;
    parentKey: string | undefined;
    unixfs: import("ipfs-unixfs").UnixFS | undefined;
    mode: number | undefined;
    mtime: import("ipfs-unixfs/types/src/types").Mtime | undefined;
    /** @type {CID | undefined} */
    cid: CID | undefined;
    /** @type {number | undefined} */
    size: number | undefined;
    /**
     * @param {string} name
     * @param {InProgressImportResult | Dir} value
     */
    put(name: string, value: InProgressImportResult | Dir): Promise<void>;
    /**
     * @param {string} name
     * @returns {Promise<InProgressImportResult | Dir | undefined>}
     */
    get(name: string): Promise<InProgressImportResult | Dir | undefined>;
    /**
     * @returns {AsyncIterable<{ key: string, child: InProgressImportResult | Dir}>}
     */
    eachChildSeries(): AsyncIterable<{
        key: string;
        child: InProgressImportResult | Dir;
    }>;
    /**
     * @param {Blockstore} blockstore
     * @returns {AsyncIterable<ImportResult>}
     */
    flush(blockstore: Blockstore): AsyncIterable<ImportResult>;
}
//# sourceMappingURL=dir.d.ts.map