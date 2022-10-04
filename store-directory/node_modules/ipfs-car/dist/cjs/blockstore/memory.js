"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.MemoryBlockStore = void 0;
const multiformats_1 = require("multiformats");
const blockstore_core_1 = require("blockstore-core");
class MemoryBlockStore extends blockstore_core_1.BaseBlockstore {
    constructor() {
        super();
        this.store = new Map();
    }
    async *blocks() {
        for (const [cidStr, bytes] of this.store.entries()) {
            yield { cid: multiformats_1.CID.parse(cidStr), bytes };
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
exports.MemoryBlockStore = MemoryBlockStore;
