"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.writeFiles = exports.unpackStreamToFs = exports.unpackToFs = void 0;
const fs_1 = __importDefault(require("fs"));
const it_pipe_1 = __importDefault(require("it-pipe"));
const streaming_iterables_1 = require("streaming-iterables");
const car_1 = require("@ipld/car");
const fs_2 = require("../blockstore/fs");
// @ts-ignore stream-to-it has no types exported
const stream_to_it_1 = __importDefault(require("stream-to-it"));
const index_1 = require("./index");
// Node only, read a car from fs, write files to fs
async function unpackToFs({ input, roots, output }) {
    const carReader = await car_1.CarIndexedReader.fromFile(input);
    await writeFiles((0, index_1.unpack)(carReader, roots), output);
}
exports.unpackToFs = unpackToFs;
// Node only, read a stream, write files to fs
async function unpackStreamToFs({ input, roots, output, blockstore: userBlockstore }) {
    const blockstore = userBlockstore ? userBlockstore : new fs_2.FsBlockStore();
    await writeFiles((0, index_1.unpackStream)(input, { roots, blockstore }), output);
    if (!userBlockstore) {
        await blockstore.close();
    }
}
exports.unpackStreamToFs = unpackStreamToFs;
async function writeFiles(source, output) {
    for await (const file of source) {
        let filePath = file.path;
        // output overrides the first part of the path.
        if (output) {
            const parts = file.path.split('/');
            parts[0] = output;
            filePath = parts.join('/');
        }
        if (file.type === 'file' || file.type === 'raw') {
            await (0, it_pipe_1.default)(file.content, (0, streaming_iterables_1.map)((chunk) => chunk.slice()), // BufferList to Buffer
            stream_to_it_1.default.sink(fs_1.default.createWriteStream(filePath)));
        }
        else if (file.type === 'directory') {
            await fs_1.default.promises.mkdir(filePath, { recursive: true });
        }
        else {
            throw new Error(`Unsupported UnixFS type ${file.type} for ${file.path}`);
        }
    }
}
exports.writeFiles = writeFiles;
