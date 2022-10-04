'use strict';

var index = require('../index.js');
var chai = require('chai');
var multiformats = require('multiformats');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var chai__default = /*#__PURE__*/_interopDefaultLegacy(chai);

const {assert} = chai__default["default"];
const fixtures = [
  [
    'murmur3-32',
    'beep boop',
    '2304243ddb9e'
  ],
  [
    'murmur3-128',
    'beep boop',
    '2210acfe9c5bbf88f075c0c4df0464430ead'
  ]
];
describe('Digests', () => {
  for (const [name, input, expectedDigest] of fixtures) {
    it(name, async () => {
      const hasher = index[name.replace('-', '')];
      const hash = await hasher.digest(multiformats.bytes.fromString(input));
      assert.strictEqual(hash.code, hasher.code);
      assert.strictEqual(multiformats.bytes.toHex(hash.bytes), expectedDigest);
    });
  }
});
describe('Hashers', () => {
  const fixture = multiformats.bytes.fromString('testFixture');
  it('murmur3-32', async () => {
    const hash = await index.murmur332.digest(fixture);
    assert.strictEqual(index.murmur332.code, 35);
    assert.strictEqual(hash.code, index.murmur332.code);
    assert.strictEqual(multiformats.bytes.toHex(hash.bytes), '2304e7285ca1');
  });
  it('murmur3-128', async () => {
    const hash = await index.murmur3128.digest(fixture);
    assert.strictEqual(index.murmur3128.code, 34);
    assert.strictEqual(hash.code, index.murmur3128.code);
    assert.strictEqual(multiformats.bytes.toHex(hash.bytes), '2210df48782b0b497325f116d6589ef4c112');
  });
});
