import { CID } from 'multiformats';
import { BaseBlockstore } from 'blockstore-core';
import { Blockstore } from './index';
export declare class MemoryBlockStore extends BaseBlockstore implements Blockstore {
    store: Map<string, Uint8Array>;
    constructor();
    blocks(): AsyncGenerator<{
        cid: CID;
        bytes: Uint8Array;
    }, void, unknown>;
    put(cid: CID, bytes: Uint8Array): Promise<void>;
    get(cid: CID): Promise<Uint8Array>;
    has(cid: CID): Promise<boolean>;
    close(): Promise<void>;
}
