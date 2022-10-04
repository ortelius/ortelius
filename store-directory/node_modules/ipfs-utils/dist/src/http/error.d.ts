export class TimeoutError extends Error {
}
export class AbortError extends Error {
}
export class HTTPError extends Error {
    /**
     * @param {Response} response
     */
    constructor(response: Response);
    response: Response;
}
//# sourceMappingURL=error.d.ts.map