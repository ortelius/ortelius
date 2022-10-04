import type { ImportCandidateStream, ImportCandidate } from 'ipfs-core-types/src/utils';
/**
 * Get a single or multiple normaliser depending on the input.
 */
export declare function getNormaliser(input: ImportCandidateStream | ImportCandidate): AsyncGenerator<import("ipfs-unixfs-importer/types/src/types").ImportCandidate, void, unknown>;
