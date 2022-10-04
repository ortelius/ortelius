'use strict'

const parallelBatch = require('./')
const test = require('ava')
const all = require('it-all')
const delay = require('delay')

test('Should batch up emitted arrays', async (t) => {
  const input = [
    async () => {
      await delay(200)

      return 1
    },
    async () => {
      await delay(100)

      return 2
    }
  ]

  const res = await all(parallelBatch(input, 2))

  t.deepEqual(res, [1, 2])
})

test('Should batch up emitted arrays in the right order', async (t) => {
  const input = [
    async () => {
      await delay(100)

      return 1
    },
    async () => {
      await delay(200)

      return 2
    },
    async () => {
      await delay(50)

      return 3
    }
  ]

  const res = await all(parallelBatch(input, 2))

  t.deepEqual(res, [1, 2, 3])
})

test('Should propagate errors', async (t) => {
  const error = new Error('wat')

  const input = [
    async () => {
      await delay(100)

      return 1
    },
    async () => {
      await delay(200)

      throw error
    },
    async () => {
      await delay(50)

      return 3
    }
  ]

  try {
    await all(parallelBatch(input, 2))
  } catch (err) {
    t.is(err, error)
  }
})

test('Execute batch in parallel', async (t) => {
  const error = new Error('wat')
  const started = [false, false, false]

  const input = [
    async () => {
      started[0] = true

      await delay(200)

      return 1
    },
    async () => {
      started[1] = true

      await delay(100)

      throw error
    },
    async () => {
      // in second batch, should not execute
      started[2] = true

      await delay(100)

      return 3
    }
  ]

  try {
    await all(parallelBatch(input, 2))
  } catch (err) {
    t.is(err, error)
  }

  t.deepEqual(started, [true, true, false])
})

test('Should work without size parameter', async (t) => {
  const input = [
    async () => {
      await delay(200)

      return 1
    },
    async () => {
      await delay(100)

      return 2
    }
  ]

  const res = await all(parallelBatch(input))

  t.deepEqual(res, [1, 2])
})

test('Should batch up entries with negative batch size', async (t) => {
  const input = [
    async () => {
      await delay(200)

      return 1
    },
    async () => {
      await delay(100)

      return 2
    }
  ]
  const batchSize = -1
  const res = await all(parallelBatch(input, batchSize))

  t.deepEqual(res, [1, 2])
})

test('Should batch up entries with zero batch size', async (t) => {
  const input = [
    async () => {
      await delay(200)

      return 1
    },
    async () => {
      await delay(100)

      return 2
    }
  ]
  const batchSize = 0
  const res = await all(parallelBatch(input, batchSize))

  t.deepEqual(res, [1, 2])
})

test('Should batch up entries with string batch size', async (t) => {
  const input = [
    async () => {
      await delay(200)

      return 1
    },
    async () => {
      await delay(100)

      return 2
    }
  ]
  const batchSize = '2'
  const res = await all(parallelBatch(input, batchSize))

  t.deepEqual(res, [1, 2])
})

test('Should batch up entries with non-integer batch size', async (t) => {
  const input = [
    async () => {
      await delay(200)

      return 1
    },
    async () => {
      await delay(100)

      return 2
    }
  ]
  const batchSize = 2.5
  const res = await all(parallelBatch(input, batchSize))

  t.deepEqual(res, [1, 2])
})

test('Should allow returning errors', async (t) => {
  const input = [
    async () => {
      await delay(200)

      return new Error('herp')
    },
    async () => {
      await delay(100)

      return new Error('derp')
    }
  ]
  const batchSize = 2
  const res = await all(parallelBatch(input, batchSize))

  t.deepEqual(res, [new Error('herp'), new Error('derp')])
})
