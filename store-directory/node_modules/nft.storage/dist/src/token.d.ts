/**
 * @typedef {import('./gateway.js').GatewayURLOptions} EmbedOptions
 * @typedef {import('./lib/interface.js').TokenInput} TokenInput
 * @typedef {import('ipfs-car/blockstore').Blockstore} Blockstore
 */
/**
 * @template T
 * @typedef {import('./lib/interface.js').Encoded<T, [[Blob, URL]]>} EncodedBlobUrl
 */
/**
 * @template G
 * @typedef {import('./lib/interface.js').Encoded<G, [[Blob, Blob]]>} EncodedBlobBlob
 */
/**
 * @template {import('./lib/interface.js').TokenInput} T
 * @typedef {import('./lib/interface.js').Token<T>} TokenType
 */
/**
 * @template {TokenInput} T
 * @implements {TokenType<T>}
 */
export class Token<T extends import("./lib/interface.js").TokenInput> implements TokenType<T> {
    /**
     * @template {TokenInput} T
     * @param {{data: import('./lib/interface.js').Encoded<T, [[Blob, URL]]>}} token
     * @returns {import('./lib/interface.js').Encoded<T, [[Blob, URL]]>}
     */
    static embed<T_1 extends import("./lib/interface.js").TokenInput>({ data }: {
        data: import("./lib/interface.js").MatchRecord<T_1, (input: Blob) => URL>;
    }): import("./lib/interface.js").MatchRecord<T_1, (input: Blob) => URL>;
    /**
     * Takes token input, encodes it as a DAG, wraps it in a CAR and creates a new
     * Token instance from it. Where values are discovered `Blob` (or `File`)
     * objects in the given input, they are replaced with IPFS URLs (an `ipfs://`
     * prefixed CID with an optional path).
     *
     * @example
     * ```js
     * const cat = new File(['...'], 'cat.png')
     * const kitty = new File(['...'], 'kitty.png')
     * const { token, car } = await Token.encode({
     *   name: 'hello'
     *   image: cat
     *   properties: {
     *     extra: {
     *       image: kitty
     *     }
     *   }
     * })
     * ```
     *
     * @template {TokenInput} T
     * @param {T} input
     * @returns {Promise<{ cid: CID, token: TokenType<T>, car: import('./lib/interface.js').CarReader }>}
     */
    static encode<T_2 extends import("./lib/interface.js").TokenInput>(input: T_2): Promise<{
        cid: CID;
        token: import("./lib/interface.js").Token<T_2>;
        car: import('./lib/interface.js').CarReader;
    }>;
    /**
     * @param {import('./lib/interface.js').CIDString} ipnft
     * @param {import('./lib/interface.js').EncodedURL} url
     * @param {import('./lib/interface.js').Encoded<T, [[Blob, URL]]>} data
     */
    constructor(ipnft: import('./lib/interface.js').CIDString, url: import('./lib/interface.js').EncodedURL, data: import('./lib/interface.js').Encoded<T, [[Blob, URL]]>);
    /** @readonly */
    readonly ipnft: import("./lib/interface.js").CIDString;
    /** @readonly */
    readonly url: import("./lib/interface.js").EncodedURL;
    /** @readonly */
    readonly data: import("./lib/interface.js").MatchRecord<T, (input: Blob) => URL>;
    /**
     * @returns {import('./lib/interface.js').Encoded<T, [[Blob, URL]]>}
     */
    embed(): import('./lib/interface.js').Encoded<T, [[Blob, URL]]>;
}
export function embed<T>(input: import("./lib/interface.js").MatchRecord<T, (input: Blob) => URL>, options: EmbedOptions): import("./lib/interface.js").MatchRecord<T, (input: Blob) => URL>;
export function decode<T extends import("./lib/interface.js").TokenInput>({ ipnft, url, data }: import("./lib/interface.js").EncodedToken<T>, paths: Set<string>): Token<T>;
export function encode<T extends import("./lib/interface.js").TokenInput>(input: import("./lib/interface.js").MatchRecord<T, (input: Blob) => Blob>): FormData;
export function mapWith<T, I, X, O, State>(input: import("./lib/interface.js").MatchRecord<T, (input: I) => X>, p: (input: any, state: State, path: PropertyKey[]) => input is X, f: (state: State, input: X, path: PropertyKey[]) => [State, O], state: State): import("./lib/interface.js").MatchRecord<T, (input: I) => O>;
export type EmbedOptions = import('./gateway.js').GatewayURLOptions;
export type TokenInput = import('./lib/interface.js').TokenInput;
export type Blockstore = import('ipfs-car/blockstore').Blockstore;
export type EncodedBlobUrl<T> = import('./lib/interface.js').Encoded<T, [[Blob, URL]]>;
export type EncodedBlobBlob<G> = import('./lib/interface.js').Encoded<G, [[Blob, Blob]]>;
export type TokenType<T extends import("./lib/interface.js").TokenInput> = import('./lib/interface.js').Token<T>;
import { Blob } from "./platform.js";
import { CID } from "multiformats/cid";
import { FormData } from "./platform.js";
//# sourceMappingURL=token.d.ts.map