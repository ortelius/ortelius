export = peekableIterator;
/**
 * @template T
 * @typedef {Object} Peek
 * @property {() => IteratorResult<T, void>} peek
 */
/**
 * @template T
 * @typedef {Object} AsyncPeek
 * @property {() => Promise<IteratorResult<T, void>>} peek
 */
/**
 * @template T
 * @typedef {Object} Push
 * @property {(value:T) => void} push
 */
/**
 * @template T
 * @typedef {Iterable<T> & Peek<T> & Push<T> & Iterator<T>} Peekable<T>
 */
/**
 * @template T
 * @typedef {AsyncIterable<T> & AsyncPeek<T> & Push<T> & AsyncIterator<T>} AsyncPeekable<T>
 */
/**
 * @template {Iterable<any> | AsyncIterable<any>} I
 * @param {I} iterable
 * @returns {I extends Iterable<infer T>
 *  ? Peekable<T>
 *  : I extends AsyncIterable<infer T>
 *  ? AsyncPeekable<T>
 *  : never
 * }
 */
declare function peekableIterator<I extends Iterable<any> | AsyncIterable<any>>(iterable: I): I extends Iterable<infer T> ? Peekable<T> : I extends AsyncIterable<infer T_1> ? AsyncPeekable<T_1> : never;
declare namespace peekableIterator {
    export { Peek, AsyncPeek, Push, Peekable, AsyncPeekable };
}
/**
 * <T>
 */
type Peekable<T> = Iterable<T> & Peek<T> & Push<T> & Iterator<T>;
/**
 * <T>
 */
type AsyncPeekable<T> = AsyncIterable<T> & AsyncPeek<T> & Push<T> & AsyncIterator<T>;
type Peek<T> = {
    peek: () => IteratorResult<T, void>;
};
type AsyncPeek<T> = {
    peek: () => Promise<IteratorResult<T, void>>;
};
type Push<T> = {
    push: (value: T) => void;
};
//# sourceMappingURL=index.d.ts.map