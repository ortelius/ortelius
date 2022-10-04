/**
 * @typedef {import('interface-blockstore').Pair} Pair
 * @typedef {import('interface-blockstore').Blockstore} Blockstore
 * @typedef {import('interface-store').Options} Options
 */
/**
 * @class MemoryBlockstore
 * @implements {Blockstore}
 */
export class MemoryBlockstore extends BaseBlockstore implements Blockstore {
    /** @type {Record<string, Uint8Array>} */
    data: Record<string, Uint8Array>;
}
export type Pair = import('interface-blockstore').Pair;
export type Blockstore = import('interface-blockstore').Blockstore;
export type Options = import('interface-store').Options;
import { BaseBlockstore } from "./base.js";
//# sourceMappingURL=memory.d.ts.map