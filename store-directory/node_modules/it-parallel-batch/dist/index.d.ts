export = parallelBatch;
/**
 * @template T
 * @typedef {{ok:true, value:T}} Success
 */
/**
 * @typedef {{ok:false, err:Error}} Failure
 */
/**
 * Takes an (async) iterator that emits promise-returning functions,
 * invokes them in parallel and emits the results as they become available but
 * in the same order as the input
 *
 * @template T
 * @param {AsyncIterable<() => Promise<T>>|Iterable<() => Promise<T>>} source
 * @param {number} [size=1]
 * @returns {AsyncIterable<T>}
 */
declare function parallelBatch<T>(source: AsyncIterable<() => Promise<T>> | Iterable<() => Promise<T>>, size?: number | undefined): AsyncIterable<T>;
declare namespace parallelBatch {
    export { Success, Failure };
}
type Success<T> = {
    ok: true;
    value: T;
};
type Failure = {
    ok: false;
    err: Error;
};
//# sourceMappingURL=index.d.ts.map