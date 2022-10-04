# browser-readablestream-to-it

[![Build status](https://github.com/achingbrain/it/actions/workflows/test.yml/badge.svg?branch=master)](https://github.com/achingbrain/it/actions/workflows/test.yml) [![Coverage Status](https://coveralls.io/repos/github/achingbrain/it/badge.svg?branch=master)](https://coveralls.io/github/achingbrain/it?branch=master) [![Dependencies Status](https://david-dm.org/achingbrain/it/status.svg?path=packages/it-all)](https://david-dm.org/achingbrain/it?path=packages/it-all)

> Turns a browser readable stream into an async iterator

## Install

```sh
$ npm install --save browser-readablestream-to-it
```

## Usage

```javascript
const toIt = require('browser-readablestream-to-it')
const all = require('it-all')

const content = [0, 1, 2, 3, 4]

const stream = new ReadableStream({
  start(controller) {
    for (let i = 0; i < content.length; i++) {
      controller.enqueue(content[i])
    }

    controller.close()
  }
})

const arr = await all(toIt(stream))

console.info(arr) // 0, 1, 2, 3, 4
```

### preventCancel

By default a readable stream will have [.cancel](https://developer.mozilla.org/en-US/docs/Web/API/ReadableStream/cancel) called on it once it has ended or
reading has stopped prematurely.

To prevent this behaviour, pass `preventCancel: true` as an option:

```javascript
const arr = await all(toIt(stream, { preventCancel: true }))

console.info(arr) // 0, 1, 2, 3, 4
```
