export class Multihashes {
    /**
     * @param {object} options
     * @param {LoadHasherFn} [options.loadHasher]
     * @param {MultihashHasher[]} options.hashers
     */
    constructor(options: {
        loadHasher?: import("./types").LoadHasherFn | undefined;
        hashers: MultihashHasher[];
    });
    /** @type {Record<string, MultihashHasher>}} */
    _hashersByName: Record<string, MultihashHasher>;
    /** @type {Record<number, MultihashHasher>}} */
    _hashersByCode: Record<number, MultihashHasher>;
    _loadHasher: import("./types").LoadHasherFn;
    /**
     * Add support for a multibase hasher
     *
     * @param {MultihashHasher} hasher
     */
    addHasher(hasher: MultihashHasher): void;
    /**
     * Remove support for a multibase hasher
     *
     * @param {MultihashHasher} hasher
     */
    removeHasher(hasher: MultihashHasher): void;
    /**
     * @param {number | string} code
     */
    getHasher(code: number | string): Promise<import("multiformats/hashes/interface").MultihashHasher>;
    listHashers(): import("multiformats/hashes/interface").MultihashHasher[];
}
export type MultihashHasher = import('multiformats/hashes/interface').MultihashHasher;
export type LoadHasherFn = import('./types').LoadHasherFn;
export type AbortOptions = import('ipfs-core-types/src/utils').AbortOptions;
//# sourceMappingURL=multihashes.d.ts.map