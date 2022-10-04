import test from 'ava'
import { CarReader } from '@ipld/car'
import { bytesEqual, collectBytes, randomCar } from './_helpers.js'
import { RootedCarSplitter, RootedCarJoiner } from '../lib/rooted/index.js'

test('joins CARs', async t => {
  const targetSize = 500
  const bytes = await collectBytes(await randomCar(1000))
  const reader = await CarReader.fromBytes(bytes)
  const splitter = new RootedCarSplitter(reader, targetSize)
  const cars = []
  for await (const car of splitter.cars()) {
    cars.push(await CarReader.fromIterable(car))
  }
  const joiner = new RootedCarJoiner(cars)
  t.true(bytesEqual(await collectBytes(joiner.car()), bytes))
})
