import { CarReader } from '@ipld/car/api';
import { CID } from 'multiformats';
import type { UnixFSEntry } from 'ipfs-unixfs-exporter';
export type { UnixFSEntry };
import { Blockstore } from '../blockstore/index';
export declare function unpack(carReader: CarReader, roots?: CID[]): AsyncIterable<UnixFSEntry>;
export declare function unpackStream(readable: ReadableStream<Uint8Array> | AsyncIterable<Uint8Array>, { roots, blockstore: userBlockstore }?: {
    roots?: CID[];
    blockstore?: Blockstore;
}): AsyncIterable<UnixFSEntry>;
