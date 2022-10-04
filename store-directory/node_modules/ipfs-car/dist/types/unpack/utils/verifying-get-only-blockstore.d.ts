import { CID } from 'multiformats';
import { CarReader } from '@ipld/car/api';
import { BaseBlockstore } from 'blockstore-core';
import { Blockstore } from '../../blockstore/index';
declare type verifyingBlockStore = {
    get: (cid: CID) => Promise<Uint8Array | undefined>;
};
export declare class VerifyingGetOnlyBlockStore extends BaseBlockstore {
    store: verifyingBlockStore;
    constructor(blockstore: verifyingBlockStore);
    get(cid: CID): Promise<Uint8Array>;
    static fromBlockstore(b: Blockstore): VerifyingGetOnlyBlockStore;
    static fromCarReader(cr: CarReader): VerifyingGetOnlyBlockStore;
}
export {};
