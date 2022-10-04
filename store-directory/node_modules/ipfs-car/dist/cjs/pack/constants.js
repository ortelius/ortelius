"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.unixfsImporterOptionsDefault = void 0;
const sha2_1 = require("multiformats/hashes/sha2");
exports.unixfsImporterOptionsDefault = {
    cidVersion: 1,
    chunker: 'fixed',
    maxChunkSize: 262144,
    hasher: sha2_1.sha256,
    rawLeaves: true,
    wrapWithDirectory: true,
    maxChildrenPerNode: 174
};
