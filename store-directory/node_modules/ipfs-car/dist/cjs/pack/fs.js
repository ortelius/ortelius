"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.packToFs = void 0;
const fs_1 = __importDefault(require("fs"));
const os_1 = __importDefault(require("os"));
const path_1 = __importDefault(require("path"));
const move_file_1 = __importDefault(require("move-file"));
const stream_1 = require("./stream");
const fs_2 = require("../blockstore/fs");
async function packToFs({ input, output, blockstore: userBlockstore, hasher, maxChunkSize, maxChildrenPerNode, wrapWithDirectory, rawLeaves }) {
    const blockstore = userBlockstore ? userBlockstore : new fs_2.FsBlockStore();
    const location = output || `${os_1.default.tmpdir()}/${(parseInt(String(Math.random() * 1e9), 10)).toString() + Date.now()}`;
    const writable = fs_1.default.createWriteStream(location);
    const { root } = await (0, stream_1.packToStream)({
        input,
        writable,
        blockstore,
        hasher,
        maxChunkSize,
        maxChildrenPerNode,
        wrapWithDirectory,
        rawLeaves
    });
    if (!userBlockstore) {
        await blockstore.close();
    }
    // Move to work dir
    if (!output) {
        const basename = typeof input === 'string' ? path_1.default.parse(path_1.default.basename(input)).name : root.toString();
        const filename = `${basename}.car`;
        await (0, move_file_1.default)(location, `${process.cwd()}/${filename}`);
        return { root, filename };
    }
    return { root, filename: output };
}
exports.packToFs = packToFs;
