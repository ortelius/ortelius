import type { Pair as StorePair, Batch as StoreBatch, QueryFilter as StoreQueryFilter, QueryOrder as StoreQueryOrder, Query as StoreQuery, KeyQueryFilter as StoreKeyQueryFilter, KeyQueryOrder as StoreKeyQueryOrder, KeyQuery as StoreKeyQuery, Options as StoreOptions, Store } from 'interface-store';
import type { Key } from './key';
export interface Options extends StoreOptions {
}
export interface Pair extends StorePair<Key, Uint8Array> {
}
export interface Batch extends StoreBatch<Key, Uint8Array> {
}
export interface Datastore extends Store<Key, Uint8Array> {
}
export interface QueryFilter extends StoreQueryFilter<Key, Uint8Array> {
}
export interface QueryOrder extends StoreQueryOrder<Key, Uint8Array> {
}
export interface Query extends StoreQuery<Key, Uint8Array> {
}
export interface KeyQueryFilter extends StoreKeyQueryFilter<Key> {
}
export interface KeyQueryOrder extends StoreKeyQueryOrder<Key> {
}
export interface KeyQuery extends StoreKeyQuery<Key> {
}
export { Key } from './key';
//# sourceMappingURL=index.d.ts.map