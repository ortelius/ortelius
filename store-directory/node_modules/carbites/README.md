# carbites

[![Build](https://github.com/nftstorage/carbites/actions/workflows/main.yml/badge.svg)](https://github.com/nftstorage/carbites/actions/workflows/main.yml)
[![dependencies Status](https://status.david-dm.org/gh/nftstorage/carbites.svg)](https://david-dm.org/nftstorage/carbites)
[![JavaScript Style Guide](https://img.shields.io/badge/code_style-standard-brightgreen.svg)](https://standardjs.com)
[![npm bundle size](https://img.shields.io/bundlephobia/minzip/carbites)](https://bundlephobia.com/package/carbites)

Chunking for [CAR files](https://github.com/ipld/specs/blob/master/block-layer/content-addressable-archives.md). Split a single CAR into multiple CARs.

## Install

```sh
npm install carbites
```

## Usage

Carbites supports 3 different strategies:

1. [**Simple**](#simple) (default) - fast but naive, only the first CAR output has a root CID, subsequent CARs have a placeholder "empty" CID.
2. [**Rooted**](#rooted) - like simple, but creates a custom root node to ensure all blocks in a CAR are referenced.
3. [**Treewalk**](#treewalk) - walks the DAG to pack sub-graphs into each CAR file that is output. Every CAR has the same root CID, but contains a different portion of the DAG.

### Simple

```js
import { CarSplitter } from 'carbites'
import { CarReader } from '@ipld/car'
import fs from 'fs'

const bigCar = await CarReader.fromIterable(fs.createReadStream('/path/to/big.car'))
const targetSize = 1024 * 1024 * 100 // chunk to ~100MB CARs
const splitter = new CarSplitter(bigCar, targetSize) // (simple strategy)

for await (const car of splitter.cars()) {
  // Each `car` is an AsyncIterable<Uint8Array>
}
```

⚠️ Note: The first CAR output has roots in the header, subsequent CARs have an empty root CID [`bafkqaaa`](https://cid.ipfs.io/#bafkqaaa) as [recommended](https://github.com/ipld/specs/blob/master/block-layer/content-addressable-archives.md#number-of-roots).

### Rooted

Instead of an empty CID, carbites can generate a special root node for each split CAR that references all the blocks _and_ the original roots (only in the first CAR). To do this, use the `RootedCarSplitter` constructor. When reading/extracting data from the CARs, the root node should be discarded.

<details>
  <summary>Example</summary>

  ```js
  import { RootedCarSplitter } from 'carbites/rooted'
  import { CarReader } from '@ipld/car/reader'
  import * as dagCbor from '@ipld/dag-cbor'
  import fs from 'fs'

  const bigCar = await CarReader.fromIterable(fs.createReadStream('/path/to/big.car'))
  const targetSize = 1024 * 1024 * 100 // chunk to ~100MB CARs
  const splitter = new RootedCarSplitter(bigCar, targetSize)

  const cars = splitter.cars()

  // Every CAR has a single root - a CBOR node that is an tuple of `/carbites/1`,
  // an array of root CIDs and an array of block CIDs.
  // e.g. ['/carbites/1', ['bafkroot'], ['bafy1', 'bafy2']]

  const { done, value: car } = await cars.next()
  const reader = await CarReader.fromIterable(car)
  const rootCids = await reader.getRoots()
  const rootNode = dagCbor.decode(await reader.get(rootCids[0]))

  console.log(rootNode[0]) // /carbites/1
  console.log(rootNode[1]) // Root CIDs (only in first CAR)
  /*
  [
    CID(bafybeictvyf6polqzgop3jt32owubfmsg3kl226omqrfte4eyidubc4rpq)
  ]
  */
  console.log(rootNode[2]) // Block CIDs (all blocks in this CAR)
  /*
  [
    CID(bafybeictvyf6polqzgop3jt32owubfmsg3kl226omqrfte4eyidubc4rpq),
    CID(bafyreihcsxqhd6agqpboc3wrlvpy5bwuxctv5upicdnt3u2wojv4exxl24),
    CID(bafyreiasq7d2ihbqm5xvhjjzlmzsensuadrpmpt2tkjsuwq42xpa34qevu)
  ]
  */
  ```

</details>

The root node is limited to 4MB in size (the largest message IPFS will bitswap). Depending on the settings used to construct the DAG in the CAR, this may mean a split CAR size limit of around 30GiB.

### Treewalk

Every CAR file has the _same_ root CID but a different portion of the DAG. The DAG is traversed from the root node and each block is decoded and links extracted in order to determine which sub-graph to include in each CAR.

<details>
  <summary>Example</summary>

  ```js
  import { TreewalkCarSplitter } from 'carbites/treewalk'
  import { CarReader } from '@ipld/car/reader'
  import * as dagCbor from '@ipld/dag-cbor'
  import fs from 'fs'

  const bigCar = await CarReader.fromIterable(fs.createReadStream('/path/to/big.car'))
  const [rootCid] = await bigCar.getRoots()
  const targetSize = 1024 * 1024 * 100 // chunk to ~100MB CARs
  const splitter = new TreewalkCarSplitter(bigCar, targetSize)

  for await (const car of splitter.cars()) {
    // Each `car` is an AsyncIterable<Uint8Array>
    const reader = await CarReader.fromIterable(car)
    const [splitCarRootCid] = await reader.getRoots()
    console.assert(rootCid.equals(splitCarRootCid)) // all cars will have the same root
  }
  ```

</details>

### CLI

Install the CLI tool to use Carbites from the comfort of your terminal:

```sh
npm i -g carbites-cli

# Split a big CAR into many smaller CARs
carbites split big.car --size 100MB --strategy simple # (default size & strategy)

# Join many split CARs back into a single CAR.
carbites join big-0.car big-1.car ...
# Note: not a tool for joining arbitrary CARs together! The split CARs MUST
# belong to the same CAR and big-0.car should be the first argument.
```

## API

* [`class CarSplitter`](#class-carsplitter)
    * [`constructor(car: AsyncIterable<Uint8Array>, targetSize: number)`](#constructorcar-asynciterableuint8array-targetsize-number)
    * [`cars(): AsyncGenerator<AsyncIterable<Uint8Array> & RootsReader>`](#cars-asyncgeneratorasynciterableuint8array--rootsreader)
    * [`static async fromBlob(blob: Blob, targetSize: number): CarSplitter`](#static-async-fromblobblob-blob-targetsize-number-carsplitter)
    * [`static async fromIterable(iterable: AsyncIterable<Uint8Array>, targetSize: number): CarSplitter`](#static-async-fromiterableiterable-asynciterableuint8array-targetsize-number-carsplitter)
* [`class CarJoiner`](#class-carjoiner)
    * [`constructor(cars: Iterable<AsyncIterable<Uint8Array>>)`](#constructorcars-iterableasynciterableuint8array)
    * [`car(): AsyncGenerator<Uint8Array>`](#car-asyncgeneratoruint8array)
* [`class RootedCarSplitter`](#class-rootedcarsplitter)
* [`class RootedCarJoiner`](#class-rootedcarjoiner)
* [`class TreewalkCarSplitter`](#class-treewalkcarsplitter)
* [`class TreewalkCarJoiner`](#class-treewalkcarjoiner)

### `class CarSplitter`

Split a CAR file into several smaller CAR files.

Import in the browser:

```js
import { CarSplitter } from 'https://cdn.skypack.dev/carbites'
```

Import in Node.js:

```js
import { CarSplitter } from 'carbites'
```

Note: This is an alias of `SimpleCarSplitter` - the default strategy for splitting CARs.

#### `constructor(car: CarReader, targetSize: number)`

Create a new `CarSplitter` for the passed CAR file, aiming to generate CARs of around `targetSize` bytes in size.

#### `cars(): AsyncGenerator<AsyncIterable<Uint8Array> & RootsReader>`

Split the CAR file and create multiple smaller CAR files. Returns an `AsyncGenerator` that yields the split CAR files (of type `AsyncIterable<Uint8Array>`).

The CAR files output also implement the [`RootsReader`](https://github.com/ipld/js-car/blob/8c74dc3c7273213b83f4610e4f88cf1ad2830fa6/api.ts#L18-L21) interface from `@ipld/car` which means you can call `getRoots(): Promise<CID[]>` to obtain the root CIDs.

#### `static async fromBlob(blob: Blob, targetSize: number): CarSplitter`

Convenience function to create a new `CarSplitter` from a [blob](https://developer.mozilla.org/en-US/docs/Web/API/Blob) of CAR file data.

#### `static async fromIterable(iterable: AsyncIterable<Uint8Array>, targetSize: number): CarSplitter`

Convenience function to create a new `CarSplitter` from an `AsyncIterable<Uint8Array>` of CAR file data.

### `class CarJoiner`

Join together split CAR files into a single big CAR.

Import in the browser:

```js
import { CarJoiner } from 'https://cdn.skypack.dev/carbites'
```

Import in Node.js:

```js
import { CarJoiner } from 'carbites'
```

Note: This is an alias of `SimpleCarJoiner` - a joiner for the the default CAR splitting strategy.

#### `constructor(cars: Iterable<CarReader>)`

Create a new `CarJoiner`  for joining the passed CAR files together.

#### `car(): AsyncGenerator<Uint8Array>`

Join the CAR files together and return the joined CAR.

### `class RootedCarSplitter`

Split a CAR file into several smaller CAR files ensuring every CAR file contains a single root node that references all the blocks _and_ the original roots (only in the first CAR). When reading/extracting data from the CARs, the root node should be discarded.

Import in the browser:

```js
import { RootedCarSplitter } from 'https://cdn.skypack.dev/carbites/rooted'
```

Import in Node.js:

```js
import { RootedCarSplitter } from 'carbites/rooted'
```

The API is the same as for [`CarSplitter`](#class-carsplitter).

#### Root Node Format

The root node is a `dag-cbor` node that is a tuple of the string `/carbites/1`, an array of root CIDs (only seen in first CAR) and an array of block CIDs (all the blocks in the CAR). e.g. `['/carbites/1', ['bafkroot'], ['bafy1', 'bafy2']]`.

Note: The root node is limited to 4MB in size (the largest message IPFS will bitswap). Depending on the settings used to construct the DAG in the CAR, this may mean a split CAR size limit of around 30GiB.

### `class RootedCarJoiner`

Join together CAR files that were split using [`RootedCarSplitter`](#class-rootedcarsplitter).

The API is the same as for [`CarJoiner`](#class-carjoiner).

### `class TreewalkCarSplitter`

Split a CAR file into several smaller CAR files. Every CAR file has the _same_ root CID but a different portion of the DAG. The DAG is traversed from the root node and each block is decoded and links extracted in order to determine which sub-graph to include in each CAR.

Import in the browser:

```js
import { TreewalkCarSplitter } from 'https://cdn.skypack.dev/carbites/treewalk'
```

Import in Node.js:

```js
import { TreewalkCarSplitter } from 'carbites/treewalk'
```

The API is the same as for [`CarSplitter`](#class-carsplitter).

### `class TreewalkCarJoiner`

Join together CAR files that were split using [`TreewalkCarSplitter`](#class-treewalkcarsplitter).

The API is the same as for [`CarJoiner`](#class-carjoiner).

## Releasing

You can publish by either running `npm publish` in the `dist` directory or using `npx ipjs publish`.

## Contribute

Feel free to dive in! [Open an issue](https://github.com/nftstorage/carbites/issues/new) or submit PRs.

## License

Dual-licensed under [MIT](https://github.com/nftstorage/carbites/blob/main/LICENSE-MIT) + [Apache 2.0](https://github.com/nftstorage/carbites/blob/main/LICENSE-APACHE)
