# it-parallel-batch

[![Build status](https://github.com/achingbrain/it/actions/workflows/test.yml/badge.svg?branch=master)](https://github.com/achingbrain/it/actions/workflows/test.yml) [![Coverage Status](https://coveralls.io/repos/github/achingbrain/it/badge.svg?branch=master)](https://coveralls.io/github/achingbrain/it?branch=master) [![Dependencies Status](https://david-dm.org/achingbrain/it/status.svg?path=packages/it-parallel-batch)](https://david-dm.org/achingbrain/it?path=packages/it-parallel-batch)

> Takes an (async) iterable that emits promise-returning functions, invokes them in parallel and emits the results as they become available but in the same order as the input

The final batch may be smaller than the batch size.

## Install

```sh
$ npm install --save it-parallel-batch
```

## Usage

```javascript
const parallelBatch = require('it-parallel-batch')
const all = require('it-all')
const delay = require('delay')

// This can also be an iterator, async iterator, generator, etc
const input = [
  async () => {
    await delay(500)

    return 1
  },
  async () => {
    await delay(200)

    return 2
  },
  async () => {
    await delay(100)

    return 3
  }
]

const batchSize = 2

const result = await all(parallelBatch(input, batchSize))

console.info(result) // [1, 2, 3]
```
