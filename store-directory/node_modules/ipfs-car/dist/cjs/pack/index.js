"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.pack = void 0;
const it_last_1 = __importDefault(require("it-last"));
const it_pipe_1 = __importDefault(require("it-pipe"));
const car_1 = require("@ipld/car");
const ipfs_unixfs_importer_1 = require("ipfs-unixfs-importer");
const normalise_input_1 = require("./utils/normalise-input");
const memory_1 = require("../blockstore/memory");
const constants_1 = require("./constants");
async function pack({ input, blockstore: userBlockstore, hasher, maxChunkSize, maxChildrenPerNode, wrapWithDirectory, rawLeaves }) {
    if (!input || (Array.isArray(input) && !input.length)) {
        throw new Error('missing input file(s)');
    }
    const blockstore = userBlockstore ? userBlockstore : new memory_1.MemoryBlockStore();
    // Consume the source
    const rootEntry = await (0, it_last_1.default)((0, it_pipe_1.default)((0, normalise_input_1.getNormaliser)(input), (source) => (0, ipfs_unixfs_importer_1.importer)(source, blockstore, {
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
    const { writer, out: carOut } = await car_1.CarWriter.create([root]);
    const carOutIter = carOut[Symbol.asyncIterator]();
    let writingPromise;
    const writeAll = async () => {
        for await (const block of blockstore.blocks()) {
            // `await` will block until all bytes in `carOut` are consumed by the user
            // so we have backpressure here
            await writer.put(block);
        }
        await writer.close();
        if (!userBlockstore) {
            await blockstore.close();
        }
    };
    const out = {
        [Symbol.asyncIterator]() {
            if (writingPromise != null) {
                throw new Error('Multiple iterator not supported');
            }
            // don't start writing until the user starts consuming the iterator
            writingPromise = writeAll();
            return {
                async next() {
                    const result = await carOutIter.next();
                    if (result.done) {
                        await writingPromise; // any errors will propagate from here
                    }
                    return result;
                }
            };
        }
    };
    return { root, out };
}
exports.pack = pack;
