
export type AwaitIterable<T> = Iterable<T> | AsyncIterable<T>
export type Await<T> = Promise<T> | T

export interface Pair<Key, Value> {
  key: Key
  value: Value
}

/**
 * Options for async operations.
 */
export interface Options {
  signal?: AbortSignal
}

export interface Batch<Key, Value> {
  put: (key: Key, value: Value) => void
  delete: (key: Key) => void
  commit: (options?: Options) => Promise<void>
}

export interface Store<Key, Value> {
  open: () => Promise<void>
  close: () => Promise<void>

  /**
   * Store the passed value under the passed key
   *
   * @example
   *
   * ```js
   * await store.put([{ key: new Key('awesome'), value: new Uint8Array([0, 1, 2, 3]) }])
   * ```
   */
  put: (key: Key, val: Value, options?: Options) => Promise<void>

  /**
   * Retrieve the value stored under the given key
   *
   * @example
   * ```js
   * const value = await store.get(new Key('awesome'))
   * console.log('got content: %s', value.toString('utf8'))
   * // => got content: datastore
   * ```
   */
  get: (key: Key, options?: Options) => Promise<Value>

  /**
   * Check for the existence of a value for the passed key
   *
   * @example
   * ```js
   *const exists = await store.has(new Key('awesome'))
   *
   *if (exists) {
   *  console.log('it is there')
   *} else {
   *  console.log('it is not there')
   *}
   *```
   */
  has: (key: Key, options?: Options) => Promise<boolean>

  /**
   * Remove the record for the passed key
   *
   * @example
   *
   * ```js
   * await store.delete(new Key('awesome'))
   * console.log('deleted awesome content :(')
   * ```
   */
  delete: (key: Key, options?: Options) => Promise<void>

  /**
   * Store the given key/value pairs
   *
   * @example
   * ```js
   * const source = [{ key: new Key('awesome'), value: new Uint8Array([0, 1, 2, 3]) }]
   *
   * for await (const { key, value } of store.putMany(source)) {
   *   console.info(`put content for key ${key}`)
   * }
   * ```
   */
  putMany: (
    source: AwaitIterable<Pair<Key, Value>>,
    options?: Options
  ) => AsyncIterable<Pair<Key, Value>>

  /**
   * Retrieve values for the passed keys
   *
   * @example
   * ```js
   * for await (const value of store.getMany([new Key('awesome')])) {
   *   console.log('got content:', new TextDecoder('utf8').decode(value))
   *   // => got content: datastore
   * }
   * ```
   */
  getMany: (
    source: AwaitIterable<Key>,
    options?: Options
  ) => AsyncIterable<Value>

  /**
   * Remove values for the passed keys
   *
   * @example
   *
   * ```js
   * const source = [new Key('awesome')]
   *
   * for await (const key of store.deleteMany(source)) {
   *   console.log(`deleted content with key ${key}`)
   * }
   * ```
   */
  deleteMany: (
    source: AwaitIterable<Key>,
    options?: Options
  ) => AsyncIterable<Key>

  /**
   * This will return an object with which you can chain multiple operations together, with them only being executed on calling `commit`.
   *
   * @example
   * ```js
   * const b = store.batch()
   *
   * for (let i = 0; i < 100; i++) {
   *   b.put(new Key(`hello${i}`), new TextEncoder('utf8').encode(`hello world ${i}`))
   * }
   *
   * await b.commit()
   * console.log('put 100 values')
   * ```
   */
  batch: () => Batch<Key, Value>

  /**
   * Query the store.
   *
   * @example
   * ```js
   * // retrieve __all__ key/value pairs from the store
   * let list = []
   * for await (const { key, value } of store.query({})) {
   *   list.push(value)
   * }
   * console.log('ALL THE VALUES', list)
   * ```
   */
  query: (query: Query<Key, Value>, options?: Options) => AsyncIterable<Pair<Key, Value>>

  /**
   * Query the store.
   *
   * @example
   * ```js
   * // retrieve __all__ keys from the store
   * let list = []
   * for await (const key of store.queryKeys({})) {
   *   list.push(key)
   * }
   * console.log('ALL THE KEYS', key)
   * ```
   */
  queryKeys: (query: KeyQuery<Key>, options?: Options) => AsyncIterable<Key>
}

export interface QueryFilter<Key, Value> { (item: Pair<Key, Value>): boolean }
export interface QueryOrder<Key, Value> { (a: Pair<Key, Value>, b: Pair<Key, Value>): -1 | 0 | 1 }

export interface Query<Key, Value> {
  prefix?: string
  filters?: Array<QueryFilter<Key, Value>>
  orders?: Array<QueryOrder<Key, Value>>
  limit?: number
  offset?: number
}

export interface KeyQueryFilter<Key> { (item: Key): boolean }
export interface KeyQueryOrder<Key> { (a: Key, b: Key): -1 | 0 | 1 }

export interface KeyQuery<Key> {
  prefix?: string
  filters?: Array<KeyQueryFilter<Key>>
  orders?: Array<KeyQueryOrder<Key>>
  limit?: number
  offset?: number
}
