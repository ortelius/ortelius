# hamt-sharding

[![](https://img.shields.io/badge/made%20by-Protocol%20Labs-blue.svg?style=flat-square)](http://ipn.io)
[![](https://img.shields.io/badge/project-IPFS-blue.svg?style=flat-square)](http://ipfs.io/)
[![](https://img.shields.io/badge/freenode-%23ipfs-blue.svg?style=flat-square)](http://webchat.freenode.net/?channels=%23ipfs)
[![standard-readme compliant](https://img.shields.io/badge/standard--readme-OK-green.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)
[![Build Status](https://flat.badgen.net/travis/ipfs-shipyard/js-hamt-sharding)](https://travis-ci.com/ipfs-shipyard/js-hamt-sharding)
[![Codecov](https://codecov.io/gh/ipfs-shipyard/js-hamt-sharding/branch/master/graph/badge.svg)](https://codecov.io/gh/ipfs-shipyard/js-hamt-sharding)
[![Dependency Status](https://david-dm.org/ipfs-shipyard/js-hamt-sharding.svg?style=flat-square)](https://david-dm.org/ipfs/js-hamt-sharding)
[![js-standard-style](https://img.shields.io/badge/code%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/feross/standard)
![](https://img.shields.io/badge/npm-%3E%3D3.0.0-orange.svg?style=flat-square)
![](https://img.shields.io/badge/Node.js-%3E%3D10.0.0-orange.svg?style=flat-square)

> JavaScript implementation of hamt for use in sharding

## Lead Maintainer

[Alex Potsides](https://github.com/achingbrain)

## Table of Contents

- [hamt-sharding](#hamt-sharding)
  - [Lead Maintainer](#lead-maintainer)
  - [Table of Contents](#table-of-contents)
  - [Install](#install)
  - [Usage](#usage)
    - [Example](#example)
  - [API](#api)
    - [`bucket.put(key, value)`](#bucketputkey-value)
    - [`bucket.get(key)`](#bucketgetkey)
    - [`bucket.del(key)`](#bucketdelkey)
    - [`bucket.leafCount()`](#bucketleafcount)
    - [`bucket.childrenCount()`](#bucketchildrencount)
    - [`bucket.onlyChild()`](#bucketonlychild)
    - [`bucket.eachLeafSeries()`](#bucketeachleafseries)
    - [`bucket.serialize(map, reduce)`](#bucketserializemap-reduce)
    - [`bucket.asyncTransform(asyncMap, asyncReduce)`](#bucketasynctransformasyncmap-asyncreduce)
    - [`bucket.toJSON()`](#buckettojson)
    - [`bucket.prettyPrint()`](#bucketprettyprint)
    - [`bucket.tableSize()`](#buckettablesize)
  - [Contribute](#contribute)
  - [License](#license)

## Install

```
> npm install hamt-sharding
```

## Usage

### Example

```javascript
const { createHAMT } = require('hamt-sharding')
const crypto = require('crypto-promise')

// decide how to hash buffers made from keys, can return a Promise
const hashFn = async (buf) => {
  return crypto
    .createHash('sha256')
    .update(buf)
    .digest()
}

const bucket = createHAMT({
  hashFn: hashFn
})

await bucket.put('key', 'value')

const output = await bucket.get('key')
// output === 'value'
```

## API

```javascript
const { createHAMT } = require('hamt-sharding')
```

### `bucket.put(key, value)`

```javascript
const { createHAMT } = require('hamt-sharding')
const bucket = createHAMT({...})

await bucket.put('key', 'value')
```

### `bucket.get(key)`

```javascript
const { createHAMT } = require('hamt-sharding')
const bucket = createHAMT({...})

await bucket.put('key', 'value')

console.info(await bucket.get('key')) // 'value'
```

### `bucket.del(key)`

```javascript
const { createHAMT } = require('hamt-sharding')
const bucket = createHAMT({...})

await bucket.put('key', 'value')
await bucket.del('key', 'value')

console.info(await bucket.get('key')) // undefined
```

### `bucket.leafCount()`

```javascript
const { createHAMT } = require('hamt-sharding')
const bucket = createHAMT({...})

console.info(bucket.leafCount()) // 0

await bucket.put('key', 'value')

console.info(bucket.leafCount()) // 1
```

### `bucket.childrenCount()`

```javascript
const { createHAMT } = require('hamt-sharding')
const bucket = createHAMT({...})

console.info(bucket.childrenCount()) // 0

await bucket.put('key', 'value')

console.info(bucket.childrenCount()) // 234 -- dependent on hashing algorithm
```

### `bucket.onlyChild()`
### `bucket.eachLeafSeries()`

```javascript
const { createHAMT } = require('hamt-sharding')
const bucket = createHAMT({...})

await bucket.put('key', 'value')

for await (const child of bucket.eachLeafSeries()) {
  console.info(child.value) // 'value'
}
```

### `bucket.serialize(map, reduce)`
### `bucket.asyncTransform(asyncMap, asyncReduce)`
### `bucket.toJSON()`
### `bucket.prettyPrint()`
### `bucket.tableSize()`

## Contribute

Feel free to join in. All welcome. Open an [issue](https://github.com/ipfs-shipyard/js-hamt-sharding/issues)!

This repository falls under the IPFS [Code of Conduct](https://github.com/ipfs/community/blob/master/code-of-conduct.md).

[![](https://cdn.rawgit.com/jbenet/contribute-ipfs-gif/master/img/contribute.gif)](https://github.com/ipfs/community/blob/master/contributing.md)

## License

[MIT](LICENSE)
