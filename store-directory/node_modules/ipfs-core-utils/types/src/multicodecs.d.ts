export class Multicodecs {
    /**
     * @param {object} options
     * @param {LoadCodecFn} [options.loadCodec]
     * @param {BlockCodec[]} options.codecs
     */
    constructor(options: {
        loadCodec?: import("./types").LoadCodecFn | undefined;
        codecs: import("multiformats/codecs/interface").BlockCodec<any, any>[];
    });
    /** @type {Record<string, BlockCodec>}} */
    _codecsByName: Record<string, import("multiformats/codecs/interface").BlockCodec<any, any>>;
    /** @type {Record<number, BlockCodec>}} */
    _codecsByCode: Record<number, import("multiformats/codecs/interface").BlockCodec<any, any>>;
    _loadCodec: import("./types").LoadCodecFn;
    /**
     * Add support for a block codec
     *
     * @param {BlockCodec} codec
     */
    addCodec(codec: import("multiformats/codecs/interface").BlockCodec<any, any>): void;
    /**
     * Remove support for a block codec
     *
     * @param {BlockCodec} codec
     */
    removeCodec(codec: import("multiformats/codecs/interface").BlockCodec<any, any>): void;
    /**
     * @param {number | string} code
     */
    getCodec(code: number | string): Promise<import("multiformats/codecs/interface").BlockCodec<any, any>>;
    listCodecs(): import("multiformats/codecs/interface").BlockCodec<any, any>[];
}
export type BlockCodec = import('multiformats/codecs/interface').BlockCodec<any, any>;
export type LoadCodecFn = import('./types').LoadCodecFn;
export type AbortOptions = import('ipfs-core-types/src/utils').AbortOptions;
//# sourceMappingURL=multicodecs.d.ts.map