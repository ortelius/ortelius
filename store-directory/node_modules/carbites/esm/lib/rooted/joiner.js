import { CarWriter } from '@ipld/car';
import {
  decode,
  code
} from '@ipld/dag-cbor';
import { Block } from 'multiformats/block';
import { isRootNode } from './root-node.js';
import { SimpleCarJoiner } from '../simple/joiner.js';
export class RootedCarJoiner extends SimpleCarJoiner {
  async *car() {
    const reader = this._cars[0];
    const rblk = await getRootedBlock(reader);
    const roots = rblk.value[1];
    const {writer, out} = CarWriter.create(roots);
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
  if (roots[0].code !== code)
    throw new Error(`invalid root block code: ${ roots[0].code }`);
  const block = await reader.get(roots[0]);
  if (!block)
    throw new Error(`missing root block: ${ roots[0] }`);
  const node = decode(block.bytes);
  if (!isRootNode(node))
    throw new Error(`invalid root node: ${ block.cid }`);
  return new Block({
    cid: block.cid,
    bytes: block.bytes,
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