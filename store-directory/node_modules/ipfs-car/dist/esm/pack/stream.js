import fs from 'fs';
import path from 'path';
import { Readable } from 'stream';
import last from 'it-last';
import pipe from 'it-pipe';
import { CarWriter } from '@ipld/car';
import { importer } from 'ipfs-unixfs-importer';
import { normaliseInput } from 'ipfs-core-utils/files/normalise-input-multiple';
import globSource from 'ipfs-utils/src/files/glob-source.js';
import { MemoryBlockStore } from "../blockstore/memory.js";
import { unixfsImporterOptionsDefault } from "./constants.js";
// Node version of toCar with Node Stream Writable
export async function packToStream({ input, writable, blockstore: userBlockstore, hasher, maxChunkSize, maxChildrenPerNode, wrapWithDirectory, rawLeaves }) {
    if (!input || (Array.isArray(input) && !input.length)) {
        throw new Error('given input could not be parsed correctly');
    }
    input = typeof input === 'string' ? [input] : input;
    const blockstore = userBlockstore ? userBlockstore : new MemoryBlockStore();
    // Consume the source
    const rootEntry = await last(pipe(legacyGlobSource(input), source => normaliseInput(source), (source) => importer(source, blockstore, {
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
    const { writer, out } = await CarWriter.create([root]);
    Readable.from(out).pipe(writable);
    for await (const block of blockstore.blocks()) {
        await writer.put(block);
    }
    await writer.close();
    if (!userBlockstore) {
        await blockstore.close();
    }
    return { root };
}
/**
 * This function replicates the old behaviour of globSource to not introduce a
 * breaking change.
 *
 * TODO: figure out what the breaking change will be.
 */
async function* legacyGlobSource(input) {
    for await (const p of input) {
        const resolvedPath = path.resolve(p);
        const stat = await fs.promises.stat(resolvedPath);
        const fileName = path.basename(resolvedPath);
        if (stat.isDirectory()) {
            yield { path: fileName };
            for await (const candidate of globSource(resolvedPath, '**/*')) {
                yield { ...candidate, path: path.join(fileName, candidate.path) };
            }
        }
        else {
            yield { path: fileName, content: fs.createReadStream(resolvedPath) };
        }
    }
}
