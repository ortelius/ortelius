'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var base = require('./base.js');
var base32 = require('multiformats/bases/base32');
var raw = require('multiformats/codecs/raw');
var cid = require('multiformats/cid');
var Digest = require('multiformats/hashes/digest');
var errors = require('./errors.js');

function _interopNamespace(e) {
  if (e && e.__esModule) return e;
  var n = Object.create(null);
  if (e) {
    Object.keys(e).forEach(function (k) {
      if (k !== 'default') {
        var d = Object.getOwnPropertyDescriptor(e, k);
        Object.defineProperty(n, k, d.get ? d : {
          enumerable: true,
          get: function () { return e[k]; }
        });
      }
    });
  }
  n["default"] = e;
  return Object.freeze(n);
}

var raw__namespace = /*#__PURE__*/_interopNamespace(raw);
var Digest__namespace = /*#__PURE__*/_interopNamespace(Digest);

class MemoryBlockstore extends base.BaseBlockstore {
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
    this.data[base32.base32.encode(key.multihash.bytes)] = val;
  }
  async get(key) {
    const exists = await this.has(key);
    if (!exists)
      throw errors.notFoundError();
    return this.data[base32.base32.encode(key.multihash.bytes)];
  }
  async has(key) {
    return this.data[base32.base32.encode(key.multihash.bytes)] !== undefined;
  }
  async delete(key) {
    delete this.data[base32.base32.encode(key.multihash.bytes)];
  }
  async *_all() {
    yield* Object.entries(this.data).map(([key, value]) => ({
      key: cid.CID.createV1(raw__namespace.code, Digest__namespace.decode(base32.base32.decode(key))),
      value
    }));
  }
  async *_allKeys() {
    yield* Object.entries(this.data).map(([key]) => cid.CID.createV1(raw__namespace.code, Digest__namespace.decode(base32.base32.decode(key))));
  }
}

exports.MemoryBlockstore = MemoryBlockstore;
