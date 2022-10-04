import { equals } from 'uint8arrays/equals';
import { sha256 } from 'multiformats/hashes/sha2';
import { BaseBlockstore } from 'blockstore-core';
export class VerifyingGetOnlyBlockStore extends BaseBlockstore {
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
async function isValid({ cid, bytes }) {
    const hash = await sha256.digest(bytes);
    return equals(hash.digest, cid.multihash.digest);
}
