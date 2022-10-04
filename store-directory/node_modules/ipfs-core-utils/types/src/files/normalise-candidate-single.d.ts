/**
 * @typedef {import('ipfs-core-types/src/utils').ToContent} ToContent
 * @typedef {import('ipfs-unixfs-importer').ImportCandidate} ImporterImportCandidate
 * @typedef {import('ipfs-core-types/src/utils').ImportCandidate} ImportCandidate
 * @typedef {import('ipfs-core-types/src/utils').ImportCandidateStream} ImportCandidateStream
 */
/**
 * @param {ImportCandidate} input
 * @param {(content:ToContent) => Promise<AsyncIterable<Uint8Array>>} normaliseContent
 */
export function normaliseCandidateSingle(input: ImportCandidate, normaliseContent: (content: ToContent) => Promise<AsyncIterable<Uint8Array>>): AsyncGenerator<import("ipfs-unixfs-importer/types/src/types").ImportCandidate, void, unknown>;
export type ToContent = import('ipfs-core-types/src/utils').ToContent;
export type ImporterImportCandidate = import('ipfs-unixfs-importer').ImportCandidate;
export type ImportCandidate = import('ipfs-core-types/src/utils').ImportCandidate;
export type ImportCandidateStream = import('ipfs-core-types/src/utils').ImportCandidateStream;
//# sourceMappingURL=normalise-candidate-single.d.ts.map