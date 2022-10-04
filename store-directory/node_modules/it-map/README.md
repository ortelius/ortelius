
# it-map

[![Build status](https://github.com/achingbrain/it/actions/workflows/test.yml/badge.svg?branch=master)](https://github.com/achingbrain/it/actions/workflows/test.yml) [![Coverage Status](https://coveralls.io/repos/github/achingbrain/it/badge.svg?branch=master)](https://coveralls.io/github/achingbrain/it?branch=master) [![Dependencies Status](https://david-dm.org/achingbrain/it/status.svg?path=packages/it-map)](https://david-dm.org/achingbrain/it?path=packages/it-map)

> Maps the values yielded by an (async) iterator

## Install

```sh
$ npm install --save it-map
```

## Usage

```javascript
const map = require('it-map')

// This can also be an iterator, async iterator, generator, etc
const values = [0, 1, 2, 3, 4]

const result = await map(values, (val) => val++)

console.info(result) // 15
```
