"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.unpackStream = exports.unpack = void 0;
const browser_readablestream_to_it_1 = __importDefault(require("browser-readablestream-to-it"));
const iterator_1 = require("@ipld/car/iterator");
const ipfs_unixfs_exporter_1 = require("ipfs-unixfs-exporter");
const verifying_get_only_blockstore_1 = require("./utils/verifying-get-only-blockstore");
const memory_1 = require("../blockstore/memory");
// Export unixfs entries from car file
async function* unpack(carReader, roots) {
    const verifyingBlockService = verifying_get_only_blockstore_1.VerifyingGetOnlyBlockStore.fromCarReader(carReader);
    if (!roots || roots.length === 0) {
        roots = await carReader.getRoots();
    }
    for (const root of roots) {
        yield* (0, ipfs_unixfs_exporter_1.recursive)(root, verifyingBlockService, { /* options */});
    }
}
exports.unpack = unpack;
async function* unpackStream(readable, { roots, blockstore: userBlockstore } = {}) {
    const carIterator = await iterator_1.CarBlockIterator.fromIterable(asAsyncIterable(readable));
    const blockstore = userBlockstore || new memory_1.MemoryBlockStore();
    for await (const block of carIterator) {
        await blockstore.put(block.cid, block.bytes);
    }
    const verifyingBlockStore = verifying_get_only_blockstore_1.VerifyingGetOnlyBlockStore.fromBlockstore(blockstore);
    if (!roots || roots.length === 0) {
        roots = await carIterator.getRoots();
    }
    for (const root of roots) {
        yield* (0, ipfs_unixfs_exporter_1.recursive)(root, verifyingBlockStore);
    }
}
exports.unpackStream = unpackStream;
/**
 * Upgrade a ReadableStream to an AsyncIterable if it isn't already
 *
 * ReadableStream (e.g res.body) is asyncIterable in node, but not in chrome, yet.
 * see: https://bugs.chromium.org/p/chromium/issues/detail?id=929585
 */
function asAsyncIterable(readable) {
    // @ts-ignore how to convince tsc that we are checking the type here?
    return Symbol.asyncIterator in readable ? readable : (0, browser_readablestream_to_it_1.default)(readable);
}
