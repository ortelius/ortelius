export = batch;
/**
 * Takes an (async) iterable that emits things and returns an async iterable that
 * emits those things in fixed-sized batches.
 *
 * @template T
 * @param {AsyncIterable<T>|Iterable<T>} source
 * @param {number} [size=1]
 * @returns {AsyncIterable<T[]>}
 */
declare function batch<T>(source: AsyncIterable<T> | Iterable<T>, size?: number | undefined): AsyncIterable<T[]>;
//# sourceMappingURL=index.d.ts.map