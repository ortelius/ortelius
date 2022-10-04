/**
 * @typedef {import('ipfs-core-types/src/utils').ImportCandidateStream} ImportCandidateStream
 * @typedef {import('ipfs-core-types/src/utils').ImportCandidate} ImportCandidate
 * @typedef {import('ipfs-core-types/src/utils').BrowserImportCandidate} BrowserImportCandidate
 */
/**
 * Transforms any of the `ipfs.add` input types into
 *
 * ```
 * AsyncIterable<{ path, mode, mtime, content: Blob }>
 * ```
 *
 * See https://github.com/ipfs/js-ipfs/blob/master/docs/core-api/FILES.md#ipfsadddata-options
 *
 * @param {ImportCandidate | ImportCandidateStream} input
 * @returns {AsyncGenerator<BrowserImportCandidate, void, undefined>}
 */
export function normaliseInput(input: ImportCandidate | ImportCandidateStream): AsyncGenerator<BrowserImportCandidate, void, undefined>;
export type ImportCandidateStream = import('ipfs-core-types/src/utils').ImportCandidateStream;
export type ImportCandidate = import('ipfs-core-types/src/utils').ImportCandidate;
export type BrowserImportCandidate = import('ipfs-core-types/src/utils').BrowserImportCandidate;
//# sourceMappingURL=normalise-input.browser.d.ts.map