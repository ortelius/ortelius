export default persist;
/**
 * @param {Uint8Array} buffer
 * @param {import('interface-blockstore').Blockstore} blockstore
 * @param {import('../types').PersistOptions} options
 */
declare function persist(buffer: Uint8Array, blockstore: import('interface-blockstore').Blockstore, options: import('../types').PersistOptions): Promise<CID>;
import { CID } from "multiformats/cid";
//# sourceMappingURL=persist.d.ts.map