import { sha256 } from 'multiformats/hashes/sha2';
export const unixfsImporterOptionsDefault = {
    cidVersion: 1,
    chunker: 'fixed',
    maxChunkSize: 262144,
    hasher: sha256,
    rawLeaves: true,
    wrapWithDirectory: true,
    maxChildrenPerNode: 174
};
