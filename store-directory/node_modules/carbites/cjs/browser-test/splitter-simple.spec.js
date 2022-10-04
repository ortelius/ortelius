'use strict';

var test = require('ava');
var car = require('@ipld/car');
var cid = require('multiformats/cid');
var blob = require('@web-std/blob');
var _helpers = require('./_helpers.js');
var splitter = require('../lib/simple/splitter.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var test__default = /*#__PURE__*/_interopDefaultLegacy(test);

const empty = cid.CID.parse('bafkqaaa');
test__default['default']('split in ~two', async t => {
  const targetSize = 500;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const blocks = await _helpers.collect((await car.CarReader.fromBytes(bytes)).blocks());
  const splitter$1 = new splitter.SimpleCarSplitter(await car.CarReader.fromBytes(bytes), targetSize);
  const cars = [];
  for await (const car of splitter$1.cars()) {
    cars.push(car);
  }
  t.true(cars.length >= 2);
  const chunkedBlocks = [];
  for (const c of cars) {
    const bs = await _helpers.collect((await car.CarReader.fromIterable(c)).blocks());
    chunkedBlocks.push(...bs);
  }
  t.is(blocks.length, chunkedBlocks.length);
});
test__default['default']('target size bigger than file size', async t => {
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const splitter$1 = new splitter.SimpleCarSplitter(await car.CarReader.fromBytes(bytes), Infinity);
  const cars = [];
  for await (const car of splitter$1.cars()) {
    cars.push(car);
  }
  t.is(cars.length, 1);
  t.true(_helpers.bytesEqual(bytes, await _helpers.collectBytes(cars[0])));
});
test__default['default']('roots in first CAR, empty CID root in other CARs', async t => {
  const targetSize = 100;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const splitter$1 = new splitter.SimpleCarSplitter(await car.CarReader.fromBytes(bytes), targetSize);
  let i = 0;
  for await (const car$1 of splitter$1.cars()) {
    const reader = await car.CarReader.fromIterable(car$1);
    const roots = await reader.getRoots();
    if (i === 0) {
      t.true(roots.length > 0);
    } else {
      t.is(roots.length, 1);
      t.true(empty.equals(roots[0]));
    }
    i++;
  }
  t.true(i > 1);
});
test__default['default']('bad target size', t => {
  t.throws(() => new splitter.SimpleCarSplitter(null, -1));
});
test__default['default']('split CARs are RootsReader', async t => {
  const targetSize = 500;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const reader = await car.CarReader.fromBytes(bytes);
  const splitter$1 = new splitter.SimpleCarSplitter(reader, targetSize);
  const splitRoots = [];
  for await (const car of splitter$1.cars()) {
    const roots = await car.getRoots();
    splitRoots.push(...roots);
  }
  const originRoots = await reader.getRoots();
  for (const [i, r] of originRoots.entries()) {
    t.true(r.equals(splitRoots[i]));
  }
  splitRoots.slice(originRoots.length).forEach(r => {
    t.true(r.equals(empty));
  });
});
test__default['default']('fromBlob', async t => {
  const targetSize = 500;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const blocks = await _helpers.collect((await car.CarReader.fromBytes(bytes)).blocks());
  const splitter$1 = await splitter.SimpleCarSplitter.fromBlob(new blob.Blob([bytes]), targetSize);
  const cars = [];
  for await (const car of splitter$1.cars()) {
    cars.push(car);
  }
  t.true(cars.length >= 2);
  const chunkedBlocks = [];
  for (const c of cars) {
    const bs = await _helpers.collect((await car.CarReader.fromIterable(c)).blocks());
    chunkedBlocks.push(...bs);
  }
  t.is(blocks.length, chunkedBlocks.length);
});
test__default['default']('fromIterable', async t => {
  const targetSize = 500;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const blocks = await _helpers.collect((await car.CarReader.fromBytes(bytes)).blocks());
  const splitter$1 = await splitter.SimpleCarSplitter.fromIterable(_helpers.toAsyncIterable([bytes]), targetSize);
  const cars = [];
  for await (const car of splitter$1.cars()) {
    cars.push(car);
  }
  t.true(cars.length >= 2);
  const chunkedBlocks = [];
  for (const c of cars) {
    const bs = await _helpers.collect((await car.CarReader.fromIterable(c)).blocks());
    chunkedBlocks.push(...bs);
  }
  t.is(blocks.length, chunkedBlocks.length);
});
