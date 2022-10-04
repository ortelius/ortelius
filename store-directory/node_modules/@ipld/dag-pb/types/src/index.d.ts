/**
 * @param {PBNode} node
 * @returns {ByteView<PBNode>}
 */
export function encode(node: PBNode): ByteView<PBNode>;
/**
 * @param {ByteView<PBNode>} bytes
 * @returns {PBNode}
 */
export function decode(bytes: ByteView<PBNode>): PBNode;
/**
 * @template T
 * @typedef {import('multiformats/codecs/interface').ByteView<T>} ByteView
 */
/**
 * @typedef {import('./interface').PBLink} PBLink
 * @typedef {import('./interface').PBNode} PBNode
 */
export const name: "dag-pb";
export const code: 112;
export type ByteView<T> = import('multiformats/codecs/interface').ByteView<T>;
export type PBLink = import('./interface').PBLink;
export type PBNode = import('./interface').PBNode;
import { prepare } from "./util.js";
import { validate } from "./util.js";
import { createNode } from "./util.js";
import { createLink } from "./util.js";
export { prepare, validate, createNode, createLink };
//# sourceMappingURL=index.d.ts.map