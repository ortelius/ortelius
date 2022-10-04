'use strict';

var test = require('ava');
var car = require('@ipld/car');
var blob = require('@web-std/blob');
var block = require('multiformats/block');
var sha2 = require('multiformats/hashes/sha2');
var cid = require('multiformats/cid');
var dagCbor = require('@ipld/dag-cbor');
var _helpers = require('./_helpers.js');
var splitter = require('../lib/treewalk/splitter.js');

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

const newBlock = async data => {
  const bytes = dagCbor__namespace.encode(data);
  const hash = await sha2.sha256.digest(bytes);
  const cid$1 = cid.CID.create(1, dagCbor__namespace.code, hash);
  return new block.Block({
    cid: cid$1,
    bytes,
    value: data
  });
};
test__default['default']('split in ~two', async t => {
  const targetSize = 500;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const blocks = await _helpers.collect((await car.CarReader.fromBytes(bytes)).blocks());
  const splitter$1 = new splitter.TreewalkCarSplitter(await car.CarReader.fromBytes(bytes), targetSize);
  const cars = [];
  for await (const car of splitter$1.cars()) {
    cars.push(car);
  }
  t.true(cars.length >= 2, `${ cars.length } >= 2`);
  const chunkedBlocks = new Map();
  for (const c of cars) {
    const reader = await car.CarReader.fromIterable(c);
    for await (const b of reader.blocks()) {
      chunkedBlocks.set(b.cid.toString(), b);
    }
  }
  t.is(blocks.length, chunkedBlocks.size);
});
test__default['default']('target size bigger than file size', async t => {
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const splitter$1 = new splitter.TreewalkCarSplitter(await car.CarReader.fromBytes(bytes), Infinity);
  const cars = [];
  for await (const car of splitter$1.cars()) {
    cars.push(car);
  }
  t.is(cars.length, 1);
  t.true(_helpers.bytesEqual(bytes, await _helpers.collectBytes(cars[0])));
});
test__default['default']('same root in every CAR', async t => {
  const targetSize = 100;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const reader = await car.CarReader.fromBytes(bytes);
  const [rootCid] = await reader.getRoots();
  const splitter$1 = new splitter.TreewalkCarSplitter(reader, targetSize);
  for await (const car$1 of splitter$1.cars()) {
    const reader = await car.CarReader.fromIterable(car$1);
    const roots = await reader.getRoots();
    t.is(roots.length, 1);
    t.true(rootCid.equals(roots[0]));
  }
});
test__default['default']('bad target size', t => {
  t.throws(() => new splitter.TreewalkCarSplitter(null, -1));
});
test__default['default']('fromBlob', async t => {
  const targetSize = 500;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const blocks = await _helpers.collect((await car.CarReader.fromBytes(bytes)).blocks());
  const splitter$1 = await splitter.TreewalkCarSplitter.fromBlob(new blob.Blob([bytes]), targetSize);
  const cars = [];
  for await (const car of splitter$1.cars()) {
    cars.push(car);
  }
  t.true(cars.length >= 2);
  const chunkedBlocks = new Map();
  for (const c of cars) {
    const reader = await car.CarReader.fromIterable(c);
    for await (const b of reader.blocks()) {
      chunkedBlocks.set(b.cid.toString(), b);
    }
  }
  t.is(blocks.length, chunkedBlocks.size);
});
test__default['default']('fromIterable', async t => {
  const targetSize = 500;
  const bytes = await _helpers.collectBytes(await _helpers.randomCar(1000));
  const blocks = await _helpers.collect((await car.CarReader.fromBytes(bytes)).blocks());
  const splitter$1 = await splitter.TreewalkCarSplitter.fromIterable(_helpers.toAsyncIterable([bytes]), targetSize);
  const cars = [];
  for await (const car of splitter$1.cars()) {
    cars.push(car);
  }
  t.true(cars.length >= 2);
  const chunkedBlocks = new Map();
  for (const c of cars) {
    const reader = await car.CarReader.fromIterable(c);
    for await (const b of reader.blocks()) {
      chunkedBlocks.set(b.cid.toString(), b);
    }
  }
  t.is(blocks.length, chunkedBlocks.size);
});
test__default['default']('zero roots', async t => {
  const targetSize = 500;
  const channel = car.CarWriter.create([]);
  channel.writer.put(await newBlock(Math.random()));
  channel.writer.close();
  const splitter$1 = await splitter.TreewalkCarSplitter.fromIterable(channel.out, targetSize);
  await t.throwsAsync(() => splitter$1.cars().next(), { message: 'unexpected number of roots: 0' });
});
test__default['default']('multiple roots', async t => {
  const targetSize = 500;
  const blocks = await Promise.all([
    newBlock('0:' + Math.random()),
    newBlock('1:' + Math.random()),
    newBlock('2:' + Math.random())
  ]);
  const channel = car.CarWriter.create(blocks.map(b => b.cid));
  blocks.forEach(b => channel.writer.put(b));
  channel.writer.close();
  const splitter$1 = await splitter.TreewalkCarSplitter.fromIterable(channel.out, targetSize);
  await t.throwsAsync(() => splitter$1.cars().next(), { message: `unexpected number of roots: ${ blocks.length }` });
});
