/**
 * @typedef {{ decoders?: import('multiformats/codecs/interface').BlockDecoder<any, any>[] }} Options
 * @typedef {import('@ipld/car/api').WriterChannel & { size: number }} WriterChannel
 * @typedef {import('multiformats/cid').CID} CID
 * @typedef {import('@ipld/car/api').BlockReader & import('@ipld/car/api').RootsReader} ICarReader
 */
export class TreewalkCarSplitter {
    /**
     * @param {AsyncIterable<Uint8Array>} iterable
     * @param {number} targetSize
     * @param {Options} [options]
     */
    static fromIterable(iterable: AsyncIterable<Uint8Array>, targetSize: number, options?: Options | undefined): Promise<TreewalkCarSplitter>;
    /**
     * @param {Blob} blob
     * @param {number} targetSize
     * @param {Options} [options]
     */
    static fromBlob(blob: Blob, targetSize: number, options?: Options | undefined): Promise<TreewalkCarSplitter>;
    /**
     * @param {ICarReader} reader
     * @param {number} targetSize
     * @param {Options} [options]
     */
    constructor(reader: ICarReader, targetSize: number, options?: Options | undefined);
    _reader: ICarReader;
    _targetSize: number;
    /** @type {import('multiformats/codecs/interface').BlockDecoder<any, any>[]} */
    _decoders: import('multiformats/codecs/interface').BlockDecoder<any, any>[];
    cars(): AsyncGenerator<AsyncIterable<Uint8Array>, void, unknown>;
    /**
     * @param {import('multiformats/cid').CID} cid
     */
    _get(cid: import('multiformats/cid').CID): Promise<Block<any>>;
    /**
     * @param {CID} cid
     * @param {Block<any>[]} [parents]
     * @param {WriterChannel} [channel]
     * @returns {AsyncGenerator<{ channel: WriterChannel, out?: AsyncIterable<Uint8Array> }>}
     */
    _cars(cid: CID, parents?: Block<any>[] | undefined, channel?: WriterChannel | undefined): AsyncGenerator<{
        channel: WriterChannel;
        out?: AsyncIterable<Uint8Array>;
    }>;
}
export type Options = {
    decoders?: import('multiformats/codecs/interface').BlockDecoder<any, any>[];
};
export type WriterChannel = import('@ipld/car/api').WriterChannel & {
    size: number;
};
export type CID = import('multiformats/cid').CID;
export type ICarReader = import('@ipld/car/api').BlockReader & import('@ipld/car/api').RootsReader;
import { Block } from "multiformats/block";
//# sourceMappingURL=splitter.d.ts.map