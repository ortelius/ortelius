import last from 'it-last';
import pipe from 'it-pipe';
import { CarWriter } from '@ipld/car';
import { importer } from 'ipfs-unixfs-importer';
import { getNormaliser } from "./utils/normalise-input.js";
import { MemoryBlockStore } from "../blockstore/memory.js";
import { unixfsImporterOptionsDefault } from "./constants.js";
export async function pack({ input, blockstore: userBlockstore, hasher, maxChunkSize, maxChildrenPerNode, wrapWithDirectory, rawLeaves }) {
    if (!input || (Array.isArray(input) && !input.length)) {
        throw new Error('missing input file(s)');
    }
    const blockstore = userBlockstore ? userBlockstore : new MemoryBlockStore();
    // Consume the source
    const rootEntry = await last(pipe(getNormaliser(input), (source) => importer(source, blockstore, {
        ...unixfsImporterOptionsDefault,
        hasher: hasher || unixfsImporterOptionsDefault.hasher,
        maxChunkSize: maxChunkSize || unixfsImporterOptionsDefault.maxChunkSize,
        maxChildrenPerNode: maxChildrenPerNode || unixfsImporterOptionsDefault.maxChildrenPerNode,
        wrapWithDirectory: wrapWithDirectory === false ? false : unixfsImporterOptionsDefault.wrapWithDirectory,
        rawLeaves: rawLeaves == null ? unixfsImporterOptionsDefault.rawLeaves : rawLeaves
    })));
    if (!rootEntry || !rootEntry.cid) {
        throw new Error('given input could not be parsed correctly');
    }
    const root = rootEntry.cid;
    const { writer, out: carOut } = await CarWriter.create([root]);
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
