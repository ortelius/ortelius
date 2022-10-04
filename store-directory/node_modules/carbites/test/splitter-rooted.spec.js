import test from 'ava'
import { CarReader } from '@ipld/car'
import * as dagCbor from '@ipld/dag-cbor'
import { Blob } from '@web-std/blob'
import { collect, collectBytes, randomCar } from './_helpers.js'
import { RootedCarSplitter } from '../lib/rooted/splitter.js'

test('root nodes', async t => {
  const targetSize = 500
  const bytes = await collectBytes(await randomCar(1000))
  const blocks = await collect((await CarReader.fromBytes(bytes)).blocks())
  const splitter = new RootedCarSplitter(await CarReader.fromBytes(bytes), targetSize)
  const cars = []
  for await (const car of splitter.cars()) {
    cars.push(car)
  }
  t.true(cars.length >= 2)
  const chunkedBlocks = []
  for (const c of cars) {
    const bs = await collect((await CarReader.fromIterable(c)).blocks())
    const root = dagCbor.decode(bs[0].bytes)
    t.true(Array.isArray(root))
    t.is(root.length, 3)
    t.is(root[0], '/carbites/1')
    t.true(Array.isArray(root[1]))
    t.true(Array.isArray(root[2]))
    chunkedBlocks.push(...bs.slice(1))
  }
  t.is(blocks.length, chunkedBlocks.length)
})

test('fromBlob', async t => {
  const targetSize = 500
  const bytes = await collectBytes(await randomCar(1000))
  const blocks = await collect((await CarReader.fromBytes(bytes)).blocks())
  const splitter = await RootedCarSplitter.fromBlob(new Blob([bytes]), targetSize)
  const cars = []
  for await (const car of splitter.cars()) {
    cars.push(car)
  }
  t.true(cars.length >= 2)
  const chunkedBlocks = []
  for (const c of cars) {
    const bs = await collect((await CarReader.fromIterable(c)).blocks())
    const root = dagCbor.decode(bs[0].bytes)
    t.true(Array.isArray(root))
    t.is(root.length, 3)
    t.is(root[0], '/carbites/1')
    t.true(Array.isArray(root[1]))
    t.true(Array.isArray(root[2]))
    chunkedBlocks.push(...bs.slice(1))
  }
  t.is(blocks.length, chunkedBlocks.length)
})
