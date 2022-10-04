export = fetch;
/** @type {fetchImpl} */
declare const fetch: fetchImpl;
declare namespace fetch {
    export { fetchImpl };
}
type fetchImpl = {
    fetch: typeof globalThis.fetch;
    Request: globalThis.Request;
    Response: globalThis.Response;
    Headers: globalThis.Headers;
};
//# sourceMappingURL=fetch.d.ts.map