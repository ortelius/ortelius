
# parse-duration [![Build Status](https://travis-ci.org/jkroso/parse-duration.svg?branch=master)](https://travis-ci.org/jkroso/parse-duration)

  convert a human readable duration to ms

## Installation

[![NPM](https://nodei.co/npm/parse-duration.png?mini=true)](https://npmjs.org/package/parse-duration)

then in your app:

```js
import parse from 'parse-duration'
```

or CommonJS:

```js
var parse = require('parse-duration')
```

## API

### parse(str, format='ms')

Convert `str` to ms

```js
var ns = parse('1ns') // => 1 / 1e6
var μs = parse('1μs') // => 1 / 1000
var ms = parse('1ms') // => 1
var s = parse('1s')   // => ms * 1000
var m = parse('1m')   // => s * 60
var h = parse('1h')   // => m * 60
var d = parse('1d')   // => h * 24
var w = parse('1w')   // => d * 7
var y = parse('1y')   // => d * 365.25
```

It can also handle basic compound expressions

```js
parse('1hr 20mins') // => 1 * h + 20 * m
```

whitespace

```js
parse('1 hr 20 mins') // => 1 * h + 20 * m
```

comma seperated numbers

```js
parse('27,681 ns') // => 27681 * ns
```

And most other types of noise

```js
parse('running length: 1hour:20mins') // => 1 * h + 20 * m
```

You can even use negatives

```js
parse('2hr -40mins') // => 1 * h + 20 * m
```

And exponents

```js
parse('2e3s') // => 2000 * s
```

#### Available unit types are:

- nanoseconds (ns)
- microseconds (μs)
- milliseconds (ms)
- seconds (s, sec)
- minutes (m, min)
- hours (h, hr)
- days (d)
- weeks (w, wk)
- months
- years (y, yr)

And its easy to add more, including unicode:

```js
parse['сек'] = parse['sec']
parse('5сек') // => 5000
```

The output format can also be defined

```js
parse('1hr 20mins', 'm') // => 80
```
