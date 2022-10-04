export default flatToShard;
export type Dir = import('./dir').default;
export type ImporterOptions = import('./types').ImporterOptions;
/**
 * @typedef {import('./dir').default} Dir
 * @typedef {import('./types').ImporterOptions} ImporterOptions
 */
/**
 * @param {Dir | null} child
 * @param {Dir} dir
 * @param {number} threshold
 * @param {ImporterOptions} options
 * @returns {Promise<DirSharded>}
 */
declare function flatToShard(child: Dir | null, dir: Dir, threshold: number, options: ImporterOptions): Promise<DirSharded>;
import DirSharded from "./dir-sharded.js";
//# sourceMappingURL=flat-to-shard.d.ts.map