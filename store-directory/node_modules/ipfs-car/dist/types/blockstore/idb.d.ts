import { CID } from 'multiformats';
import { BaseBlockstore } from 'blockstore-core';
import { Blockstore } from './index';
/**
 * Save blocks to IndexedDB in the browser via idb-keyval
 * Creates a probably unique indexed db per instance to ensure that the
 * blocks iteration method only returns blocks from this invocation,
 * and so that the caller can destory it without affecting others.
 */
export declare class IdbBlockStore extends BaseBlockstore implements Blockstore {
    private store;
    constructor();
    blocks(): AsyncGenerator<{
        cid: CID;
        bytes: any;
    }, void, unknown>;
    put(cid: CID, bytes: Uint8Array): Promise<void>;
    get(cid: CID): Promise<Uint8Array>;
    has(cid: CID): Promise<boolean>;
    close(): Promise<void>;
}
