# ipfs-car 🚘✨⬢

> Convert files to content-addressable archives (.car) and back

[![Build](https://github.com/web3-storage/ipfs-car/actions/workflows/main.yml/badge.svg)](https://github.com/web3-storage/ipfs-car/actions/workflows/main.yml)
[![dependencies Status](https://status.david-dm.org/gh/web3-storage/ipfs-car.svg)](https://david-dm.org/web3-storage/ipfs-car)
[![JavaScript Style Guide](https://img.shields.io/badge/code_style-standard-brightgreen.svg)](https://standardjs.com)
[![Downloads](https://img.shields.io/npm/dm/ipfs-car.svg)](https://www.npmjs.com/package/ipfs-car)
[![Minzipped size](https://badgen.net/bundlephobia/minzip/ipfs-car)](https://bundlephobia.com/result?p=ipfs-car)

## Description

`ipfs-car` is a library and CLI tool to pack & unpack files from [Content Addressable aRchives (CAR)](https://github.com/ipld/specs/blob/master/block-layer/content-addressable-archives.md) file. A thin wrapper over [@ipld/car](https://github.com/ipld/js-car) and [unix-fs](https://github.com/ipfs/js-ipfs-unixfs).

Content-addressable archives store data as blocks (a sequence of bytes) each prefixed with the [Content ID (CID)](https://docs.ipfs.io/concepts/content-addressing/) derived from the hash of the data; typically in a file with a `.car` extension.

Use `ipfs-car` to pack your files into a .car; a portable, verifiable, IPFS compatible archive.

```sh
$ ipfs-car --pack path/to/files --output my-files.car
```

or unpack files from a .car, and verify that every block matches it's CID

```sh
$ ipfs-car --unpack my-files.car --output path/to/write/to
```

Fetch and locally verify files from a IPFS gateway over http

```sh
curl -X POST "https://ipfs.io/api/v0/dag/export?arg=bafybeidd2gyhagleh47qeg77xqndy2qy3yzn4vkxmk775bg2t5lpuy7pcu" | 🚘
```

## Install

```sh
# install it as a dependency
$ npm i ipfs-car

# or use the cli without installing via `npx`
$ npx ipfs-car --help
```

## Usage

`--pack` files into a .car

```sh
# write a content addressed archive to the current working dir.
$ ipfs-car --pack path/to/file/or/dir

# specify the car file name.
$ ipfs-car --pack path/to/files --output path/to/write/a.car
```

`--unpack` files from a .car

```sh
# unpack files to a specific path.
$ ipfs-car --unpack path/to/my.car --output /path/to/unpack/files/to

# unpack specific roots
$ ipfs-car --unpack path/to/my.car --root <cid1> [--root <cid2>]

# unpack files from a .car on stdin.
$ cat path/to/my.car | ipfs-car --unpack
```

List the contents of a .car

```sh
# list the files.
$ ipfs-car --list path/to/my.car

# list the cid roots.
$ ipfs-car --list-roots path/to/my.car

# list the cids for all the blocks.
$ ipfs-car --list-cids path/to/my.car
```

## API

To pack files into content-addressable archives, you can use the functions provided in:

- `ipfs-car/pack` for consuming a [CAR writer](https://github.com/ipld/js-car#carwriter) async iterable
- `ipfs-car/pack/blob` for getting a blob with the CAR file
- `ipfs-car/pack/fs` for storing in the local file system (**Node.js only**)
- `ipfs-car/pack/stream` for writing to a writable stream (**Node.js only**)

⚠️ While packing files into CAR files, a [Blockstore](./src/blockstore) is used for temporary storage. All pack functions provide a default, but you can use other options (if supported on your runtime).

To unpack content-addressable archives to files, you can use the functions provided in:

- `ipfs-car/unpack` for getting an async iterable of the UnixFS entries stored in the CAR file
- `ipfs-car/unpack/fs` for writing the unpacked files to disk (**Node.js only**)

### `ipfs-car/pack`

Takes an [ImportCandidateStream](https://github.com/ipfs/js-ipfs/blob/master/packages/ipfs-core-types/src/utils.d.ts#L27) and returns a a [CAR writer](https://github.com/ipld/js-car#carwriter) async iterable.

```js
import { pack } from 'ipfs-car/pack'
import { MemoryBlockStore } from 'ipfs-car/blockstore/memory' // You can also use the `level-blockstore` module

const { root, out } = await pack({
  input: [new Uint8Array([21, 31, 41])],
  blockstore: new MemoryBlockStore(),
  wrapWithDirectory: true // Wraps input into a directory. Defaults to `true`
  maxChunkSize: 262144 // The maximum block size in bytes. Defaults to `262144`. Max safe value is < 1048576 (1MiB)
})

const carParts = []
for await (const part of out) {
  carParts.push(part)
}
```

### `ipfs-car/pack/blob`

Takes an [ImportCandidateStream](https://github.com/ipfs/js-ipfs/blob/master/packages/ipfs-core-types/src/utils.d.ts#L27) and writes it to a [Blob](https://github.com/web-std/io/tree/main/blob).

```js
import { packToBlob } from 'ipfs-car/pack/blob'
import { MemoryBlockStore } from 'ipfs-car/blockstore/memory' // You can also use the `level-blockstore` module

const { root, car } = await packToBlob({
  input: [new Uint8Array([21, 31, 41])],
  blockstore: new MemoryBlockStore()
})
```

### `ipfs-car/pack/fs`

Takes a path on disk and writes it to CAR file (**Node.js only**).

```js
import { packToFs } from 'ipfs-car/pack/fs'
import { FsBlockStore } from 'ipfs-car/blockstore/fs'

await packToFs({
  input: `${process.cwd()}/path/to/files`,
  output: `${process.cwd()}/output.car`,
  blockstore: new FsBlockStore()
})
// output.car file now exists in process.cwd()
```

### `ipfs-car/pack/stream`

Takes a writable stream and pipes the CAR Writer stream to it (**Node.js only**).

```js
import fs from 'fs'
import { packToStream } from 'ipfs-car/pack/stream'
import { FsBlockStore } from 'ipfs-car/blockstore/fs'

const writable = fs.createWriteStream(`${process.cwd()}/output.car`)

await packToStream({
  input: `${process.cwd()}/path/to/files`,
  writable,
  blockstore: new FsBlockStore()
})
// output.car file now exists in process.cwd()
```

### `ipfs-car/unpack`

Takes a CAR reader and yields files to be consumed.

```js
import fs from 'fs'
import { unpack } from 'ipfs-car/unpack'

const inStream = fs.createReadStream(`${process.cwd()}/output.car`)
const carReader = await CarReader.fromIterable(inStream)

const files = []
for await (const file of unpack(carReader)) {
  // Iterate over files
}
```

Takes an AsyncIterable and yields files to be consumed.

```js
import fs from 'fs'
import { unpackStream } from 'ipfs-car/unpack'

const inStream = fs.createReadStream(`${process.cwd()}/output.car`)

const files = []
for await (const file of unpackStream(inStream)) {
  // Iterate over files
}
```

`unpackStream` takes an options object, allowing you to pass in a `BlockStore` implementation. The blocks are unpacked from the stream in the order they appear, which may not be the order needed to reassemble them into the Files and Directories they represent. The blockstore is used to store the blocks as they are consumed from the stream. Once the stream is consumed, the blockstore provides the random access by CID to the blocks, needed to assemble the tree.

The default is a [`MemoryBlockStore`](./src/blockstore/memory.ts), that will store all the blocks in memory.
For larger CARs in the browser you can use IndexedDB by passing in an [IdbBlocksStore]('./src/blockstore/idb.ts'), and in Node.js you can provide a [FsBlockStore] instance to write blocks to the tmp dir.

```js
/* browser */
import { unpackStream } from 'ipfs-car/unpack'
import { IdbBlockStore } from 'ipfs-car/blockstore/idb'

const res = fetch(
  'https://ipfs.io/api/v0/dag/export?arg=bafkreigh2akiscaildcqabsyg3dfr6chu3fgpregiymsck7e7aqa4s52zy',
  { method: 'POST' }
)
const files = []
const blockstore = new IdbBlockStore()
for await (const file of unpackStream(res.body, { blockstore })) {
  // Iterate over files
}
blockstore.destroy()
```

When providing a custom Blockstore, it is your responsibiltiy to call `blockstore.destroy()` when you're finished. Failing to do so will fill up the users storage.

### `ipfs-car/unpack/fs`

Takes a path to a CAR file on disk and unpacks it to a given path

```js
import { unpackToFs } from 'ipfs-car/unpack/fs'

await unpackToFs({
  input: `${process.cwd()}/my.car`,
  output: `${process.cwd()}/foo`
})
// foo now exists in process.cwd()
// it is either a file or a directory depending on the contents of the .car
```

Takes a stream to a CAR file and unpacks it to a given path on disc

```js
import fs from 'fs'
import { unpackStreamToFs } from 'ipfs-car/unpack/fs'

const input = fs.createReadStream(`${process.cwd()}/my.car`)

await unpackStreamToFs({
  input,
  output: `${process.cwd()}/foo`
})
// foo now exists in process.cwd()
// it is either a file or a directory depending on the contents of the .car
```
