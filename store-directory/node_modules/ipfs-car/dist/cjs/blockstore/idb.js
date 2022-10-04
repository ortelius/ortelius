"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    Object.defineProperty(o, k2, { enumerable: true, get: function() { return m[k]; } });
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.prototype.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.IdbBlockStore = void 0;
const idb = __importStar(require("idb-keyval"));
const multiformats_1 = require("multiformats");
const blockstore_core_1 = require("blockstore-core");
/**
 * Save blocks to IndexedDB in the browser via idb-keyval
 * Creates a probably unique indexed db per instance to ensure that the
 * blocks iteration method only returns blocks from this invocation,
 * and so that the caller can destory it without affecting others.
 */
class IdbBlockStore extends blockstore_core_1.BaseBlockstore {
    constructor() {
        super();
        const dbName = `IdbBlockStore-${Date.now()}-${Math.random()}`;
        this.store = idb.createStore(dbName, `IdbBlockStore`);
    }
    async *blocks() {
        const keys = await idb.keys(this.store);
        for await (const key of keys) {
            yield {
                cid: multiformats_1.CID.parse(key.toString()),
                bytes: await idb.get(key, this.store)
            };
        }
    }
    async put(cid, bytes) {
        await idb.set(cid.toString(), bytes, this.store);
    }
    async get(cid) {
        const bytes = await idb.get(cid.toString(), this.store);
        if (!bytes) {
            throw new Error(`block with cid ${cid.toString()} no found`);
        }
        return bytes;
    }
    async has(cid) {
        const bytes = await idb.get(cid.toString(), this.store);
        return Boolean(bytes);
    }
    async close() {
        return idb.clear(this.store);
    }
}
exports.IdbBlockStore = IdbBlockStore;
