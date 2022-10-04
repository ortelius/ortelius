# it-first

[![Build status](https://github.com/achingbrain/it/actions/workflows/test.yml/badge.svg?branch=master)](https://github.com/achingbrain/it/actions/workflows/test.yml) [![Coverage Status](https://coveralls.io/repos/github/achingbrain/it/badge.svg?branch=master)](https://coveralls.io/github/achingbrain/it?branch=master) [![Dependencies Status](https://david-dm.org/achingbrain/it/status.svg?path=packages/it-first)](https://david-dm.org/achingbrain/it?path=packages/it-first)

> Returns the first result from an (async) iterable.

Mostly useful for tests.

## Install

```sh
$ npm install --save it-first
```

## Usage

```javascript
const first = require('it-first')

// This can also be an iterator, async iterator, generator, etc
const values = [0, 1, 2, 3, 4]

const res = await first(values)

console.info(res) // 0
```
