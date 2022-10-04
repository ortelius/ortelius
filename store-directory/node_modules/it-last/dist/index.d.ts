export = last;
/**
 * Returns the last item of an (async) iterable, unless empty, in which case
 * return `undefined`.
 *
 * @template T
 * @param {AsyncIterable<T>|Iterable<T>} source
 */
declare function last<T>(source: AsyncIterable<T> | Iterable<T>): Promise<T | undefined>;
//# sourceMappingURL=index.d.ts.map