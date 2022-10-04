declare const _exports: {
    default: typeof fetch;
    Headers: {
        new (init?: globalThis.Headers | string[][] | Record<string, string> | undefined): globalThis.Headers;
        prototype: globalThis.Headers;
    };
    Request: {
        new (input: RequestInfo, init?: RequestInit | undefined): globalThis.Request;
        prototype: globalThis.Request;
    };
    Response: {
        new (body?: string | ReadableStream<Uint8Array> | URLSearchParams | Blob | ArrayBufferView | ArrayBuffer | FormData | null | undefined, init?: ResponseInit | undefined): globalThis.Response;
        prototype: globalThis.Response;
        error(): globalThis.Response;
        redirect(url: string, status?: number | undefined): globalThis.Response;
    };
} | {
    default: any;
    Headers: any;
    Request: any;
    Response: any;
};
export = _exports;
export type Headers = globalThis.Headers;
export type Request = globalThis.Request;
export type Response = globalThis.Response;
//# sourceMappingURL=fetch.browser.d.ts.map