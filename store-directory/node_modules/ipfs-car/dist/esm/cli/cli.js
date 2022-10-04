#!/usr/bin/env node
import meow from 'meow';
import { CID } from 'multiformats';
import { packToFs } from "../pack/fs.js";
import { unpackToFs, unpackStreamToFs } from "../unpack/fs.js";
import { listFilesInCar, listCidsInCar, listRootsInCar } from "./lib.js";
const options = {
    flags: {
        output: {
            type: 'string',
            alias: 'o',
        },
        pack: {
            type: 'string',
            alias: 'p'
        },
        unpack: {
            type: 'string',
            alias: 'u',
        },
        root: {
            type: 'string',
            isMultiple: true
        },
        list: {
            type: 'string',
            alias: 'l',
        },
        listCids: {
            type: 'string'
        },
        listRoots: {
            type: 'string'
        },
        wrapWithDirectory: {
            type: 'boolean',
            alias: 'w',
            default: true
        }
    }
};
const cli = meow(`
  Content Addressable archives (CAR) store IPLD block data as a sequence of bytes;
  typically in a file with a .car extension. The CAR format is a serialized
  representation of any IPLD DAG (graph) as the concatenation of its blocks, plus
  a header that describes the graphs in the file (via root CIDs).

  See: https://github.com/ipld/specs/blob/master/block-layer/content-addressable-archives.md

  Packing files into a .car

    # write a content addressed archive to the current working dir.
    $ ipfs-car --pack path/to/file/or/dir

    # specify the car file name.
    $ ipfs-car --pack path/to/files --output path/to/write/a.car

    # pack files without wrapping with top-level directory
    $ ipfs-car --wrapWithDirectory false --pack path/to/files --output path/to/write/a.car

  Unpacking files from a .car

    # write 1 or more files to the current working dir.
    $ ipfs-car --unpack path/to/my.car

    # unpack files to a specific path.
    $ ipfs-car --unpack path/to/my.car --output /path/to/unpack/files/to

    # unpack specific roots
    $ ipfs-car --unpack path/to/my.car --root <cid1> [--root <cid2>]

    # unpack files from a .car on stdin.
    $ cat path/to/my.car | ipfs-car --unpack

  Listing the contents of a .car

    # list the cids for all the blocks.
    $ ipfs-car --list-cids path/to/my.car

    # list the cid roots.
    $ ipfs-car --list-roots path/to/my.car

    # list the files.
    $ ipfs-car --list path/to/my.car

  TL;DR
  --pack <path> --output <my.car>
  --unpack <my.car> --output <path>

`, options);
async function handleInput({ flags }) {
    if (flags.pack) {
        const { root, filename } = await packToFs({ input: flags.pack, output: flags.output, wrapWithDirectory: flags.wrapWithDirectory });
        // tslint:disable-next-line: no-console
        console.log(`root CID: ${root.toString()}`);
        // tslint:disable-next-line: no-console
        console.log(`  output: ${filename}`);
    }
    else if (flags.unpack !== undefined) {
        const roots = (flags.root || []).map(r => CID.parse(r));
        if (flags.unpack === '') {
            return unpackStreamToFs({ input: process.stdin, roots, output: flags.output });
        }
        return unpackToFs({ input: flags.unpack, roots, output: flags.output });
    }
    else if (flags.list) {
        return listFilesInCar({ input: flags.list });
    }
    else if (flags.listRoots) {
        return listRootsInCar({ input: flags.listRoots });
    }
    else if (flags.listCids) {
        return listCidsInCar({ input: flags.listCids });
    }
    else if (!process.stdin.isTTY) {
        // maybe stream?
        // tslint:disable-next-line: no-console
        console.log('Reading .car from stdin');
        return unpackStreamToFs({ input: process.stdin, output: flags.output });
    }
    else {
        cli.showHelp();
        throw new Error('--pack or --unpack flag required');
    }
}
handleInput(cli);
