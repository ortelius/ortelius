export default BASES;
export type MultibaseCodec = import('multiformats/bases/interface').MultibaseCodec<any>;
export type SupportedEncodings = 'utf8' | 'utf-8' | 'hex' | 'latin1' | 'ascii' | 'binary' | keyof {
    base256emoji: import("multiformats/bases/base").Codec<"base256emoji", "🚀">;
    base64: import("multiformats/bases/base").Codec<"base64", "m">;
    base64pad: import("multiformats/bases/base").Codec<"base64pad", "M">;
    base64url: import("multiformats/bases/base").Codec<"base64url", "u">;
    base64urlpad: import("multiformats/bases/base").Codec<"base64urlpad", "U">;
    base58btc: import("multiformats/bases/base").Codec<"base58btc", "z">;
    base58flickr: import("multiformats/bases/base").Codec<"base58flickr", "Z">;
    base36: import("multiformats/bases/base").Codec<"base36", "k">;
    base36upper: import("multiformats/bases/base").Codec<"base36upper", "K">;
    base32: import("multiformats/bases/base").Codec<"base32", "b">;
    base32upper: import("multiformats/bases/base").Codec<"base32upper", "B">;
    base32pad: import("multiformats/bases/base").Codec<"base32pad", "c">;
    base32padupper: import("multiformats/bases/base").Codec<"base32padupper", "C">;
    base32hex: import("multiformats/bases/base").Codec<"base32hex", "v">;
    base32hexupper: import("multiformats/bases/base").Codec<"base32hexupper", "V">;
    base32hexpad: import("multiformats/bases/base").Codec<"base32hexpad", "t">;
    base32hexpadupper: import("multiformats/bases/base").Codec<"base32hexpadupper", "T">;
    base32z: import("multiformats/bases/base").Codec<"base32z", "h">;
    base16: import("multiformats/bases/base").Codec<"base16", "f">;
    base16upper: import("multiformats/bases/base").Codec<"base16upper", "F">;
    base10: import("multiformats/bases/base").Codec<"base10", "9">;
    base8: import("multiformats/bases/base").Codec<"base8", "7">;
    base2: import("multiformats/bases/base").Codec<"base2", "0">;
    identity: import("multiformats/bases/base").Codec<"identity", "\0">;
};
/**
 * @typedef {'utf8' | 'utf-8' | 'hex' | 'latin1' | 'ascii' | 'binary' | keyof bases } SupportedEncodings
 */
/**
 * @type {Record<SupportedEncodings, MultibaseCodec>}
 */
declare const BASES: Record<SupportedEncodings, import("multiformats/bases/interface").MultibaseCodec<any>>;
//# sourceMappingURL=bases.d.ts.map