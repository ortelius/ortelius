# blob-to-it

[![Build status](https://github.com/achingbrain/it/actions/workflows/test.yml/badge.svg?branch=master)](https://github.com/achingbrain/it/actions/workflows/test.yml) [![Coverage Status](https://coveralls.io/repos/github/achingbrain/it/badge.svg?branch=master)](https://coveralls.io/github/achingbrain/it?branch=master) [![Dependencies Status](https://david-dm.org/achingbrain/it/status.svg?path=packages/it-all)](https://david-dm.org/achingbrain/it?path=packages/it-all)

> Turns a Blob into an async iterator

## Install

```sh
$ npm install --save blob-to-it
```

## Usage

```javascript
const toIt = require('blob-to-it')
const all = require('it-all')

const content = [ Uint8Array.from([0, 1, 2, 3, 4]) ]
const blob = new Blob(content)
const arr = await all(toIt(blob))

console.info(arr) // [ [ 0, 1, 2, 3, 4 ] ]
```
