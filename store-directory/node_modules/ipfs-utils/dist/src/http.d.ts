/// <reference types="node" />
export = HTTP;
declare class HTTP {
    /**
     *
     * @param {HTTPOptions} options
     */
    constructor(options?: HTTPOptions);
    /** @type {HTTPOptions} */
    opts: HTTPOptions;
    /**
     * Fetch
     *
     * @param {string | Request} resource
     * @param {HTTPOptions} options
     * @returns {Promise<ExtendedResponse>}
     */
    fetch(resource: string | Request, options?: HTTPOptions): Promise<ExtendedResponse>;
    /**
     * @param {string | Request} resource
     * @param {HTTPOptions} options
     */
    post(resource: string | Request, options?: HTTPOptions): Promise<import("./types").ExtendedResponse>;
    /**
     * @param {string | Request} resource
     * @param {HTTPOptions} options
     */
    get(resource: string | Request, options?: HTTPOptions): Promise<import("./types").ExtendedResponse>;
    /**
     * @param {string | Request} resource
     * @param {HTTPOptions} options
     */
    put(resource: string | Request, options?: HTTPOptions): Promise<import("./types").ExtendedResponse>;
    /**
     * @param {string | Request} resource
     * @param {HTTPOptions} options
     */
    delete(resource: string | Request, options?: HTTPOptions): Promise<import("./types").ExtendedResponse>;
    /**
     * @param {string | Request} resource
     * @param {HTTPOptions} options
     */
    options(resource: string | Request, options?: HTTPOptions): Promise<import("./types").ExtendedResponse>;
}
declare namespace HTTP {
    export { HTTPError, TimeoutError, fromStream as streamToAsyncIterator, post, get, put, _delete as delete, options, NodeReadableStream, HTTPOptions, ExtendedResponse };
}
type HTTPOptions = import('./types').HTTPOptions;
import { Request } from "./http/fetch";
type ExtendedResponse = import('./types').ExtendedResponse;
import { HTTPError } from "./http/error";
import { TimeoutError } from "./http/error";
/**
 * Stream to AsyncIterable
 *
 * @template TChunk
 * @param {ReadableStream<TChunk> | NodeReadableStream | null} source
 * @returns {AsyncIterable<TChunk>}
 */
declare function fromStream<TChunk>(source: import("stream").Readable | ReadableStream<TChunk> | null): AsyncIterable<TChunk>;
/**
 * @param {string | Request} resource
 * @param {HTTPOptions} [options]
 */
declare function post(resource: string | Request, options?: import("./types").HTTPOptions | undefined): Promise<import("./types").ExtendedResponse>;
/**
 * @param {string | Request} resource
 * @param {HTTPOptions} [options]
 */
declare function get(resource: string | Request, options?: import("./types").HTTPOptions | undefined): Promise<import("./types").ExtendedResponse>;
/**
 * @param {string | Request} resource
 * @param {HTTPOptions} [options]
 */
declare function put(resource: string | Request, options?: import("./types").HTTPOptions | undefined): Promise<import("./types").ExtendedResponse>;
/**
 * @param {string | Request} resource
 * @param {HTTPOptions} [options]
 */
declare function options(resource: string | Request, options?: import("./types").HTTPOptions | undefined): Promise<import("./types").ExtendedResponse>;
type NodeReadableStream = import('stream').Readable;
//# sourceMappingURL=http.d.ts.map