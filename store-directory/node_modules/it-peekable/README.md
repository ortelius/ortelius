# it-peekable

[![Build status](https://github.com/achingbrain/it/actions/workflows/test.yml/badge.svg?branch=master)](https://github.com/achingbrain/it/actions/workflows/test.yml) [![Coverage Status](https://coveralls.io/repos/github/achingbrain/it/badge.svg?branch=master)](https://coveralls.io/github/achingbrain/it?branch=master) [![Dependencies Status](https://david-dm.org/achingbrain/it/status.svg?path=packages/it-reduce)](https://david-dm.org/achingbrain/it?path=packages/it-reduce)

> A peekable async iterator

Lets you look at the contents of an async iterator and decide what to do

## Install

```sh
$ npm install --save it-peekable
```

## Usage

```javascript
const peekable = require('it-peekable')

// This can also be an iterator, async iterator, generator, etc
const values = [0, 1, 2, 3, 4]

const it = peekable(value)

const first = await it.peek()

console.info(first) // 0

it.push(first)

console.info([...it])
// [ 0, 1, 2, 3, 4 ]
```
