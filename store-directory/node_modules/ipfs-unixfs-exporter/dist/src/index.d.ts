/**
 * @param {string | CID} path
 * @param {Blockstore} blockstore
 * @param {ExporterOptions} [options]
 */
export function walkPath(path: string | CID, blockstore: Blockstore, options?: import("./types").ExporterOptions | undefined): AsyncGenerator<import("./types").UnixFSEntry, void, unknown>;
/**
 * @param {string | CID} path
 * @param {Blockstore} blockstore
 * @param {ExporterOptions} [options]
 */
export function exporter(path: string | CID, blockstore: Blockstore, options?: import("./types").ExporterOptions | undefined): Promise<import("./types").UnixFSEntry>;
/**
 * @param {string | CID} path
 * @param {Blockstore} blockstore
 * @param {ExporterOptions} [options]
 */
export function recursive(path: string | CID, blockstore: Blockstore, options?: import("./types").ExporterOptions | undefined): AsyncGenerator<import("./types").UnixFSEntry, void, unknown>;
export type UnixFS = import('ipfs-unixfs').UnixFS;
export type Blockstore = import('interface-blockstore').Blockstore;
export type ExporterOptions = import('./types').ExporterOptions;
export type UnixFSFile = import('./types').UnixFSFile;
export type UnixFSDirectory = import('./types').UnixFSDirectory;
export type ObjectNode = import('./types').ObjectNode;
export type RawNode = import('./types').RawNode;
export type IdentityNode = import('./types').IdentityNode;
export type UnixFSEntry = import('./types').UnixFSEntry;
import { CID } from "multiformats/cid";
//# sourceMappingURL=index.d.ts.map