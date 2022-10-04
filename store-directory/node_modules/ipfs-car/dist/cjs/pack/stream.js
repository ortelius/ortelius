"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.packToStream = void 0;
const fs_1 = __importDefault(require("fs"));
const path_1 = __importDefault(require("path"));
const stream_1 = require("stream");
const it_last_1 = __importDefault(require("it-last"));
const it_pipe_1 = __importDefault(require("it-pipe"));
const car_1 = require("@ipld/car");
const ipfs_unixfs_importer_1 = require("ipfs-unixfs-importer");
const normalise_input_multiple_1 = require("ipfs-core-utils/files/normalise-input-multiple");
const glob_source_js_1 = __importDefault(require("ipfs-utils/src/files/glob-source.js"));
const memory_1 = require("../blockstore/memory");
const constants_1 = require("./constants");
// Node version of toCar with Node Stream Writable
async function packToStream({ input, writable, blockstore: userBlockstore, hasher, maxChunkSize, maxChildrenPerNode, wrapWithDirectory, rawLeaves }) {
    if (!input || (Array.isArray(input) && !input.length)) {
        throw new Error('given input could not be parsed correctly');
    }
    input = typeof input === 'string' ? [input] : input;
    const blockstore = userBlockstore ? userBlockstore : new memory_1.MemoryBlockStore();
    // Consume the source
    const rootEntry = await (0, it_last_1.default)((0, it_pipe_1.default)(legacyGlobSource(input), source => (0, normalise_input_multiple_1.normaliseInput)(source), (source) => (0, ipfs_unixfs_importer_1.importer)(source, blockstore, {
        ...constants_1.unixfsImporterOptionsDefault,
        hasher: hasher || constants_1.unixfsImporterOptionsDefault.hasher,
        maxChunkSize: maxChunkSize || constants_1.unixfsImporterOptionsDefault.maxChunkSize,
        maxChildrenPerNode: maxChildrenPerNode || constants_1.unixfsImporterOptionsDefault.maxChildrenPerNode,
        wrapWithDirectory: wrapWithDirectory === false ? false : constants_1.unixfsImporterOptionsDefault.wrapWithDirectory,
        rawLeaves: rawLeaves == null ? constants_1.unixfsImporterOptionsDefault.rawLeaves : rawLeaves
    })));
    if (!rootEntry || !rootEntry.cid) {
        throw new Error('given input could not be parsed correctly');
    }
    const root = rootEntry.cid;
    const { writer, out } = await car_1.CarWriter.create([root]);
    stream_1.Readable.from(out).pipe(writable);
    for await (const block of blockstore.blocks()) {
        await writer.put(block);
    }
    await writer.close();
    if (!userBlockstore) {
        await blockstore.close();
    }
    return { root };
}
exports.packToStream = packToStream;
/**
 * This function replicates the old behaviour of globSource to not introduce a
 * breaking change.
 *
 * TODO: figure out what the breaking change will be.
 */
async function* legacyGlobSource(input) {
    for await (const p of input) {
        const resolvedPath = path_1.default.resolve(p);
        const stat = await fs_1.default.promises.stat(resolvedPath);
        const fileName = path_1.default.basename(resolvedPath);
        if (stat.isDirectory()) {
            yield { path: fileName };
            for await (const candidate of (0, glob_source_js_1.default)(resolvedPath, '**/*')) {
                yield { ...candidate, path: path_1.default.join(fileName, candidate.path) };
            }
        }
        else {
            yield { path: fileName, content: fs_1.default.createReadStream(resolvedPath) };
        }
    }
}
