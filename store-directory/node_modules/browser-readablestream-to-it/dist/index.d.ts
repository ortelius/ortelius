export = browserReadableStreamToIt;
/**
 * Turns a browser readable stream into an async iterable. Async iteration over
 * returned iterable will lock give stream, preventing any other consumer from
 * acquiring a reader. The lock will be released if iteration loop is broken. To
 * prevent stream cancelling optional `{ preventCancel: true }` could be passed
 * as a second argument.
 * @template T
 * @param {ReadableStream<T>} stream
 * @param {Object} [options]
 * @param {boolean} [options.preventCancel=boolean]
 * @returns {AsyncIterable<T>}
 */
declare function browserReadableStreamToIt<T>(stream: ReadableStream<T>, options?: {
    preventCancel?: boolean | undefined;
} | undefined): AsyncIterable<T>;
//# sourceMappingURL=index.d.ts.map