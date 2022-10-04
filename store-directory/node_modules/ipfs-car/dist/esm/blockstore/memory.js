import { CID } from 'multiformats';
import { BaseBlockstore } from 'blockstore-core';
export class MemoryBlockStore extends BaseBlockstore {
    constructor() {
        super();
        this.store = new Map();
    }
    async *blocks() {
        for (const [cidStr, bytes] of this.store.entries()) {
            yield { cid: CID.parse(cidStr), bytes };
        }
    }
    put(cid, bytes) {
        this.store.set(cid.toString(), bytes);
        return Promise.resolve();
    }
    get(cid) {
        const bytes = this.store.get(cid.toString());
        if (!bytes) {
            throw new Error(`block with cid ${cid.toString()} no found`);
        }
        return Promise.resolve(bytes);
    }
    has(cid) {
        return Promise.resolve(this.store.has(cid.toString()));
    }
    close() {
        this.store.clear();
        return Promise.resolve();
    }
}
