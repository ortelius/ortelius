import chai from 'chai';
import { bytes } from 'multiformats';
import { CID } from 'multiformats/cid';
import {
  encode,
  decode
} from '../src/index.js';
import { encodeNode } from '../src/pb-encode.js';
import { decodeNode } from '../src/pb-decode.js';
const {assert} = chai;
const acid = CID.decode(Uint8Array.from([
  1,
  85,
  0,
  5,
  0,
  1,
  2,
  3,
  4
]));
function verifyRoundTrip(testCase, bypass) {
  const actualBytes = (bypass ? encodeNode : encode)(testCase.node);
  assert.strictEqual(bytes.toHex(actualBytes), testCase.expectedBytes);
  const roundTripNode = (bypass ? decodeNode : decode)(actualBytes);
  if (roundTripNode.Data) {
    roundTripNode.Data = bytes.toHex(roundTripNode.Data);
  }
  if (roundTripNode.Links) {
    for (const link of roundTripNode.Links) {
      if (link.Hash) {
        link.Hash = bytes.toHex(bypass ? link.Hash : link.Hash.bytes);
      }
    }
  }
  const actualForm = JSON.stringify(roundTripNode, null, 2);
  assert.strictEqual(actualForm, testCase.expectedForm);
}
describe('Compatibility', () => {
  it('empty', () => {
    verifyRoundTrip({
      node: { Links: [] },
      expectedBytes: '',
      expectedForm: `{
  "Links": []
}`
    });
  });
  it('Data zero', () => {
    verifyRoundTrip({
      node: {
        Data: new Uint8Array(0),
        Links: []
      },
      expectedBytes: '0a00',
      expectedForm: `{
  "Data": "",
  "Links": []
}`
    });
  });
  it('Data some', () => {
    verifyRoundTrip({
      node: {
        Data: Uint8Array.from([
          0,
          1,
          2,
          3,
          4
        ]),
        Links: []
      },
      expectedBytes: '0a050001020304',
      expectedForm: `{
  "Data": "0001020304",
  "Links": []
}`
    });
  });
  it('Links zero', () => {
    const testCase = {
      node: { Links: [] },
      expectedBytes: '',
      expectedForm: `{
  "Links": []
}`
    };
    verifyRoundTrip(testCase);
  });
  it('Data some Links zero', () => {
    const testCase = {
      node: {
        Data: Uint8Array.from([
          0,
          1,
          2,
          3,
          4
        ]),
        Links: []
      },
      expectedBytes: '0a050001020304',
      expectedForm: `{
  "Data": "0001020304",
  "Links": []
}`
    };
    verifyRoundTrip(testCase);
  });
  it('Links empty', () => {
    const testCase = {
      node: { Links: [{}] },
      expectedBytes: '1200',
      expectedForm: `{
  "Links": [
    {}
  ]
}`
    };
    assert.throws(() => verifyRoundTrip(testCase), /Hash/);
    verifyRoundTrip(testCase, true);
  });
  it('Data some Links empty', () => {
    const testCase = {
      node: {
        Data: Uint8Array.from([
          0,
          1,
          2,
          3,
          4
        ]),
        Links: [{}]
      },
      expectedBytes: '12000a050001020304',
      expectedForm: `{
  "Data": "0001020304",
  "Links": [
    {}
  ]
}`
    };
    assert.throws(() => verifyRoundTrip(testCase), /Hash/);
    verifyRoundTrip(testCase, true);
  });
  it('Links Hash zero', () => {
    const testCase = {
      node: { Links: [{ Hash: new Uint8Array(0) }] },
      expectedBytes: '12020a00',
      expectedForm: `{
  "Links": [
    {
      "Hash": ""
    }
  ]
}`
    };
    assert.throws(() => verifyRoundTrip(testCase), /CID/);
    verifyRoundTrip(testCase, true);
    assert.throws(() => decode(bytes.fromHex(testCase.expectedBytes)), /CID/);
  });
  it('Links Hash some', () => {
    verifyRoundTrip({
      node: { Links: [{ Hash: acid }] },
      expectedBytes: '120b0a09015500050001020304',
      expectedForm: `{
  "Links": [
    {
      "Hash": "015500050001020304"
    }
  ]
}`
    });
  });
  it('Links Name zero', () => {
    const testCase = {
      node: { Links: [{ Name: '' }] },
      expectedBytes: '12021200',
      expectedForm: `{
  "Links": [
    {
      "Name": ""
    }
  ]
}`
    };
    assert.throws(() => verifyRoundTrip(testCase), /Hash/);
    verifyRoundTrip(testCase, true);
  });
  it('Links Hash some Name zero', () => {
    verifyRoundTrip({
      node: {
        Links: [{
            Hash: acid,
            Name: ''
          }]
      },
      expectedBytes: '120d0a090155000500010203041200',
      expectedForm: `{
  "Links": [
    {
      "Hash": "015500050001020304",
      "Name": ""
    }
  ]
}`
    });
  });
  it('Links Name some', () => {
    const testCase = {
      node: { Links: [{ Name: 'some name' }] },
      expectedBytes: '120b1209736f6d65206e616d65',
      expectedForm: `{
  "Links": [
    {
      "Name": "some name"
    }
  ]
}`
    };
    assert.throws(() => verifyRoundTrip(testCase), /Hash/);
    verifyRoundTrip(testCase, true);
  });
  it('Links Hash some Name some', () => {
    verifyRoundTrip({
      node: {
        Links: [{
            Hash: acid,
            Name: 'some name'
          }]
      },
      expectedBytes: '12160a090155000500010203041209736f6d65206e616d65',
      expectedForm: `{
  "Links": [
    {
      "Hash": "015500050001020304",
      "Name": "some name"
    }
  ]
}`
    });
  });
  it('Links Tsize zero', () => {
    const testCase = {
      node: { Links: [{ Tsize: 0 }] },
      expectedBytes: '12021800',
      expectedForm: `{
  "Links": [
    {
      "Tsize": 0
    }
  ]
}`
    };
    assert.throws(() => verifyRoundTrip(testCase), /Hash/);
    verifyRoundTrip(testCase, true);
  });
  it('Links Hash some Tsize zero', () => {
    verifyRoundTrip({
      node: {
        Links: [{
            Hash: acid,
            Tsize: 0
          }]
      },
      expectedBytes: '120d0a090155000500010203041800',
      expectedForm: `{
  "Links": [
    {
      "Hash": "015500050001020304",
      "Tsize": 0
    }
  ]
}`
    });
  });
  it('Links Name some', () => {
    const testCase = {
      node: { Links: [{ Tsize: 1010 }] },
      expectedBytes: '120318f207',
      expectedForm: `{
  "Links": [
    {
      "Tsize": 1010
    }
  ]
}`
    };
    assert.throws(() => verifyRoundTrip(testCase), /Hash/);
    verifyRoundTrip(testCase, true);
  });
  it('Links Hash some Tsize some', () => {
    verifyRoundTrip({
      node: {
        Links: [{
            Hash: acid,
            Tsize: 9007199254740991
          }]
      },
      expectedBytes: '12140a0901550005000102030418ffffffffffffff0f',
      expectedForm: `{
  "Links": [
    {
      "Hash": "015500050001020304",
      "Tsize": 9007199254740991
    }
  ]
}`
    });
  });
});