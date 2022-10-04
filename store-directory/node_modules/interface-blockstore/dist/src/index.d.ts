import type { Pair as StorePair, Batch as StoreBatch, QueryFilter as StoreQueryFilter, QueryOrder as StoreQueryOrder, Query as StoreQuery, KeyQueryFilter as StoreKeyQueryFilter, KeyQueryOrder as StoreKeyQueryOrder, KeyQuery as StoreKeyQuery, Options as StoreOptions, Store } from 'interface-store';
import type { CID } from 'multiformats';
export interface Options extends StoreOptions {
}
export interface Pair extends StorePair<CID, Uint8Array> {
}
export interface Batch extends StoreBatch<CID, Uint8Array> {
}
export interface Blockstore extends Store<CID, Uint8Array> {
}
export interface QueryFilter extends StoreQueryFilter<CID, Uint8Array> {
}
export interface QueryOrder extends StoreQueryOrder<CID, Uint8Array> {
}
export interface Query extends StoreQuery<CID, Uint8Array> {
}
export interface KeyQueryFilter extends StoreKeyQueryFilter<CID> {
}
export interface KeyQueryOrder extends StoreKeyQueryOrder<CID> {
}
export interface KeyQuery extends StoreKeyQuery<CID> {
}
//# sourceMappingURL=index.d.ts.map