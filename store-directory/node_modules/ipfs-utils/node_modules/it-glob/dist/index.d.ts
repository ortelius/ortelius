export = glob;
/**
 * @typedef {string} Glob
 * @typedef {object} OptionsExt
 * @property {string} [cwd=process.cwd()]
 * @property {boolean} [absolute=false] - If true produces absolute paths
 * @property {boolean} [nodir] - If true yields file paths and skip directories
 *
 * @typedef {OptionsExt & minimatch.IOptions} Options
 */
/**
 * Async iterable filename pattern matcher
 *
 * @param {string} dir
 * @param {string} pattern
 * @param {Options} [options]
 * @returns {AsyncIterable<string>}
 */
declare function glob(dir: string, pattern: string, options?: Options | undefined): AsyncIterable<string>;
declare namespace glob {
    export { Glob, OptionsExt, Options };
}
type Options = OptionsExt & minimatch.IOptions;
type Glob = string;
type OptionsExt = {
    cwd?: string | undefined;
    /**
     * - If true produces absolute paths
     */
    absolute?: boolean | undefined;
    /**
     * - If true yields file paths and skip directories
     */
    nodir?: boolean | undefined;
};
import minimatch = require("minimatch");
//# sourceMappingURL=index.d.ts.map