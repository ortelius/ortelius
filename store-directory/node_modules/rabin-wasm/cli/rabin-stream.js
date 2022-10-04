const fs = require('fs');
const stream = require('readable-stream')
const Rabin = require('../src/rabin')
const util = require('util')
const BufferList = require('bl')
const debug = require('debug')('rabin')

const { instantiateSync } = require("@assemblyscript/loader");
const imports = {};
const compiled = instantiateSync(fs.readFileSync(__dirname + "/../dist/rabin.wasm"), imports);

module.exports = RabinStream

function RabinStream (opts = {}) {
  stream.Duplex.call(this)
  this._readableState.highWaterMark = 16
  this._readableState.objectMode = true
  this.destroyed = false
  var avgBits = +opts.bits || 12
  var min = +opts.min || 8 * 1024
  var max = +opts.max || 32 * 1024
  this.rabin = new Rabin(compiled, avgBits, min, max, 64)
  this.nextCb = null
  this.buffers = new BufferList()
  this.pending = []
  this.on('finish', this._finish)
}

util.inherits(RabinStream, stream.Duplex)

RabinStream.prototype._finish = function () {
  if (this.destroyed) return
  if (this.buffers.length) this.push(this.buffers.slice(0, this.buffers.length))
  this.push(null)
}

RabinStream.prototype._writev = function (batch, cb) {
  if (this.destroyed) return cb()

  for (var i = 0; i < batch.length; i++) {
    this.buffers.append(batch[i].chunk)
    this.pending.push(batch[i].chunk)
  }
  this._process(cb)
}

RabinStream.prototype._read = function (size) {
  var nextCb = this.nextCb
  if (nextCb) {
    this.nextCb = null
    nextCb()
  }
}

RabinStream.prototype._write = function (data, enc, cb) {
  if (this.destroyed) return cb()

  this.buffers.append(data)
  this.pending.push(data)
  this._process(cb)
}

RabinStream.prototype._process = function (cb) {
  var drained = true
  var sizes = this.rabin.fingerprint(Buffer.concat(this.pending))

  this.pending = []

  debug('chunks', sizes)

  for (var i = 0; i < sizes.length; i++) {
    var size = sizes[i]
    if(size === 0) break
    var buf = this.buffers.slice(0, size)
    this.buffers.consume(size)
    drained = this.push(buf)
  }

  if (drained) cb()
  else this.nextCb = cb
}

RabinStream.prototype.destroy = function (err) {
  if (this.destroyed) return
  this.destroyed = true
  if (err) this.emit('error', err)
  this.emit('close')
}