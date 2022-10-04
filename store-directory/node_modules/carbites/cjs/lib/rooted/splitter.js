'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var car = require('@ipld/car');
var rootNode = require('./root-node.js');
var splitter = require('../simple/splitter.js');

class RootedCarSplitter extends splitter.SimpleCarSplitter {
  async *cars() {
    const allBlocks = this._reader.blocks()[Symbol.asyncIterator]();
    const roots = await this._reader.getRoots();
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
      const root = await rootNode.mkRootNode(first ? roots : [], blocks.map(b => b.cid));
      const {writer, out} = car.CarWriter.create(root.cid);
      Object.assign(out, {
        version: 1,
        getRoots: async () => [root.cid]
      });
      writer.put(root);
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
    return new RootedCarSplitter(reader, targetSize);
  }
  static async fromIterable(iterable, targetSize) {
    const reader = await car.CarReader.fromIterable(iterable);
    return new RootedCarSplitter(reader, targetSize);
  }
}

exports.RootedCarSplitter = RootedCarSplitter;
