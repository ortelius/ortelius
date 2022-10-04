import { CarReader, CarWriter } from '@ipld/car'
import { Block } from 'multiformats/block'
import * as raw from 'multiformats/codecs/raw'
import * as cbor from '@ipld/dag-cbor'
import * as pb from '@ipld/dag-pb'

/**
 * @typedef {{ decoders?: import('multiformats/codecs/interface').BlockDecoder<any, any>[] }} Options
 * @typedef {import('@ipld/car/api').WriterChannel & { size: number }} WriterChannel
 * @typedef {import('multiformats/cid').CID} CID
 * @typedef {import('@ipld/car/api').BlockReader & import('@ipld/car/api').RootsReader} ICarReader
 */

export class TreewalkCarSplitter {
  /**
   * @param {ICarReader} reader
   * @param {number} targetSize
   * @param {Options} [options]
   */
  constructor (reader, targetSize, options = {}) {
    if (typeof targetSize !== 'number' || targetSize <= 0) {
      throw new Error('invalid target chunk size')
    }
    this._reader = reader
    this._targetSize = targetSize
    /** @type {import('multiformats/codecs/interface').BlockDecoder<any, any>[]} */
    this._decoders = [pb, raw, cbor, ...(options.decoders || [])]
  }

  async * cars () {
    const roots = await this._reader.getRoots()
    if (roots.length !== 1) throw new Error(`unexpected number of roots: ${roots.length}`)
    let channel
    for await (const val of this._cars(roots[0])) {
      channel = val.channel
      if (val.out) yield val.out
    }

    if (!channel) {
      throw new Error('missing CAR writer channel')
    }

    channel.writer.close()
    yield channel.out
  }

  /**
   * @param {import('multiformats/cid').CID} cid
   */
  async _get (cid) {
    const rawBlock = await this._reader.get(cid)
    if (!rawBlock) throw new Error(`missing block for ${cid}`)
    const { bytes } = rawBlock
    const decoder = this._decoders.find(d => d.code === cid.code)
    if (!decoder) throw new Error(`missing decoder for ${cid.code}`)
    return new Block({ cid, bytes, value: decoder.decode(bytes) })
  }

  /**
   * @param {CID} cid
   * @param {Block<any>[]} [parents]
   * @param {WriterChannel} [channel]
   * @returns {AsyncGenerator<{ channel: WriterChannel, out?: AsyncIterable<Uint8Array> }>}
   */
  async * _cars (cid, parents = [], channel = undefined) {
    const block = await this._get(cid)
    channel = channel || Object.assign(CarWriter.create(cid), { size: 0 })

    if (channel.size > 0 && channel.size + block.bytes.byteLength >= this._targetSize) {
      channel.writer.close()
      const { out } = channel
      channel = newCar(parents)
      yield { channel, out }
    }

    parents = parents.concat(block)
    channel.size += block.bytes.byteLength
    channel.writer.put(block)

    for (const [, cid] of block.links()) {
      // @ts-ignore TS does not get type of async generator
      for await (const val of this._cars(cid, parents, channel)) {
        channel = val.channel
        yield val
      }
    }

    if (!channel) {
      throw new Error('missing CAR writer channel')
    }

    yield { channel }
  }

  /**
   * @param {AsyncIterable<Uint8Array>} iterable
   * @param {number} targetSize
   * @param {Options} [options]
   */
  static async fromIterable (iterable, targetSize, options) {
    const reader = await CarReader.fromIterable(iterable)
    return new TreewalkCarSplitter(reader, targetSize, options)
  }

  /**
   * @param {Blob} blob
   * @param {number} targetSize
   * @param {Options} [options]
   */
  static async fromBlob (blob, targetSize, options) {
    const buffer = await blob.arrayBuffer()
    const reader = await CarReader.fromBytes(new Uint8Array(buffer))
    return new TreewalkCarSplitter(reader, targetSize, options)
  }
}

/**
 * @param {Block<any>[]} parents
 */
function newCar (parents) {
  const ch = Object.assign(
    CarWriter.create(parents[0].cid),
    { size: parents.reduce((size, b) => size + b.bytes.byteLength, 0) }
  )
  for (const b of parents) {
    ch.writer.put(b)
  }
  return ch
}
