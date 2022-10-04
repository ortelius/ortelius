import toIterable from 'browser-readablestream-to-it';
import { CarBlockIterator } from '@ipld/car/iterator';
import { recursive as unixFsExporter } from 'ipfs-unixfs-exporter';
import { VerifyingGetOnlyBlockStore } from "./utils/verifying-get-only-blockstore.js";
import { MemoryBlockStore } from "../blockstore/memory.js";
// Export unixfs entries from car file
export async function* unpack(carReader, roots) {
    const verifyingBlockService = VerifyingGetOnlyBlockStore.fromCarReader(carReader);
    if (!roots || roots.length === 0) {
        roots = await carReader.getRoots();
    }
    for (const root of roots) {
        yield* unixFsExporter(root, verifyingBlockService, { /* options */});
    }
}
export async function* unpackStream(readable, { roots, blockstore: userBlockstore } = {}) {
    const carIterator = await CarBlockIterator.fromIterable(asAsyncIterable(readable));
    const blockstore = userBlockstore || new MemoryBlockStore();
    for await (const block of carIterator) {
        await blockstore.put(block.cid, block.bytes);
    }
    const verifyingBlockStore = VerifyingGetOnlyBlockStore.fromBlockstore(blockstore);
    if (!roots || roots.length === 0) {
        roots = await carIterator.getRoots();
    }
    for (const root of roots) {
        yield* unixFsExporter(root, verifyingBlockStore);
    }
}
/**
 * Upgrade a ReadableStream to an AsyncIterable if it isn't already
 *
 * ReadableStream (e.g res.body) is asyncIterable in node, but not in chrome, yet.
 * see: https://bugs.chromium.org/p/chromium/issues/detail?id=929585
 */
function asAsyncIterable(readable) {
    // @ts-ignore how to convince tsc that we are checking the type here?
    return Symbol.asyncIterator in readable ? readable : toIterable(readable);
}
