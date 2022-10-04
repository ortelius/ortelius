/**
 * @typedef FromPathOptions
 * @property {boolean} [hidden] - Include .dot files in matched paths
 * @property {Array<string>} [ignore] - Glob paths to ignore
 * @property {boolean} [followSymlinks] - follow symlinks
 * @property {boolean} [preserveMode] - preserve mode
 * @property {boolean} [preserveMtime] - preserve mtime
 * @property {number} [mode] - mode to use - if preserveMode is true this will be ignored
 * @property {import('ipfs-unixfs').MtimeLike} [mtime] - mtime to use - if preserveMtime is true this will be ignored
 * @property {string} [pathPrefix] - base path prefix that will get stripped out of the filenames yielded
 *
 * @typedef FileObject
 * @property {string} name
 * @property {() => fs.ReadStream} stream
 */
/**
 * Gets all the FileObjects that match requested file paths.
 *
 * @param {Iterable<string> | AsyncIterable<string> | string} paths - File system path(s) to glob from
 * @param {FromPathOptions} [options] - options
 * @returns {Promise<FileObject[]>}
 */
export function getFilesFromPath(paths: Iterable<string> | AsyncIterable<string> | string, options?: FromPathOptions | undefined): Promise<FileObject[]>;
/**
 * Create an async iterator that yields paths that match requested file paths.
 *
 * @param {Iterable<string> | AsyncIterable<string> | string} paths - File system path(s) to glob from
 * @param {FromPathOptions} [options] - options
 * @yields {FileObject}
 */
export function filesFromPath(paths: Iterable<string> | AsyncIterable<string> | string, options?: FromPathOptions | undefined): AsyncGenerator<{
    name: string;
    stream: () => fs.ReadStream;
    mode: any;
    mtime: any;
    size: any;
}, void, unknown>;
export type FromPathOptions = {
    /**
     * - Include .dot files in matched paths
     */
    hidden?: boolean | undefined;
    /**
     * - Glob paths to ignore
     */
    ignore?: string[] | undefined;
    /**
     * - follow symlinks
     */
    followSymlinks?: boolean | undefined;
    /**
     * - preserve mode
     */
    preserveMode?: boolean | undefined;
    /**
     * - preserve mtime
     */
    preserveMtime?: boolean | undefined;
    /**
     * - mode to use - if preserveMode is true this will be ignored
     */
    mode?: number | undefined;
    /**
     * - mtime to use - if preserveMtime is true this will be ignored
     */
    mtime?: import("ipfs-unixfs/types/src/types").MtimeLike | undefined;
    /**
     * - base path prefix that will get stripped out of the filenames yielded
     */
    pathPrefix?: string | undefined;
};
export type FileObject = {
    name: string;
    stream: () => fs.ReadStream;
};
import fs from "graceful-fs";
//# sourceMappingURL=index.d.ts.map