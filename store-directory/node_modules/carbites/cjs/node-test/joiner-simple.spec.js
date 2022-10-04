'use strict';

var test = require('ava');
var car = require('@ipld/car');
var _helpers = require('./_helpers.js');
require('../lib/simple/index.js');
var splitter = require('../lib/simple/splitter.js');
var joiner = require('../lib/simple/joiner.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var test__default = /*#__PURE__*/_interopDefaultLegacy(test);

test__default['default']('joins CARs', async t => {
  const targetSize = 500;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const reader = await car.CarReader.fromBytes(bytes);
  const splitter$1 = new splitter.SimpleCarSplitter(reader, targetSize);
  const cars = [];
  for await (const car$1 of splitter$1.cars()) {
    cars.push(await car.CarReader.fromIterable(car$1));
  }
  const joiner$1 = new joiner.SimpleCarJoiner(cars);
  t.true(_helpers.bytesEqual(await _helpers.collectBytes(joiner$1.car()), bytes));
});
