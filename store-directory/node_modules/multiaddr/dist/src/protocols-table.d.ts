export = Protocols;
/** @typedef {import("./types").Protocol} Protocol */
/**
 * Protocols
 *
 * @param {number | string} proto
 * @returns {Protocol}
 */
declare function Protocols(proto: number | string): Protocol;
declare namespace Protocols {
    export { V as lengthPrefixedVarSize, V, table, names, codes, p as object, Protocol };
}
type Protocol = import("./types").Protocol;
declare const V: -1;
declare var table: Array<[number, number, string, (string | boolean)?, string?]>;
declare var names: Record<string, Protocol>;
declare var codes: Record<number, Protocol>;
/**
 *
 * Create a protocol
 *
 * @param {number} code
 * @param {number} size
 * @param {string} name
 * @param {any} [resolvable]
 * @param {any} [path]
 * @returns {Protocol}
 */
declare function p(code: number, size: number, name: string, resolvable?: any, path?: any): Protocol;
//# sourceMappingURL=protocols-table.d.ts.map