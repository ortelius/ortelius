# ipfs-unixfs-importer <!-- omit in toc -->

[![](https://img.shields.io/badge/made%20by-Protocol%20Labs-blue.svg?style=flat-square)](http://ipn.io)
[![](https://img.shields.io/badge/project-IPFS-blue.svg?style=flat-square)](http://ipfs.io/)
[![](https://img.shields.io/badge/freenode-%23ipfs-blue.svg?style=flat-square)](http://webchat.freenode.net/?channels=%23ipfs)
[![Build Status](https://flat.badgen.net/travis/ipfs/js-ipfs-unixfs)](https://travis-ci.com/ipfs/js-ipfs-unixfs)
[![Codecov](https://codecov.io/gh/ipfs/js-ipfs-unixfs/branch/master/graph/badge.svg)](https://codecov.io/gh/ipfs/js-ipfs-unixfs)

> JavaScript implementation of the layout and chunking mechanisms used by IPFS to handle Files

## Lead Maintainer <!-- omit in toc -->

[Alex Potsides](https://github.com/achingbrain)

## Table of Contents <!-- omit in toc -->

- [Install](#install)
- [Usage](#usage)
  - [Example](#example)
    - [API](#api)
    - [const stream = importer(source, blockstore [, options])](#const-stream--importersource-blockstore--options)
- [Overriding internals](#overriding-internals)
- [Contribute](#contribute)
- [License](#license)

## Install

```
> npm install ipfs-unixfs-importer
```

## Usage

### Example

Let's create a little directory to import:

```sh
> cd /tmp
> mkdir foo
> echo 'hello' > foo/bar
> echo 'world' > foo/quux
```

And write the importing logic:

```js
import { importer } from 'ipfs-unixfs-importer'
import { MemoryBlockstore } from 'blockstore-core/memory'

// Where the blocks will be stored
const blockstore = new MemoryBlockstore()

// Import path /tmp/foo/bar
const source = [{
  path: '/tmp/foo/bar',
  content: fs.createReadStream(file)
}, {
  path: '/tmp/foo/quxx',
  content: fs.createReadStream(file2)
}]

for await (const entry of importer(source, blockstore, options)) {
  console.info(entry)
}
```

When run, metadata about DAGNodes in the created tree is printed until the root:

```js
{
  cid: CID, // see https://github.com/multiformats/js-cid
  path: 'tmp/foo/bar',
  unixfs: UnixFS // see https://github.com/ipfs/js-ipfs-unixfs
}
{
  cid: CID, // see https://github.com/multiformats/js-cid
  path: 'tmp/foo/quxx',
  unixfs: UnixFS // see https://github.com/ipfs/js-ipfs-unixfs
}
{
  cid: CID, // see https://github.com/multiformats/js-cid
  path: 'tmp/foo',
  unixfs: UnixFS // see https://github.com/ipfs/js-ipfs-unixfs
}
{
  cid: CID, // see https://github.com/multiformats/js-cid
  path: 'tmp',
  unixfs: UnixFS // see https://github.com/ipfs/js-ipfs-unixfs
}
```

#### API

```js
import { importer } from 'ipfs-unixfs-importer'
```

#### const stream = importer(source, blockstore [, options])

The `importer` function returns an async iterator takes a source async iterator that yields objects of the form:

```js
{
  path: 'a name',
  content: (Buffer or iterator emitting Buffers),
  mtime: (Number representing seconds since (positive) or before (negative) the Unix Epoch),
  mode: (Number representing ugo-rwx, setuid, setguid and sticky bit)
}
```

`stream` will output file info objects as files get stored in IPFS. When stats on a node are emitted they are guaranteed to have been written.

`blockstore` is an instance of a [blockstore][]

The input's file paths and directory structure will be preserved in the [`dag-pb`](https://github.com/ipld/js-dag-pb) created nodes.

`options` is an JavaScript option that might include the following keys:

- `wrapWithDirectory` (boolean, defaults to false): if true, a wrapping node will be created
- `shardSplitThreshold` (positive integer, defaults to 1000): the number of directory entries above which we decide to use a sharding directory builder (instead of the default flat one)
- `chunker` (string, defaults to `"fixed"`): the chunking strategy. Supports:
  - `fixed`
  - `rabin`
- `avgChunkSize` (positive integer, defaults to `262144`): the average chunk size (rabin chunker only)
- `minChunkSize` (positive integer): the minimum chunk size (rabin chunker only)
- `maxChunkSize` (positive integer, defaults to `262144`): the maximum chunk size
- `strategy` (string, defaults to `"balanced"`): the DAG builder strategy name. Supports:
  - `flat`: flat list of chunks
  - `balanced`: builds a balanced tree
  - `trickle`: builds [a trickle tree](https://github.com/ipfs/specs/pull/57#issuecomment-265205384)
- `maxChildrenPerNode` (positive integer, defaults to `174`): the maximum children per node for the `balanced` and `trickle` DAG builder strategies
- `layerRepeat` (positive integer, defaults to 4): (only applicable to the `trickle` DAG builder strategy). The maximum repetition of parent nodes for each layer of the tree.
- `reduceSingleLeafToSelf` (boolean, defaults to `true`): optimization for, when reducing a set of nodes with one node, reduce it to that node.
- `hamtHashFn` (async function(string) Buffer): a function that hashes file names to create HAMT shards
- `hamtBucketBits` (positive integer, defaults to `8`): the number of bits at each bucket of the HAMT
- `progress` (function): a function that will be called with the byte length of chunks as a file is added to ipfs.
- `onlyHash` (boolean, defaults to false): Only chunk and hash - do not write to disk
- `hashAlg` (string): multihash hashing algorithm to use
- `cidVersion` (integer, default 0): the CID version to use when storing the data (storage keys are based on the CID, _including_ it's version)
- `rawLeaves` (boolean, defaults to false): When a file would span multiple DAGNodes, if this is true the leaf nodes will not be wrapped in `UnixFS` protobufs and will instead contain the raw file bytes
- `leafType` (string, defaults to `'file'`) what type of UnixFS node leaves should be - can be `'file'` or `'raw'` (ignored when `rawLeaves` is `true`)
- `blockWriteConcurrency` (positive integer, defaults to 10) How many blocks to hash and write to the block store concurrently. For small numbers of large files this should be high (e.g. 50).
- `fileImportConcurrency` (number, defaults to 50) How many files to import concurrently. For large numbers of small files this should be high (e.g. 50).

## Overriding internals

Several aspects of the importer are overridable by specifying functions as part of the options object with these keys:

- `chunkValidator` (function): Optional function that supports the signature `async function * (source, options)`
  - This function takes input from the `content` field of imported entries. It should transform them into `Buffer`s, throwing an error if it cannot.
  - It should yield `Buffer` objects constructed from the `source` or throw an `Error`
- `chunker` (function): Optional function that supports the signature `async function * (source, options)` where `source` is an async generator and `options` is an options object
  - It should yield `Buffer` objects.
- `bufferImporter` (function): Optional function that supports the signature `async function * (entry, blockstore, options)`
  - This function should read `Buffer`s from `source` and persist them using `blockstore.put` or similar
  - `entry` is the `{ path, content }` entry, where `entry.content` is an async generator that yields Buffers
  - It should yield functions that return a Promise that resolves to an object with the properties `{ cid, unixfs, size }` where `cid` is a [CID], `unixfs` is a [UnixFS] entry and `size` is a `Number` that represents the serialized size of the [IPLD] node that holds the buffer data.
  - Values will be pulled from this generator in parallel - the amount of parallelisation is controlled by the `blockWriteConcurrency` option (default: 10)
- `dagBuilder` (function): Optional function that supports the signature `async function * (source, blockstore, options)`
  - This function should read `{ path, content }` entries from `source` and turn them into DAGs
  - It should yield a `function` that returns a `Promise` that resolves to `{ cid, path, unixfs, node }` where `cid` is a `CID`, `path` is a string, `unixfs` is a UnixFS entry and `node` is a `DAGNode`.
  - Values will be pulled from this generator in parallel - the amount of parallelisation is controlled by the `fileImportConcurrency` option (default: 50)
- `treeBuilder` (function): Optional function that supports the signature `async function * (source, blockstore, options)`
  - This function should read `{ cid, path, unixfs, node }` entries from `source` and place them in a directory structure
  - It should yield an object with the properties `{ cid, path, unixfs, size }` where `cid` is a `CID`, `path` is a string, `unixfs` is a UnixFS entry and `size` is a `Number`.

[blockstore]: https://github.com/ipfs/js-ipfs-interfaces/tree/master/packages/interface-blockstore#readme
[UnixFS]: https://github.com/ipfs/specs/tree/master/unixfs
[IPLD]: https://github.com/ipld/js-ipld
[CID]: https://github.com/multiformats/js-cid

## Contribute

Feel free to join in. All welcome. Open an [issue](https://github.com/ipfs/js-ipfs-unixfs-importer/issues)!

This repository falls under the IPFS [Code of Conduct](https://github.com/ipfs/community/blob/master/code-of-conduct.md).

[![](https://cdn.rawgit.com/jbenet/contribute-ipfs-gif/master/img/contribute.gif)](https://github.com/ipfs/community/blob/master/contributing.md)

## License

[MIT](LICENSE)
