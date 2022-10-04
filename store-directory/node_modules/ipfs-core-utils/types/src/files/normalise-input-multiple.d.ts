/**
 * @typedef {import('ipfs-core-types/src/utils').ImportCandidateStream} ImportCandidateStream
 */
/**
 * Transforms any of the `ipfs.addAll` input types into
 *
 * ```
 * AsyncIterable<{ path, mode, mtime, content: AsyncIterable<Uint8Array> }>
 * ```
 *
 * See https://github.com/ipfs/js-ipfs/blob/master/docs/core-api/FILES.md#ipfsadddata-options
 *
 * @param {ImportCandidateStream} input
 */
export function normaliseInput(input: ImportCandidateStream): AsyncGenerator<import("ipfs-unixfs-importer/types/src/types").ImportCandidate, void, undefined>;
export type ImportCandidateStream = import('ipfs-core-types/src/utils').ImportCandidateStream;
//# sourceMappingURL=normalise-input-multiple.d.ts.map