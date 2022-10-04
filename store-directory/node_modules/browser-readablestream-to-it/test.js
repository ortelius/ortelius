/* eslint-env mocha, browser */

'use strict'

const all = require('it-all')
const { expect } = require('chai')
const toIt = require('./index')

describe('browser-readablestream-to-it', () => {
  it('should convert a readablestream to an async iterator', async () => {
    const content = [0, 1, 2, 3, 4]

    const stream = new ReadableStream({
      start (controller) {
        for (let i = 0; i < content.length; i++) {
          controller.enqueue(content[i])
        }

        controller.close()
      }
    })

    const it = toIt(stream)
    expect(stream.locked).to.equal(false)

    const result = await all(it)

    expect(result).to.deep.equal(content)
    expect(stream.locked).to.equal(false)
  })

  it('should release lock', async () => {
    const content = [0, 1, 2, 3, 4]

    const stream = new ReadableStream({
      start (controller) {
        for (let i = 0; i < content.length; i++) {
          controller.enqueue(content[i])
        }

        controller.close()
      }
    })

    const it = toIt(stream)
    expect(stream.locked).to.equal(false)

    for await (const item of it) {
      expect(stream.locked).to.equal(true)
      if (item > 2) {
        break
      }
    }

    expect(stream.locked).to.equal(false)
  })

  it('should cancel stream', async () => {
    const source = [1, 2, 3, 4, 5]
    const cancel = []
    const stream = new ReadableStream({
      pull (controller) {
        controller.enqueue(source.shift())
      },
      cancel (reason) {
        cancel.push(reason)
      }
    })

    const it = toIt(stream)
    expect(stream.locked).to.equal(false)

    for await (const item of it) {
      if (item === 2) {
        break
      }
      expect(stream.locked).to.equal(true)
    }

    expect(cancel).to.deep.equal([undefined])
    // It appears that for pulls one extra
    expect(source.length).to.be.greaterThan(1)
    expect(stream.locked).to.equal(false)
  })

  it('should not cancel stream if preventCancel is set', async () => {
    const source = [1, 2, 3, 4, 5]
    const cancel = []
    const stream = new ReadableStream({
      pull (controller) {
        controller.enqueue(source.shift())
      },
      cancel (reason) {
        cancel.push(reason)
      }
    })

    const it = toIt(stream, { preventCancel: true })
    expect(stream.locked).to.equal(false)

    for await (const item of it) {
      if (item === 2) {
        break
      }
      expect(stream.locked).to.equal(true)
    }

    expect(cancel).to.deep.equal([])
    // It appears that for pulls one extra
    expect(source.length).to.be.greaterThan(1)
    expect(stream.locked).to.equal(false)
  })

  it('error cancels the stream', async () => {
    const source = [1, 2, 3, 4, 5]
    const cancel = []
    const stream = new ReadableStream({
      pull (controller) {
        controller.enqueue(source.shift())
      },
      cancel (reason) {
        cancel.push(reason)
      }
    })

    const it = toIt(stream)
    expect(stream.locked).to.equal(false)

    try {
      for await (const item of it) {
        if (item === 2) {
          throw Error('Boom')
        }
        expect(stream.locked).to.equal(true)
      }
    } catch (_) {}

    expect(cancel).to.deep.equal([undefined])
    // It appears that for pulls one extra
    expect(source.length).to.be.greaterThan(1)
    expect(stream.locked).to.equal(false)
  })

  it('error does not cancels the stream if preventCancel is set', async () => {
    const source = [1, 2, 3, 4, 5]
    const cancel = []
    const stream = new ReadableStream({
      pull (controller) {
        controller.enqueue(source.shift())
      },
      cancel (reason) {
        cancel.push(reason)
      }
    })

    const it = toIt(stream, { preventCancel: true })
    expect(stream.locked).to.equal(false)

    try {
      for await (const item of it) {
        if (item === 2) {
          throw Error('Boom')
        }
        expect(stream.locked).to.equal(true)
      }
    } catch (_) {}

    expect(cancel).to.deep.equal([])
    // It appears that for pulls one extra
    expect(source.length).to.be.greaterThan(1)
    expect(stream.locked).to.equal(false)
  })
})
