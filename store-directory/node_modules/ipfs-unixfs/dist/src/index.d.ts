/**
 * @param {string | number | undefined} [mode]
 */
export function parseMode(mode?: string | number | undefined): number | undefined;
/**
 * @param {any} input
 */
export function parseMtime(input: any): import("./types").Mtime | undefined;
export type Mtime = import('./types').Mtime;
export type MtimeLike = import('./types').MtimeLike;
export class UnixFS {
    /**
     * Decode from protobuf https://github.com/ipfs/specs/blob/master/UNIXFS.md
     *
     * @param {Uint8Array} marshaled
     */
    static unmarshal(marshaled: Uint8Array): UnixFS;
    /**
     * @param {object} [options]
     * @param {string} [options.type='file']
     * @param {Uint8Array} [options.data]
     * @param {number[]} [options.blockSizes]
     * @param {number} [options.hashType]
     * @param {number} [options.fanout]
     * @param {MtimeLike | null} [options.mtime]
     * @param {number | string} [options.mode]
     */
    constructor(options?: {
        type?: string | undefined;
        data?: Uint8Array | undefined;
        blockSizes?: number[] | undefined;
        hashType?: number | undefined;
        fanout?: number | undefined;
        mtime?: import("./types").MtimeLike | null | undefined;
        mode?: string | number | undefined;
    } | undefined);
    type: string;
    data: Uint8Array | undefined;
    hashType: number | undefined;
    fanout: number | undefined;
    /** @type {number[]} */
    blockSizes: number[];
    _originalMode: number;
    /**
     * @param {number | undefined} mode
     */
    set mode(arg: number | undefined);
    /**
     * @returns {number | undefined}
     */
    get mode(): number | undefined;
    mtime: import("./types").Mtime | undefined;
    _mode: number | undefined;
    isDirectory(): boolean;
    /**
     * @param {number} size
     */
    addBlockSize(size: number): void;
    /**
     * @param {number} index
     */
    removeBlockSize(index: number): void;
    /**
     * Returns `0` for directories or `data.length + sum(blockSizes)` for everything else
     */
    fileSize(): number;
    /**
     * encode to protobuf Uint8Array
     */
    marshal(): Uint8Array;
}
//# sourceMappingURL=index.d.ts.map