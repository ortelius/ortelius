import type { CID } from 'multiformats/cid';
import type { UnixFS } from 'ipfs-unixfs';
import type { PBNode } from '@ipld/dag-pb';
import type { Blockstore } from 'interface-blockstore';
export interface ExporterOptions {
    offset?: number;
    length?: number;
    signal?: AbortSignal;
    timeout?: number;
}
export interface Exportable<T> {
    type: 'file' | 'directory' | 'object' | 'raw' | 'identity';
    name: string;
    path: string;
    cid: CID;
    depth: number;
    size: number;
    content: (options?: ExporterOptions) => AsyncIterable<T>;
}
export interface UnixFSFile extends Exportable<Uint8Array> {
    type: 'file';
    unixfs: UnixFS;
    node: PBNode;
}
export interface UnixFSDirectory extends Exportable<UnixFSEntry> {
    type: 'directory';
    unixfs: UnixFS;
    node: PBNode;
}
export interface ObjectNode extends Exportable<any> {
    type: 'object';
    node: Uint8Array;
}
export interface RawNode extends Exportable<Uint8Array> {
    type: 'raw';
    node: Uint8Array;
}
export interface IdentityNode extends Exportable<Uint8Array> {
    type: 'identity';
    node: Uint8Array;
}
export declare type UnixFSEntry = UnixFSFile | UnixFSDirectory | ObjectNode | RawNode | IdentityNode;
export interface NextResult {
    cid: CID;
    name: string;
    path: string;
    toResolve: string[];
}
export interface ResolveResult {
    entry: UnixFSEntry;
    next?: NextResult;
}
export interface Resolve {
    (cid: CID, name: string, path: string, toResolve: string[], depth: number, blockstore: Blockstore, options: ExporterOptions): Promise<ResolveResult>;
}
export interface Resolver {
    (cid: CID, name: string, path: string, toResolve: string[], resolve: Resolve, depth: number, blockstore: Blockstore, options: ExporterOptions): Promise<ResolveResult>;
}
export declare type UnixfsV1FileContent = AsyncIterable<Uint8Array> | Iterable<Uint8Array>;
export declare type UnixfsV1DirectoryContent = AsyncIterable<UnixFSEntry> | Iterable<UnixFSEntry>;
export declare type UnixfsV1Content = UnixfsV1FileContent | UnixfsV1DirectoryContent;
export interface UnixfsV1Resolver {
    (cid: CID, node: PBNode, unixfs: UnixFS, path: string, resolve: Resolve, depth: number, blockstore: Blockstore): (options: ExporterOptions) => UnixfsV1Content;
}
//# sourceMappingURL=types.d.ts.map