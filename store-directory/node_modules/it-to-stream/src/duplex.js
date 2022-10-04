const { Readable, Writable, Duplex } = require('stream')
const getIterator = require('get-iterator')
const Fifo = require('p-fifo')
const { Buffer } = require('buffer')
const END_CHUNK = Buffer.alloc(0)

module.exports = function toDuplex (duplex, options) {
  options = options || {}

  let reading = false
  const fifo = new Fifo()

  duplex = {
    sink: duplex.sink,
    source: duplex.source ? getIterator(duplex.source) : null
  }

  let Stream = Duplex
  if (!duplex.source) {
    Stream = Writable
  } else if (!duplex.sink) {
    Stream = Readable
  }

  let readable
  if (duplex.source) {
    readable = {
      async read (size) {
        if (reading) return
        reading = true

        try {
          while (true) {
            const { value, done } = await duplex.source.next(size)
            if (done) return this.push(null)
            if (!this.push(value)) break
          }
        } catch (err) {
          this.emit('error', err)
        } finally {
          reading = false
        }
      }
    }
  }

  let writable
  if (duplex.sink) {
    writable = {
      write (chunk, enc, cb) {
        fifo.push(chunk).then(() => cb(), cb)
      },
      final (cb) {
        fifo.push(END_CHUNK).then(() => cb(), cb)
      }
    }
  }

  Object.assign(options, readable, writable)

  const stream = new Stream(options)

  if (duplex.sink) {
    duplex.sink({
      [Symbol.asyncIterator] () {
        return this
      },
      async next () {
        const chunk = await fifo.shift()
        return chunk === END_CHUNK ? { done: true } : { value: chunk }
      },
      async throw (err) {
        stream.destroy(err)
        return { done: true }
      },
      async return () {
        stream.destroy()
        return { done: true }
      }
    })
  }

  return stream
}
