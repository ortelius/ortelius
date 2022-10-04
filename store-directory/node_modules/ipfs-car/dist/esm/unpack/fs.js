import fs from 'fs';
import pipe from 'it-pipe';
import { map } from 'streaming-iterables';
import { CarIndexedReader } from '@ipld/car';
import { FsBlockStore } from "../blockstore/fs.js";
// @ts-ignore stream-to-it has no types exported
import toIterable from 'stream-to-it';
import { unpack, unpackStream } from "./index.js";
// Node only, read a car from fs, write files to fs
export async function unpackToFs({ input, roots, output }) {
    const carReader = await CarIndexedReader.fromFile(input);
    await writeFiles(unpack(carReader, roots), output);
}
// Node only, read a stream, write files to fs
export async function unpackStreamToFs({ input, roots, output, blockstore: userBlockstore }) {
    const blockstore = userBlockstore ? userBlockstore : new FsBlockStore();
    await writeFiles(unpackStream(input, { roots, blockstore }), output);
    if (!userBlockstore) {
        await blockstore.close();
    }
}
export async function writeFiles(source, output) {
    for await (const file of source) {
        let filePath = file.path;
        // output overrides the first part of the path.
        if (output) {
            const parts = file.path.split('/');
            parts[0] = output;
            filePath = parts.join('/');
        }
        if (file.type === 'file' || file.type === 'raw') {
            await pipe(file.content, map((chunk) => chunk.slice()), // BufferList to Buffer
            toIterable.sink(fs.createWriteStream(filePath)));
        }
        else if (file.type === 'directory') {
            await fs.promises.mkdir(filePath, { recursive: true });
        }
        else {
            throw new Error(`Unsupported UnixFS type ${file.type} for ${file.path}`);
        }
    }
}
