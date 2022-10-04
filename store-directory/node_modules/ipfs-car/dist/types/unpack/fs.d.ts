import { CID } from 'multiformats';
import { UnixFSEntry } from 'ipfs-unixfs-exporter';
import { Blockstore } from '../blockstore/index';
export declare function unpackToFs({ input, roots, output }: {
    input: string;
    roots?: CID[];
    output?: string;
}): Promise<void>;
export declare function unpackStreamToFs({ input, roots, output, blockstore: userBlockstore }: {
    input: AsyncIterable<Uint8Array>;
    roots?: CID[];
    output?: string;
    blockstore?: Blockstore;
}): Promise<void>;
export declare function writeFiles(source: AsyncIterable<UnixFSEntry>, output?: string): Promise<void>;
