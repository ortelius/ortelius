/**
 * @typedef {import('ipfs-core-types/src/utils').ImportCandidate} ImportCandidate
 * @typedef {import('ipfs-core-types/src/utils').ToContent} ToContent
 * @typedef {import('ipfs-unixfs-importer').ImportCandidate} ImporterImportCandidate
 * @typedef {import('ipfs-core-types/src/utils').ImportCandidateStream} ImportCandidateStream
 */
/**
 * @param {ImportCandidateStream} input
 * @param {(content:ToContent) => Promise<AsyncIterable<Uint8Array>>} normaliseContent
 */
export function normaliseCandidateMultiple(input: ImportCandidateStream, normaliseContent: (content: ToContent) => Promise<AsyncIterable<Uint8Array>>): AsyncGenerator<import("ipfs-unixfs-importer/types/src/types").ImportCandidate, void, undefined>;
export type ImportCandidate = import('ipfs-core-types/src/utils').ImportCandidate;
export type ToContent = import('ipfs-core-types/src/utils').ToContent;
export type ImporterImportCandidate = import('ipfs-unixfs-importer').ImportCandidate;
export type ImportCandidateStream = import('ipfs-core-types/src/utils').ImportCandidateStream;
//# sourceMappingURL=normalise-candidate-multiple.d.ts.map