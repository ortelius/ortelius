# it-glob

[![Build status](https://github.com/achingbrain/it/actions/workflows/test.yml/badge.svg?branch=master)](https://github.com/achingbrain/it/actions/workflows/test.yml) [![Coverage Status](https://coveralls.io/repos/github/achingbrain/it/badge.svg?branch=master)](https://coveralls.io/github/achingbrain/it?branch=master) [![Dependencies Status](https://david-dm.org/achingbrain/it/status.svg?path=packages/it-glob)](https://david-dm.org/achingbrain/it?path=packages/it-glob)

> Async iterable filename pattern matcher

Like [`glob`](https://npmjs.com/package/glob) but async iterable.

## Installation

```console
$ npm install --save it-glob
```

## Usage

```javascript
const glob = require('it-glob')

const options = {
  cwd // defaults to process.cwd
  absolute // return absolute paths, defaults to false
  nodir // only yield file paths, skip directories

  // all other options are passed to minimatch
}

for await (const path of glob('/path/to/file', '**/*', options)) {
  console.info(path)
}
```

See the [minimatch docs](https://www.npmjs.com/package/minimatch#options) for the full list of options.
