import {
  CarReader,
  CarWriter
} from '@ipld/car';
import { CID } from 'multiformats/cid';
const empty = CID.parse('bafkqaaa');
export class SimpleCarSplitter {
  constructor(reader, targetSize) {
    if (typeof targetSize !== 'number' || targetSize <= 0) {
      throw new Error('invalid target chunk size');
    }
    this._reader = reader;
    this._targetSize = targetSize;
  }
  async *cars() {
    const allBlocks = this._reader.blocks()[Symbol.asyncIterator]();
    const originRoots = await this._reader.getRoots();
    let blocks = [];
    let size = 0;
    let first = true;
    let finished = false;
    while (!finished) {
      while (size < this._targetSize) {
        const {
          done,
          value: block
        } = await allBlocks.next();
        if (done) {
          finished = true;
          break;
        }
        blocks.push(block);
        size += block.bytes.length;
      }
      const roots = first ? originRoots : [empty];
      const {writer, out} = CarWriter.create(roots);
      Object.assign(out, {
        version: 1,
        getRoots: async () => roots
      });
      blocks.forEach(b => writer.put(b));
      writer.close();
      yield out;
      blocks = [];
      size = 0;
      first = false;
    }
  }
  static async fromBlob(blob, targetSize) {
    const buffer = await blob.arrayBuffer();
    const reader = await CarReader.fromBytes(new Uint8Array(buffer));
    return new SimpleCarSplitter(reader, targetSize);
  }
  static async fromIterable(iterable, targetSize) {
    const reader = await CarReader.fromIterable(iterable);
    return new SimpleCarSplitter(reader, targetSize);
  }
}