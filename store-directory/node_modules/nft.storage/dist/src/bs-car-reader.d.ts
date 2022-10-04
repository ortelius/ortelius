/**
 * An implementation of the CAR reader interface that is backed by a blockstore.
 *
 * @typedef {import('multiformats').CID} CID
 * @typedef {import('@ipld/car/api').CarReader} CarReader
 * @implements {CarReader}
 */
export class BlockstoreCarReader implements CarReader {
    /**
     * @param {number} version
     * @param {CID[]} roots
     * @param {import('ipfs-car/blockstore').Blockstore} blockstore
     */
    constructor(version: number, roots: CID[], blockstore: import('ipfs-car/blockstore').Blockstore);
    /**
     * @private
     */
    private _version;
    /**
     * @private
     */
    private _roots;
    /**
     * @private
     */
    private _blockstore;
    get version(): number;
    get blockstore(): import("ipfs-car/blockstore").Blockstore;
    getRoots(): Promise<import("multiformats").CID[]>;
    /**
     * @param {CID} cid
     */
    has(cid: CID): Promise<boolean>;
    /**
     * @param {CID} cid
     */
    get(cid: CID): Promise<{
        cid: import("multiformats").CID;
        bytes: Uint8Array;
    }>;
    blocks(): AsyncGenerator<import("@ipld/car/api").Block, void, unknown>;
    cids(): AsyncGenerator<import("multiformats").CID, void, unknown>;
}
/**
 * An implementation of the CAR reader interface that is backed by a blockstore.
 */
export type CID = import('multiformats').CID;
/**
 * An implementation of the CAR reader interface that is backed by a blockstore.
 */
export type CarReader = import('@ipld/car/api').CarReader;
//# sourceMappingURL=bs-car-reader.d.ts.map