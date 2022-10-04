/**
 * @typedef {import('interface-blockstore').Options} Options
 * @typedef {import('interface-blockstore').Pair} Pair
 * @typedef {import('interface-blockstore').Blockstore} Blockstore
 * @typedef {import('interface-blockstore').Query} Query
 * @typedef {import('interface-blockstore').KeyQuery} KeyQuery
 * @typedef {import('interface-blockstore').Batch} Batch
 *
 * @typedef {import('multiformats').CID} CID
 */
/**
 * @template O
 * @typedef {import('interface-store').AwaitIterable<O>} AwaitIterable
 */
/**
 * @implements {Blockstore}
 */
export class BaseBlockstore implements Blockstore {
    /**
     * @returns {Promise<void>}
     */
    open(): Promise<void>;
    /**
     * @returns {Promise<void>}
     */
    close(): Promise<void>;
    /**
     * @param {CID} key
     * @param {Uint8Array} val
     * @param {Options} [options]
     * @returns {Promise<void>}
     */
    put(key: CID, val: Uint8Array, options?: import("interface-blockstore").Options | undefined): Promise<void>;
    /**
     * @param {CID} key
     * @param {Options} [options]
     * @returns {Promise<Uint8Array>}
     */
    get(key: CID, options?: import("interface-blockstore").Options | undefined): Promise<Uint8Array>;
    /**
     * @param {CID} key
     * @param {Options} [options]
     * @returns {Promise<boolean>}
     */
    has(key: CID, options?: import("interface-blockstore").Options | undefined): Promise<boolean>;
    /**
     * @param {CID} key
     * @param {Options} [options]
     * @returns {Promise<void>}
     */
    delete(key: CID, options?: import("interface-blockstore").Options | undefined): Promise<void>;
    /**
     * @param {AwaitIterable<Pair>} source
     * @param {Options} [options]
     * @returns {AsyncIterable<Pair>}
     */
    putMany(source: AwaitIterable<Pair>, options?: import("interface-blockstore").Options | undefined): AsyncIterable<Pair>;
    /**
     * @param {AwaitIterable<CID>} source
     * @param {Options} [options]
     * @returns {AsyncIterable<Uint8Array>}
     */
    getMany(source: AwaitIterable<CID>, options?: import("interface-blockstore").Options | undefined): AsyncIterable<Uint8Array>;
    /**
     * @param {AwaitIterable<CID>} source
     * @param {Options} [options]
     * @returns {AsyncIterable<CID>}
     */
    deleteMany(source: AwaitIterable<CID>, options?: import("interface-blockstore").Options | undefined): AsyncIterable<CID>;
    /**
     * @returns {Batch}
     */
    batch(): Batch;
    /**
     * Extending classes should override `query` or implement this method
     *
     * @param {Query} q
     * @param {Options} [options]
     * @returns {AsyncIterable<Pair>}
     */
    _all(q: Query, options?: import("interface-blockstore").Options | undefined): AsyncIterable<Pair>;
    /**
     * Extending classes should override `queryKeys` or implement this method
     *
     * @param {KeyQuery} q
     * @param {Options} [options]
     * @returns {AsyncIterable<CID>}
     */
    _allKeys(q: KeyQuery, options?: import("interface-blockstore").Options | undefined): AsyncIterable<CID>;
    /**
     * @param {Query} q
     * @param {Options} [options]
     */
    query(q: Query, options?: import("interface-blockstore").Options | undefined): AsyncIterable<import("interface-blockstore").Pair>;
    /**
     * @param {KeyQuery} q
     * @param {Options} [options]
     */
    queryKeys(q: KeyQuery, options?: import("interface-blockstore").Options | undefined): AsyncIterable<import("multiformats").CID>;
}
export type Options = import('interface-blockstore').Options;
export type Pair = import('interface-blockstore').Pair;
export type Blockstore = import('interface-blockstore').Blockstore;
export type Query = import('interface-blockstore').Query;
export type KeyQuery = import('interface-blockstore').KeyQuery;
export type Batch = import('interface-blockstore').Batch;
export type CID = import('multiformats').CID;
export type AwaitIterable<O> = import('interface-store').AwaitIterable<O>;
//# sourceMappingURL=base.d.ts.map