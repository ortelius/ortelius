import { CarReader, CarWriter } from '@ipld/car'
import { CID } from 'multiformats/cid'

/**
 * @typedef {import('@ipld/car/api').BlockReader & import('@ipld/car/api').RootsReader} ICarReader
 */

/**
 * A work-around for use-cases where the inclusion of a root CID is difficult
 * but needing to be safely within the "at least one" recommendation is to use
 * an empty CID: \x01\x55\x00\x00 (zero-length "identity" multihash with "raw"
 * codec). Since current implementations for this version of the CAR
 * specification don't check for the existence of root CIDs (see Root CID block
 * existence), this will be safe as far as CAR implementations are concerned.
 * However, there is no guarantee that applications that use CAR files will
 * correctly consume (ignore) this empty root CID.
 *
 * https://github.com/ipld/specs/blob/master/block-layer/content-addressable-archives.md#number-of-roots
 */
const empty = CID.parse('bafkqaaa')

export class SimpleCarSplitter {
  /**
   * @param {ICarReader} reader
   * @param {number} targetSize
   */
  constructor (reader, targetSize) {
    if (typeof targetSize !== 'number' || targetSize <= 0) {
      throw new Error('invalid target chunk size')
    }
    this._reader = reader
    this._targetSize = targetSize
  }

  async * cars () {
    const allBlocks = this._reader.blocks()[Symbol.asyncIterator]()
    const originRoots = await this._reader.getRoots()
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
      const roots = first ? originRoots : [empty]
      const { writer, out } = CarWriter.create(roots)
      Object.assign(out, { version: 1, getRoots: async () => roots })
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
    return new SimpleCarSplitter(reader, targetSize)
  }

  /**
   * @param {AsyncIterable<Uint8Array>} iterable
   * @param {number} targetSize
   */
  static async fromIterable (iterable, targetSize) {
    const reader = await CarReader.fromIterable(iterable)
    return new SimpleCarSplitter(reader, targetSize)
  }
}
