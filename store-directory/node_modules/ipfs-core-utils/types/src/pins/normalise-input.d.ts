/**
 * @typedef {Object} Pinnable
 * @property {string | InstanceType<typeof window.String> | CID} [path]
 * @property {CID} [cid]
 * @property {boolean} [recursive]
 * @property {any} [metadata]
 *
 * @typedef {CID|string|InstanceType<typeof window.String>|Pinnable} ToPin
 * @typedef {ToPin|Iterable<ToPin>|AsyncIterable<ToPin>} Source
 *
 * @typedef {Object} Pin
 * @property {string|CID} path
 * @property {boolean} recursive
 * @property {any} [metadata]
 */
/**
 * Transform one of:
 *
 * ```ts
 * CID
 * String
 * { cid: CID recursive, metadata }
 * { path: String recursive, metadata }
 * Iterable<CID>
 * Iterable<String>
 * Iterable<{ cid: CID recursive, metadata }>
 * Iterable<{ path: String recursive, metadata }>
 * AsyncIterable<CID>
 * AsyncIterable<String>
 * AsyncIterable<{ cid: CID recursive, metadata }>
 * AsyncIterable<{ path: String recursive, metadata }>
 * ```
 * Into:
 *
 * ```ts
 * AsyncIterable<{ path: CID|String, recursive:boolean, metadata }>
 * ```
 *
 * @param {Source} input
 * @returns {AsyncIterable<Pin>}
 */
export function normaliseInput(input: Source): AsyncIterable<Pin>;
export type Pinnable = {
    path?: string | String | CID | undefined;
    cid?: CID | undefined;
    recursive?: boolean | undefined;
    metadata?: any;
};
export type ToPin = CID | string | InstanceType<typeof window.String> | Pinnable;
export type Source = ToPin | Iterable<ToPin> | AsyncIterable<ToPin>;
export type Pin = {
    path: string | CID;
    recursive: boolean;
    metadata?: any;
};
import { CID } from "multiformats/cid";
//# sourceMappingURL=normalise-input.d.ts.map