export default resolve;
export type Resolver = import('../types').Resolver;
export type Resolve = import('../types').Resolve;
/**
 * @type {Resolve}
 */
declare function resolve(cid: import("multiformats").CID, name: string, path: string, toResolve: string[], depth: number, blockstore: import("interface-blockstore").Blockstore, options: import("../types").ExporterOptions): Promise<import("../types").ResolveResult>;
//# sourceMappingURL=index.d.ts.map