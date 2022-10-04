import * as murmur3 from '../index.js';
import chai from 'chai';
import { bytes } from 'multiformats';
const {assert} = chai;
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
      const hasher = murmur3[name.replace('-', '')];
      const hash = await hasher.digest(bytes.fromString(input));
      assert.strictEqual(hash.code, hasher.code);
      assert.strictEqual(bytes.toHex(hash.bytes), expectedDigest);
    });
  }
});
describe('Hashers', () => {
  const fixture = bytes.fromString('testFixture');
  it('murmur3-32', async () => {
    const hash = await murmur3.murmur332.digest(fixture);
    assert.strictEqual(murmur3.murmur332.code, 35);
    assert.strictEqual(hash.code, murmur3.murmur332.code);
    assert.strictEqual(bytes.toHex(hash.bytes), '2304e7285ca1');
  });
  it('murmur3-128', async () => {
    const hash = await murmur3.murmur3128.digest(fixture);
    assert.strictEqual(murmur3.murmur3128.code, 34);
    assert.strictEqual(hash.code, murmur3.murmur3128.code);
    assert.strictEqual(bytes.toHex(hash.bytes), '2210df48782b0b497325f116d6589ef4c112');
  });
});