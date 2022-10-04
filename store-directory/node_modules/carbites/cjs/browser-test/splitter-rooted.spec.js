'use strict';

var test = require('ava');
var car = require('@ipld/car');
var dagCbor = require('@ipld/dag-cbor');
var blob = require('@web-std/blob');
var _helpers = require('./_helpers.js');
var splitter = require('../lib/rooted/splitter.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

function _interopNamespace(e) {
  if (e && e.__esModule) return e;
  var n = Object.create(null);
  if (e) {
    Object.keys(e).forEach(function (k) {
      if (k !== 'default') {
        var d = Object.getOwnPropertyDescriptor(e, k);
        Object.defineProperty(n, k, d.get ? d : {
          enumerable: true,
          get: function () {
            return e[k];
          }
        });
      }
    });
  }
  n['default'] = e;
  return Object.freeze(n);
}

var test__default = /*#__PURE__*/_interopDefaultLegacy(test);
var dagCbor__namespace = /*#__PURE__*/_interopNamespace(dagCbor);

test__default['default']('root nodes', async t => {
  const targetSize = 500;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const blocks = await _helpers.collect((await car.CarReader.fromBytes(bytes)).blocks());
  const splitter$1 = new splitter.RootedCarSplitter(await car.CarReader.fromBytes(bytes), targetSize);
  const cars = [];
  for await (const car of splitter$1.cars()) {
    cars.push(car);
  }
  t.true(cars.length >= 2);
  const chunkedBlocks = [];
  for (const c of cars) {
    const bs = await _helpers.collect((await car.CarReader.fromIterable(c)).blocks());
    const root = dagCbor__namespace.decode(bs[0].bytes);
    t.true(Array.isArray(root));
    t.is(root.length, 3);
    t.is(root[0], '/carbites/1');
    t.true(Array.isArray(root[1]));
    t.true(Array.isArray(root[2]));
    chunkedBlocks.push(...bs.slice(1));
  }
  t.is(blocks.length, chunkedBlocks.length);
});
test__default['default']('fromBlob', async t => {
  const targetSize = 500;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const blocks = await _helpers.collect((await car.CarReader.fromBytes(bytes)).blocks());
  const splitter$1 = await splitter.RootedCarSplitter.fromBlob(new blob.Blob([bytes]), targetSize);
  const cars = [];
  for await (const car of splitter$1.cars()) {
    cars.push(car);
  }
  t.true(cars.length >= 2);
  const chunkedBlocks = [];
  for (const c of cars) {
    const bs = await _helpers.collect((await car.CarReader.fromIterable(c)).blocks());
    const root = dagCbor__namespace.decode(bs[0].bytes);
    t.true(Array.isArray(root));
    t.is(root.length, 3);
    t.is(root[0], '/carbites/1');
    t.true(Array.isArray(root[1]));
    t.true(Array.isArray(root[2]));
    chunkedBlocks.push(...bs.slice(1));
  }
  t.is(blocks.length, chunkedBlocks.length);
});
