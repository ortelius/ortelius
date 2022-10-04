/// <reference types="node" />
import { CID } from 'multiformats';
import { BaseBlockstore } from 'blockstore-core';
import { Blockstore } from './index';
export declare class FsBlockStore extends BaseBlockstore implements Blockstore {
    path: string;
    _opened: boolean;
    _opening?: Promise<void>;
    constructor();
    _open(): Promise<void>;
    put(cid: CID, bytes: Uint8Array): Promise<void>;
    get(cid: CID): Promise<Uint8Array>;
    has(cid: CID): Promise<boolean>;
    blocks(): AsyncGenerator<{
        cid: CID;
        bytes: Buffer;
    }, void, unknown>;
    close(): Promise<void>;
}
