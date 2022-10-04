'use strict';

var chai = require('chai');
var multiformats = require('multiformats');
var pbEncode = require('../src/pb-encode.js');
var pbDecode = require('../src/pb-decode.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var chai__default = /*#__PURE__*/_interopDefaultLegacy(chai);

const {assert} = chai__default["default"];
const acidBytes = Uint8Array.from([
  1,
  85,
  0,
  5,
  0,
  1,
  2,
  3,
  4
]);
describe('Edge cases', () => {
  it('fail to encode large int', () => {
    let form = {
      Links: [{
          Hash: acidBytes,
          Tsize: Number.MAX_SAFE_INTEGER - 1
        }]
    };
    let expected = '12140a0901550005000102030418feffffffffffff0f';
    assert.strictEqual(multiformats.bytes.toHex(pbEncode.encodeNode(form)), expected);
    assert.deepEqual(pbDecode.decodeNode(multiformats.bytes.fromHex(expected)), form);
    form = {
      Links: [{
          Hash: acidBytes,
          Tsize: Number.MAX_SAFE_INTEGER
        }]
    };
    expected = '12140a0901550005000102030418ffffffffffffff0f';
    assert.strictEqual(multiformats.bytes.toHex(pbEncode.encodeNode(form)), expected);
    assert.deepEqual(pbDecode.decodeNode(multiformats.bytes.fromHex(expected)), form);
    form = {
      Links: [{
          Hash: acidBytes,
          Tsize: Number.MAX_SAFE_INTEGER + 1
        }]
    };
    expected = '12140a09015500050001020304188080808080808010';
    assert.throws(() => {
      pbEncode.encodeNode(form);
    }, /too large/);
    assert.deepEqual(pbDecode.decodeNode(multiformats.bytes.fromHex(expected)), form);
  });
  it('fail to encode negative large', () => {
    assert.throws(() => {
      pbEncode.encodeNode({
        Links: [{
            Hash: acidBytes,
            Name: 'yoik',
            Tsize: -1
          }],
        Data: new Uint8Array(0)
      });
    }, /negative/);
  });
  it('encode awkward tsize values ', () => {
    const cases = [
      6779297111,
      5368709120,
      4831838208,
      4294967296,
      3758096384,
      3221225472,
      2813203579,
      2147483648,
      1932735283,
      1610612736,
      1073741824
    ];
    for (const Tsize of cases) {
      const node = {
        Links: [{
            Hash: acidBytes,
            Name: 'big.bin',
            Tsize
          }],
        Data: new Uint8Array([
          8,
          1
        ])
      };
      const encoded = pbEncode.encodeNode(node);
      assert.deepEqual(pbDecode.decodeNode(encoded), node);
    }
  });
});
