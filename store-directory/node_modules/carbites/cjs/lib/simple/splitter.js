'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var car = require('@ipld/car');
var cid = require('multiformats/cid');

const empty = cid.CID.parse('bafkqaaa');
class SimpleCarSplitter {
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
      const {writer, out} = car.CarWriter.create(roots);
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
    const reader = await car.CarReader.fromBytes(new Uint8Array(buffer));
    return new SimpleCarSplitter(reader, targetSize);
  }
  static async fromIterable(iterable, targetSize) {
    const reader = await car.CarReader.fromIterable(iterable);
    return new SimpleCarSplitter(reader, targetSize);
  }
}

exports.SimpleCarSplitter = SimpleCarSplitter;
