/**
 * @typedef {import('ipfs-core-types/src/utils').ImportCandidateStream} ImportCandidateStream
 */
/**
 * @param {ImportCandidateStream} source
 * @param {AbortController} abortController
 * @param {Headers|Record<string, string>} [headers]
 * @param {string} [boundary]
 */
export function multipartRequest(source: ImportCandidateStream, abortController: AbortController, headers?: Headers | Record<string, string> | undefined, boundary?: string | undefined): Promise<{
    parts: null;
    total: number;
    headers: any;
    body: any;
}>;
export type ImportCandidateStream = import('ipfs-core-types/src/utils').ImportCandidateStream;
//# sourceMappingURL=multipart-request.d.ts.map