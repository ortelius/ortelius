import test from 'ava';
import { CarReader } from '@ipld/car';
import { CID } from 'multiformats/cid';
import { Blob } from '@web-std/blob';
import {
  bytesEqual,
  collect,
  collectBytes,
  randomCar,
  toAsyncIterable
} from './_helpers.js';
import { SimpleCarSplitter } from '../lib/simple/splitter.js';
const empty = CID.parse('bafkqaaa');
test('split in ~two', async t => {
  const targetSize = 500;
  const bytes = await collectBytes(await randomCar(1000));
  const blocks = await collect((await CarReader.fromBytes(bytes)).blocks());
  const splitter = new SimpleCarSplitter(await CarReader.fromBytes(bytes), targetSize);
  const cars = [];
  for await (const car of splitter.cars()) {
    cars.push(car);
  }
  t.true(cars.length >= 2);
  const chunkedBlocks = [];
  for (const c of cars) {
    const bs = await collect((await CarReader.fromIterable(c)).blocks());
    chunkedBlocks.push(...bs);
  }
  t.is(blocks.length, chunkedBlocks.length);
});
test('target size bigger than file size', async t => {
  const bytes = await collectBytes(await randomCar(1000));
  const splitter = new SimpleCarSplitter(await CarReader.fromBytes(bytes), Infinity);
  const cars = [];
  for await (const car of splitter.cars()) {
    cars.push(car);
  }
  t.is(cars.length, 1);
  t.true(bytesEqual(bytes, await collectBytes(cars[0])));
});
test('roots in first CAR, empty CID root in other CARs', async t => {
  const targetSize = 100;
  const bytes = await collectBytes(await randomCar(1000));
  const splitter = new SimpleCarSplitter(await CarReader.fromBytes(bytes), targetSize);
  let i = 0;
  for await (const car of splitter.cars()) {
    const reader = await CarReader.fromIterable(car);
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
test('bad target size', t => {
  t.throws(() => new SimpleCarSplitter(null, -1));
});
test('split CARs are RootsReader', async t => {
  const targetSize = 500;
  const bytes = await collectBytes(await randomCar(1000));
  const reader = await CarReader.fromBytes(bytes);
  const splitter = new SimpleCarSplitter(reader, targetSize);
  const splitRoots = [];
  for await (const car of splitter.cars()) {
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
test('fromBlob', async t => {
  const targetSize = 500;
  const bytes = await collectBytes(await randomCar(1000));
  const blocks = await collect((await CarReader.fromBytes(bytes)).blocks());
  const splitter = await SimpleCarSplitter.fromBlob(new Blob([bytes]), targetSize);
  const cars = [];
  for await (const car of splitter.cars()) {
    cars.push(car);
  }
  t.true(cars.length >= 2);
  const chunkedBlocks = [];
  for (const c of cars) {
    const bs = await collect((await CarReader.fromIterable(c)).blocks());
    chunkedBlocks.push(...bs);
  }
  t.is(blocks.length, chunkedBlocks.length);
});
test('fromIterable', async t => {
  const targetSize = 500;
  const bytes = await collectBytes(await randomCar(1000));
  const blocks = await collect((await CarReader.fromBytes(bytes)).blocks());
  const splitter = await SimpleCarSplitter.fromIterable(toAsyncIterable([bytes]), targetSize);
  const cars = [];
  for await (const car of splitter.cars()) {
    cars.push(car);
  }
  t.true(cars.length >= 2);
  const chunkedBlocks = [];
  for (const c of cars) {
    const bs = await collect((await CarReader.fromIterable(c)).blocks());
    chunkedBlocks.push(...bs);
  }
  t.is(blocks.length, chunkedBlocks.length);
});