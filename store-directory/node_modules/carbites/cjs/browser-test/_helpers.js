'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var car = require('@ipld/car');
var dagCbor = require('@ipld/dag-cbor');
var ipldGarbage = require('ipld-garbage');
var sha2 = require('multiformats/hashes/sha2');
var cid = require('multiformats/cid');

function _interopNamespace(e) {
  if (e && e.__esModule) return e;
  var n = Object.create(null);
  if (e) {
    Object.keys(e).forEach(function (k) {
      if (k !== 'default') {
        var d = Object.getOwnPropertyDescriptor(e, k);
        Object.defineProperty(n, k, d.get ? d : {
          enumerable: true,
          get: function () {
            return e[k];
          }
        });
      }
    });
  }
  n['default'] = e;
  return Object.freeze(n);
}

var dagCbor__namespace = /*#__PURE__*/_interopNamespace(dagCbor);

function randomInt(min, max) {
  return Math.random() * (max - min) + min;
}
async function randomCar(targetSize) {
  const blocks = [];
  let size = 0;
  const seen = new Set();
  while (size < targetSize) {
    const bytes = dagCbor__namespace.encode(ipldGarbage.garbage(randomInt(1, targetSize), { weights: { CID: 0 } }));
    const hash = await sha2.sha256.digest(bytes);
    const cid$1 = cid.CID.create(1, dagCbor__namespace.code, hash);
    if (seen.has(cid$1.toString()))
      continue;
    seen.add(cid$1.toString());
    blocks.push({
      cid: cid$1,
      bytes
    });
    size += bytes.length;
  }
  const rootBytes = dagCbor__namespace.encode(blocks.map(b => b.cid));
  const rootHash = await sha2.sha256.digest(rootBytes);
  const rootCid = cid.CID.create(1, dagCbor__namespace.code, rootHash);
  const {writer, out} = car.CarWriter.create([rootCid]);
  writer.put({
    cid: rootCid,
    bytes: rootBytes
  });
  blocks.forEach(b => writer.put(b));
  writer.close();
  return out;
}
async function collect(iterable) {
  const chunks = [];
  for await (const chunk of iterable) {
    chunks.push(chunk);
  }
  return chunks;
}
async function collectBytes(iterable) {
  const chunks = await collect(iterable);
  return new Uint8Array([].concat(...chunks.map(c => Array.from(c))));
}
function bytesEqual(b0, b1) {
  return b0.length === b1.length && b0.every((v, i) => v === b1[i]);
}
function toAsyncIterable(iterable) {
  return async function* () {
    for (const item of iterable) {
      yield item;
    }
  }();
}

exports.bytesEqual = bytesEqual;
exports.collect = collect;
exports.collectBytes = collectBytes;
exports.randomCar = randomCar;
exports.toAsyncIterable = toAsyncIterable;
