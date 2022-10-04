/**
 * @typedef {import('ipfs-core-types/src/utils').ImportCandidateStream} ImportCandidateStream
 */
/**
 * @param {ImportCandidateStream} source
 * @param {AbortController} abortController
 * @param {Headers|Record<string, string>} [headers]
 */
export function multipartRequest(source: ImportCandidateStream, abortController: AbortController, headers?: Headers | Record<string, string> | undefined): Promise<{
    total: number;
    parts: {
        name: string | undefined;
        start: number;
        end: number;
    }[];
    headers: Headers | Record<string, string>;
    body: FormData;
}>;
export type ImportCandidateStream = import('ipfs-core-types/src/utils').ImportCandidateStream;
//# sourceMappingURL=multipart-request.browser.d.ts.map