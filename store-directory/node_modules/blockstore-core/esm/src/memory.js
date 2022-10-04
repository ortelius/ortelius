import { BaseBlockstore } from './base.js';
import { base32 } from 'multiformats/bases/base32';
import * as raw from 'multiformats/codecs/raw';
import { CID } from 'multiformats/cid';
import * as Digest from 'multiformats/hashes/digest';
import * as Errors from './errors.js';
export class MemoryBlockstore extends BaseBlockstore {
  constructor() {
    super();
    this.data = {};
  }
  open() {
    return Promise.resolve();
  }
  close() {
    return Promise.resolve();
  }
  async put(key, val) {
    this.data[base32.encode(key.multihash.bytes)] = val;
  }
  async get(key) {
    const exists = await this.has(key);
    if (!exists)
      throw Errors.notFoundError();
    return this.data[base32.encode(key.multihash.bytes)];
  }
  async has(key) {
    return this.data[base32.encode(key.multihash.bytes)] !== undefined;
  }
  async delete(key) {
    delete this.data[base32.encode(key.multihash.bytes)];
  }
  async *_all() {
    yield* Object.entries(this.data).map(([key, value]) => ({
      key: CID.createV1(raw.code, Digest.decode(base32.decode(key))),
      value
    }));
  }
  async *_allKeys() {
    yield* Object.entries(this.data).map(([key]) => CID.createV1(raw.code, Digest.decode(base32.decode(key))));
  }
}