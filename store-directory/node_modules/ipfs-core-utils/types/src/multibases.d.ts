export class Multibases {
    /**
     * @param {object} options
     * @param {LoadBaseFn} [options.loadBase]
     * @param {MultibaseCodec[]} options.bases
     */
    constructor(options: {
        loadBase?: import("./types").LoadBaseFn | undefined;
        bases: import("multiformats/bases/interface").MultibaseCodec<any>[];
    });
    /** @type {Record<string, MultibaseCodec>}} */
    _basesByName: Record<string, import("multiformats/bases/interface").MultibaseCodec<any>>;
    /** @type {Record<string, MultibaseCodec>}} */
    _basesByPrefix: Record<string, import("multiformats/bases/interface").MultibaseCodec<any>>;
    _loadBase: import("./types").LoadBaseFn;
    /**
     * Add support for a multibase codec
     *
     * @param {MultibaseCodec} base
     */
    addBase(base: import("multiformats/bases/interface").MultibaseCodec<any>): void;
    /**
     * Remove support for a multibase codec
     *
     * @param {MultibaseCodec} base
     */
    removeBase(base: import("multiformats/bases/interface").MultibaseCodec<any>): void;
    /**
     * @param {string} nameOrPrefix
     */
    getBase(nameOrPrefix: string): Promise<import("multiformats/bases/interface").MultibaseCodec<any>>;
    listBases(): import("multiformats/bases/interface").MultibaseCodec<any>[];
}
export type MultibaseCodec = import('multiformats/bases/interface').MultibaseCodec<any>;
export type LoadBaseFn = import('./types').LoadBaseFn;
export type AbortOptions = import('ipfs-core-types/src/utils').AbortOptions;
//# sourceMappingURL=multibases.d.ts.map