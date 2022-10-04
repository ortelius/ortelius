"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.VerifyingGetOnlyBlockStore = void 0;
const equals_1 = require("uint8arrays/equals");
const sha2_1 = require("multiformats/hashes/sha2");
const blockstore_core_1 = require("blockstore-core");
class VerifyingGetOnlyBlockStore extends blockstore_core_1.BaseBlockstore {
    constructor(blockstore) {
        super();
        this.store = blockstore;
    }
    async get(cid) {
        const res = await this.store.get(cid);
        if (!res) {
            throw new Error(`Incomplete CAR. Block missing for CID ${cid}`);
        }
        if (!isValid({ cid, bytes: res })) {
            throw new Error(`Invalid CAR. Hash of block data does not match CID ${cid}`);
        }
        return res;
    }
    static fromBlockstore(b) {
        return new VerifyingGetOnlyBlockStore(b);
    }
    static fromCarReader(cr) {
        return new VerifyingGetOnlyBlockStore({
            // Return bytes in the same fashion as a Blockstore implementation
            get: async (cid) => {
                const block = await cr.get(cid);
                return block === null || block === void 0 ? void 0 : block.bytes;
            }
        });
    }
}
exports.VerifyingGetOnlyBlockStore = VerifyingGetOnlyBlockStore;
async function isValid({ cid, bytes }) {
    const hash = await sha2_1.sha256.digest(bytes);
    return (0, equals_1.equals)(hash.digest, cid.multihash.digest);
}
