/**
 * @param {any} node
 * @returns {PBNode}
 */
export function prepare(node: any): PBNode;
/**
 * @param {PBNode} node
 */
export function validate(node: PBNode): void;
/**
 * @param {Uint8Array} data
 * @param {PBLink[]} [links=[]]
 * @returns {PBNode}
 */
export function createNode(data: Uint8Array, links?: import("./interface").PBLink[] | undefined): PBNode;
/**
 * @param {string} name
 * @param {number} size
 * @param {CID} cid
 * @returns {PBLink}
 */
export function createLink(name: string, size: number, cid: CID): PBLink;
export type PBLink = import('./interface').PBLink;
export type PBNode = import('./interface').PBNode;
import { CID } from "multiformats/cid";
//# sourceMappingURL=util.d.ts.map