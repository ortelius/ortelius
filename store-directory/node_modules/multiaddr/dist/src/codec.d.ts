/**
 * @param {string} str
 */
export function stringToStringTuples(str: string): string[][];
/**
 * @param {[number, string?][]} tuples
 */
export function stringTuplesToString(tuples: [number, string?][]): string;
/**
 * Convert tuples to string tuples
 *
 * [[int code, Uint8Array]... ] -> [[int code, str addr]... ]
 *
 * @param {Array<[number, Uint8Array?]>} tuples
 * @returns {Array<[number, string?]>}
 */
export function tuplesToStringTuples(tuples: Array<[number, Uint8Array?]>): Array<[number, string?]>;
/**
 * @param {Array<string[] | string >} tuples
 * @returns {[number , Uint8Array?][]}
 */
export function stringTuplesToTuples(tuples: Array<string[] | string>): [number, Uint8Array?][];
/**
 *
 * @param {Uint8Array} buf
 * @returns {Array<[number, Uint8Array?]>}
 */
export function bytesToTuples(buf: Uint8Array): Array<[number, Uint8Array?]>;
/**
 * @param {[number, Uint8Array?][]} tuples
 */
export function tuplesToBytes(tuples: [number, Uint8Array?][]): Uint8Array;
/**
 * @param {Uint8Array} buf
 */
export function bytesToString(buf: Uint8Array): string;
/**
 * @param {string} str
 */
export function stringToBytes(str: string): Uint8Array;
/**
 * @param {string} str
 */
export function fromString(str: string): Uint8Array;
/**
 * @param {Uint8Array} buf
 */
export function fromBytes(buf: Uint8Array): Uint8Array;
/**
 * @param {Uint8Array} buf
 */
export function validateBytes(buf: Uint8Array): any;
/**
 * @param {Uint8Array} buf
 */
export function isValidBytes(buf: Uint8Array): boolean;
/**
 * @param {string} str
 */
export function cleanPath(str: string): string;
/**
 * @param {string} str
 */
export function ParseError(str: string): Error;
/**
 * @param {any[]} tup
 */
export function protoFromTuple(tup: any[]): import("./types").Protocol;
/**
 * @param {import("./types").Protocol} p
 * @param {Uint8Array | number[]} addr
 */
export function sizeForAddr(p: import("./types").Protocol, addr: Uint8Array | number[]): number;
//# sourceMappingURL=codec.d.ts.map