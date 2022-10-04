'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var car = require('@ipld/car');
var dagCbor = require('@ipld/dag-cbor');
var block = require('multiformats/block');
var rootNode = require('./root-node.js');
var joiner = require('../simple/joiner.js');

class RootedCarJoiner extends joiner.SimpleCarJoiner {
  async *car() {
    const reader = this._cars[0];
    const rblk = await getRootedBlock(reader);
    const roots = rblk.value[1];
    const {writer, out} = car.CarWriter.create(roots);
    const writeCar = async () => {
      try {
        for await (const b of filterBlock(rblk.cid, reader)) {
          await writer.put(b);
        }
        for (const reader of this._cars.slice(1)) {
          const rblk = await getRootedBlock(reader);
          for await (const b of filterBlock(rblk.cid, reader)) {
            await writer.put(b);
          }
        }
      } catch (err) {
        console.error(err);
      } finally {
        await writer.close();
      }
    };
    writeCar();
    yield* out;
  }
}
async function getRootedBlock(reader) {
  const roots = await reader.getRoots();
  if (roots.length !== 1)
    throw new Error(`invalid number of roots: ${ roots.length }`);
  if (roots[0].code !== dagCbor.code)
    throw new Error(`invalid root block code: ${ roots[0].code }`);
  const block$1 = await reader.get(roots[0]);
  if (!block$1)
    throw new Error(`missing root block: ${ roots[0] }`);
  const node = dagCbor.decode(block$1.bytes);
  if (!rootNode.isRootNode(node))
    throw new Error(`invalid root node: ${ block$1.cid }`);
  return new block.Block({
    cid: block$1.cid,
    bytes: block$1.bytes,
    value: node
  });
}
async function* filterBlock(cid, reader) {
  for await (const b of reader.blocks()) {
    if (b.cid.equals(cid))
      continue;
    yield b;
  }
}

exports.RootedCarJoiner = RootedCarJoiner;
