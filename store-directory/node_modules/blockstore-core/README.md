# js-blockstore-core <!-- omit in toc -->

[![](https://img.shields.io/badge/made%20by-Protocol%20Labs-blue.svg?style=flat-square)](http://ipn.io)
[![](https://img.shields.io/badge/project-IPFS-blue.svg?style=flat-square)](http://ipfs.io/)
[![](https://img.shields.io/badge/freenode-%23ipfs-blue.svg?style=flat-square)](http://webchat.freenode.net/?channels=%23ipfs)
[![codecov](https://img.shields.io/codecov/c/github/ipfs/js-blockstore-core.svg?style=flat-square)](https://codecov.io/gh/ipfs/js-blockstore-core)
[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/ipfs/js-blockstore-core/ci?label=ci&style=flat-square)](https://github.com/ipfs/js-blockstore-core/actions?query=branch%3Amaster+workflow%3Aci+)

> Contains various implementations of the API contract described in [interface-blockstore](https://github.com/ipfs/js-ipfs-interfaces/tree/master/packages/interface-blockstore)

## Table of Contents <!-- omit in toc -->

- [Implementations](#implementations)
- [Install](#install)
- [Usage](#usage)
  - [BaseBlockstore](#baseblockstore)
  - [MemoryBlockstore](#memoryblockstore)
- [Contribute](#contribute)
- [License](#license)

## Implementations

- Base: [`src/base`](src/base.js)
- Memory: [`src/memory`](src/memory.js)

## Install

```
$ npm install blockstore-core
```

## Usage

### BaseBlockstore

Provides a complete implementation of the Blockstore interface.  You must implement `.get`, `.put`, etc.

```js
import { BaseBlockstore } from 'blockstore-core/base'

class MyCustomBlockstore extends BaseBlockstore {
  put (key, val, options) {
    // store a block
  }

  get (key, options) {
    // retrieve a block
  }

  // ...etc
}
```

### MemoryBlockstore

A simple Blockstore that stores blocks in memory.

```js
import { MemoryBlockstore } from 'blockstore-core/memory'

const store = new MemoryBlockstore()
```

## Contribute

Feel free to join in. All welcome. Open an [issue](https://github.com/ipfs/js-ipfs-unixfs-importer/issues)!

This repository falls under the IPFS [Code of Conduct](https://github.com/ipfs/community/blob/master/code-of-conduct.md).

[![](https://cdn.rawgit.com/jbenet/contribute-ipfs-gif/master/img/contribute.gif)](https://github.com/ipfs/community/blob/master/CONTRIBUTING.md)

## License

[Apache-2.0](LICENSE-APACHE) OR [MIT](LICENSE-MIT)
