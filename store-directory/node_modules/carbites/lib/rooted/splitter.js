import { CarReader, CarWriter } from '@ipld/car'
import { mkRootNode } from './root-node.js'
import { SimpleCarSplitter } from '../simple/splitter.js'

export class RootedCarSplitter extends SimpleCarSplitter {
  async * cars () {
    const allBlocks = this._reader.blocks()[Symbol.asyncIterator]()
    const roots = await this._reader.getRoots()
    let blocks = []
    let size = 0
    let first = true
    let finished = false

    while (!finished) {
      while (size < this._targetSize) {
        const { done, value: block } = await allBlocks.next()
        if (done) {
          finished = true
          break
        }
        blocks.push(block)
        size += block.bytes.length
      }
      const root = await mkRootNode(first ? roots : [], blocks.map(b => b.cid))
      const { writer, out } = CarWriter.create(root.cid)
      Object.assign(out, { version: 1, getRoots: async () => [root.cid] })
      writer.put(root)
      blocks.forEach(b => writer.put(b))
      writer.close()
      yield out
      blocks = []
      size = 0
      first = false
    }
  }

  /**
   * @param {Blob} blob
   * @param {number} targetSize
   */
  static async fromBlob (blob, targetSize) {
    const buffer = await blob.arrayBuffer()
    const reader = await CarReader.fromBytes(new Uint8Array(buffer))
    return new RootedCarSplitter(reader, targetSize)
  }

  /**
   * @param {AsyncIterable<Uint8Array>} iterable
   * @param {number} targetSize
   */
  static async fromIterable (iterable, targetSize) {
    const reader = await CarReader.fromIterable(iterable)
    return new RootedCarSplitter(reader, targetSize)
  }
}
