'use strict';

var chai = require('chai');
var chaiSubset = require('chai-subset');
var multiformats = require('multiformats');
var cid = require('multiformats/cid');
var sha2 = require('multiformats/hashes/sha2');
var index = require('../src/index.js');
var util = require('../src/util.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var chai__default = /*#__PURE__*/_interopDefaultLegacy(chai);
var chaiSubset__default = /*#__PURE__*/_interopDefaultLegacy(chaiSubset);

chai__default["default"].use(chaiSubset__default["default"]);
const {assert} = chai__default["default"];
function linkCidsToStrings(links) {
  return links.map(l => {
    return {
      Name: l.Name,
      Tsize: l.Tsize,
      Hash: l.Hash.toString()
    };
  });
}
describe('Basics', () => {
  it('prepare & encode an empty node', () => {
    const prepared = util.prepare({});
    assert.deepEqual(prepared, { Links: [] });
    const result = index.encode(prepared);
    assert.instanceOf(result, Uint8Array);
    assert.strictEqual(result.length, 0);
  });
  it('prepare & encode a node with data', () => {
    const data = Uint8Array.from([
      0,
      1,
      2,
      3,
      4
    ]);
    const prepared = util.prepare({ Data: data });
    assert.deepEqual(prepared, {
      Data: data,
      Links: []
    });
    const result = index.encode(prepared);
    assert.instanceOf(result, Uint8Array);
    const node = index.decode(result);
    assert.deepEqual(node.Data, data);
  });
  it('prepare & encode a node with links', () => {
    const links = [{ Hash: cid.CID.parse('QmWDtUQj38YLW8v3q4A6LwPn4vYKEbuKWpgSm6bjKW6Xfe') }];
    const prepared = util.prepare({ Links: links });
    assert.deepEqual(prepared, { Links: [{ Hash: links[0].Hash }] });
    const result = index.encode(prepared);
    assert.instanceOf(result, Uint8Array);
    const node = index.decode(result);
    assert.containSubset(linkCidsToStrings(node.Links), linkCidsToStrings([{ Hash: cid.CID.parse('QmWDtUQj38YLW8v3q4A6LwPn4vYKEbuKWpgSm6bjKW6Xfe') }]));
  });
  it('prepare & encode a node with links as plain objects', () => {
    const links = [{ Hash: cid.CID.parse('QmWDtUQj38YLW8v3q4A6LwPn4vYKEbuKWpgSm6bjKW6Xfe') }];
    const prepared = util.prepare({ Links: links });
    assert.deepEqual(prepared, { Links: [{ Hash: links[0].Hash }] });
    const result = index.encode(prepared);
    assert.instanceOf(result, Uint8Array);
    const node = index.decode(result);
    assert.containSubset(linkCidsToStrings(node.Links), linkCidsToStrings(links));
  });
  it('ignore invalid properties when preparing', () => {
    const prepared = util.prepare({ foo: 'bar' });
    assert.deepEqual(prepared, { Links: [] });
    const result = index.encode(prepared);
    assert.strictEqual(result.length, 0);
  });
  it('prepare & create a node with string data', () => {
    const data = 'some data';
    const prepared = util.prepare({ Data: data });
    assert.deepEqual(prepared, {
      Data: new TextEncoder().encode(data),
      Links: []
    });
    const serialized = index.encode(prepared);
    const deserialized = index.decode(serialized);
    assert.deepEqual(deserialized.Data, new TextEncoder().encode('some data'));
  });
  it('prepare & create a node with bare string', () => {
    const data = 'some data';
    const prepared = util.prepare(data);
    assert.deepEqual(prepared, {
      Data: new TextEncoder().encode(data),
      Links: []
    });
    const serialized = index.encode(prepared);
    const deserialized = index.decode(serialized);
    assert.deepEqual(deserialized.Data, new TextEncoder().encode('some data'));
  });
  it('prepare & create a node with links (& sorting)', () => {
    const origLinks = [
      {
        Name: 'some other link',
        Hash: cid.CID.parse('QmXg9Pp2ytZ14xgmQjYEiHjVjMFXzCVVEcRTWJBmLgR39V'),
        Tsize: 8
      },
      {
        Name: 'some link',
        Hash: cid.CID.parse('QmXg9Pp2ytZ14xgmQjYEiHjVjMFXzCVVEcRTWJBmLgR39U'),
        Tsize: 100000000
      }
    ];
    const someData = new TextEncoder().encode('some data');
    const node = {
      Data: someData,
      Links: origLinks.map(({Name, Tsize, Hash}) => {
        return {
          Name,
          Tsize,
          Hash
        };
      })
    };
    const prepared = util.prepare(node);
    assert.deepEqual(prepared.Links.map(l => l.Name), [
      'some link',
      'some other link'
    ]);
    const byts = index.encode(prepared);
    const expectedBytes = '12340a2212208ab7a6c5e74737878ac73863cb76739d15d4666de44e5756bf55a2f9e9ab5f431209736f6d65206c696e6b1880c2d72f12370a2212208ab7a6c5e74737878ac73863cb76739d15d4666de44e5756bf55a2f9e9ab5f44120f736f6d65206f74686572206c696e6b18080a09736f6d652064617461';
    assert.strictEqual(multiformats.bytes.toHex(byts), expectedBytes);
    const reconstituted = index.decode(byts);
    assert.deepEqual(reconstituted.Links.map(l => l.Name), [
      'some link',
      'some other link'
    ]);
  });
  it('prepare & create a node with stable sorted links', () => {
    const links = [
      {
        Name: '',
        Hash: cid.CID.parse('QmUGhP2X8xo9dsj45vqx1H6i5WqPqLqmLQsHTTxd3ke8mp'),
        Tsize: 262158
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmP7SrR76KHK9A916RbHG1ufy2TzNABZgiE23PjZDMzZXy'),
        Tsize: 262158
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmQg1v4o9xdT3Q14wh4S7dxZkDjyZ9ssFzFzyep1YrVJBY'),
        Tsize: 262158
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmdP6fartWRrydZCUjHgrJ4XpxSE4SAoRsWJZ1zJ4MWiuf'),
        Tsize: 262158
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmNNjUStxtMC1WaSZYiDW6CmAUrvd5Q2e17qnxPgVdwrwW'),
        Tsize: 262158
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmWJwqZBJWerHsN1b7g4pRDYmzGNnaMYuD3KSbnpaxsB2h'),
        Tsize: 262158
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmRXPSdysBS3dbUXe6w8oXevZWHdPQWaR2d3fggNsjvieL'),
        Tsize: 262158
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmTUZAXfws6zrhEksnMqLxsbhXZBQs4FNiarjXSYQqVrjC'),
        Tsize: 262158
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmNNk7dTdh8UofwgqLNauq6N78DPc6LKK2yBs1MFdx7Mbg'),
        Tsize: 262158
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmW5mrJfyqh7B4ywSvraZgnWjS3q9CLiYURiJpCX3aro5i'),
        Tsize: 262158
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmTFHZL5CkgNz19MdPnSuyLAi6AVq9fFp81zmPpaL2amED'),
        Tsize: 262158
      }
    ];
    const node = {
      Data: new TextEncoder().encode('some data'),
      Links: links
    };
    const prepared = util.prepare(node);
    assert.deepEqual(prepared, node);
    const reconstituted = index.decode(index.encode(node));
    assert.deepEqual(reconstituted.Links.map(l => l.Hash), links.map(l => l.Hash));
  });
  it('prepare & create with empty link name', () => {
    const node = {
      Data: new TextEncoder().encode('hello'),
      Links: [cid.CID.parse('QmXg9Pp2ytZ14xgmQjYEiHjVjMFXzCVVEcRTWJBmLgR39U')]
    };
    const expected = {
      Data: node.Data,
      Links: [{ Hash: node.Links[0] }]
    };
    const prepared = util.prepare(node);
    assert.deepEqual(prepared, expected);
    const reconstituted = index.decode(index.encode(prepared));
    assert.deepEqual(reconstituted, expected);
  });
  it('prepare & create with undefined link name', () => {
    const node = {
      Data: new TextEncoder().encode('hello'),
      Links: [{
          Tsize: 10,
          Hash: cid.CID.parse('QmXg9Pp2ytZ14xgmQjYEiHjVjMFXzCVVEcRTWJBmLgR39U')
        }]
    };
    const prepared = util.prepare(node);
    assert.deepEqual(prepared, node);
    const reconstituted = index.decode(index.encode(prepared));
    assert.deepEqual(reconstituted, node);
  });
  it('prepare & create a node with bytes only', () => {
    const node = new TextEncoder().encode('hello');
    const reconstituted = index.decode(index.encode(util.prepare(node)));
    assert.deepEqual(reconstituted, {
      Data: new TextEncoder().encode('hello'),
      Links: []
    });
  });
  it('prepare & create an empty node', () => {
    const node = new Uint8Array(0);
    const prepared = util.prepare(node);
    assert.deepEqual(prepared, {
      Data: new Uint8Array(0),
      Links: []
    });
    const reconstituted = index.decode(index.encode(prepared));
    assert.deepEqual(reconstituted, {
      Data: new Uint8Array(0),
      Links: []
    });
  });
  it('prepare & create an empty node from object', () => {
    const prepared = util.prepare({});
    assert.deepEqual(prepared, { Links: [] });
    const reconstituted = index.decode(index.encode(prepared));
    assert.deepEqual(reconstituted, { Links: [] });
  });
  it('fail to prepare & create a node with other data types', () => {
    const invalids = [
      [],
      true,
      100,
      () => {
      },
      Symbol.for('nope')
    ];
    for (const invalid of invalids) {
      assert.throws(() => index.encode(util.prepare(invalid)), 'Invalid DAG-PB form');
    }
  });
  it('fail to prepare & create a link with other data types', () => {
    const invalids = [
      [],
      true,
      100,
      () => {
      },
      Symbol.for('nope'),
      { asCID: {} }
    ];
    for (const invalid of invalids) {
      assert.throws(() => index.encode(util.prepare({ Links: [invalid] })), 'Invalid DAG-PB form');
      if (!Array.isArray(invalid)) {
        assert.throws(() => index.encode(util.prepare({ Links: invalid })), 'Invalid DAG-PB form');
      }
    }
  });
  it('fail to create link with bad CID hash', () => {
    assert.throws(() => util.prepare({
      Links: [{
          Hash: Uint8Array.from([
            240,
            1,
            2,
            3,
            4
          ])
        }]
    }), 'Invalid DAG-PB form');
  });
  it('deserialize go-ipfs block with unnamed links', async () => {
    const testBlockUnnamedLinks = multiformats.bytes.fromHex('122b0a2212203f29086b59b9e046b362b4b19c9371e834a9f5a80597af83be6d8b7d1a5ad33b120018aed4e015122b0a221220ae1a5afd7c770507dddf17f92bba7a326974af8ae5277c198cf13206373f7263120018aed4e015122b0a22122022ab2ebf9c3523077bd6a171d516ea0e1be1beb132d853778bcc62cd208e77f1120018aed4e015122b0a22122040a77fe7bc69bbef2491f7633b7c462d0bce968868f88e2cbcaae9d0996997e8120018aed4e015122b0a2212206ae1979b14dd43966b0241ebe80ac2a04ad48959078dc5affa12860648356ef6120018aed4e015122b0a221220a957d1f89eb9a861593bfcd19e0637b5c957699417e2b7f23c88653a240836c4120018aed4e015122b0a221220345f9c2137a2cd76d7b876af4bfecd01f80b7dd125f375cb0d56f8a2f96de2c31200189bfec10f0a2b080218cbc1819201208080e015208080e015208080e015208080e015208080e015208080e01520cbc1c10f');
    const expectedLinks = [
      {
        Name: '',
        Hash: cid.CID.parse('QmSbCgdsX12C4KDw3PDmpBN9iCzS87a5DjgSCoW9esqzXk'),
        Tsize: 45623854
      },
      {
        Name: '',
        Hash: cid.CID.parse('Qma4GxWNhywSvWFzPKtEswPGqeZ9mLs2Kt76JuBq9g3fi2'),
        Tsize: 45623854
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmQfyxyys7a1e3mpz9XsntSsTGc8VgpjPj5BF1a1CGdGNc'),
        Tsize: 45623854
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmSh2wTTZT4N8fuSeCFw7wterzdqbE93j1XDhfN3vQHzDV'),
        Tsize: 45623854
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmVXsSVjwxMsCwKRCUxEkGb4f4B98gXVy3ih3v4otvcURK'),
        Tsize: 45623854
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmZjhH97MEYwQXzCqSQbdjGDhXWuwW4RyikR24pNqytWLj'),
        Tsize: 45623854
      },
      {
        Name: '',
        Hash: cid.CID.parse('QmRs6U5YirCqC7taTynz3x2GNaHJZ3jDvMVAzaiXppwmNJ'),
        Tsize: 32538395
      }
    ];
    const node = index.decode(testBlockUnnamedLinks);
    assert.deepEqual(node.Links, expectedLinks);
    const hash = await sha2.sha256.digest(testBlockUnnamedLinks);
    const cid$1 = cid.CID.create(0, index.code, hash);
    assert.strictEqual(cid$1.toString(), 'QmQqy2SiEkKgr2cw5UbQ93TtLKEMsD8TdcWggR8q9JabjX');
  });
  it('deserialize go-ipfs block with named links', async () => {
    const testBlockNamedLinks = multiformats.bytes.fromHex('12390a221220b4397c02da5513563d33eef894bf68f2ccdf1bdfc14a976956ab3d1c72f735a0120e617564696f5f6f6e6c792e6d346118cda88f0b12310a221220025c13fcd1a885df444f64a4a82a26aea867b1148c68cb671e83589f971149321208636861742e74787418e40712340a2212205d44a305b9b328ab80451d0daa72a12a7bf2763c5f8bbe327597a31ee40d1e48120c706c61796261636b2e6d3375187412360a2212202539ed6e85f2a6f9097db9d76cffd49bf3042eb2e3e8e9af4a3ce842d49dea22120a7a6f6f6d5f302e6d70341897fb8592010a020801');
    const expectedLinks = [
      {
        Name: 'audio_only.m4a',
        Hash: cid.CID.parse('QmaUAwAQJNtvUdJB42qNbTTgDpzPYD1qdsKNtctM5i7DGB'),
        Tsize: 23319629
      },
      {
        Name: 'chat.txt',
        Hash: cid.CID.parse('QmNVrxbB25cKTRuKg2DuhUmBVEK9NmCwWEHtsHPV6YutHw'),
        Tsize: 996
      },
      {
        Name: 'playback.m3u',
        Hash: cid.CID.parse('QmUcjKzDLXBPmB6BKHeKSh6ZoFZjss4XDhMRdLYRVuvVfu'),
        Tsize: 116
      },
      {
        Name: 'zoom_0.mp4',
        Hash: cid.CID.parse('QmQqy2SiEkKgr2cw5UbQ93TtLKEMsD8TdcWggR8q9JabjX'),
        Tsize: 306281879
      }
    ];
    const node = index.decode(testBlockNamedLinks);
    assert.deepEqual(node.Links, expectedLinks);
    const hash = await sha2.sha256.digest(testBlockNamedLinks);
    const cid$1 = cid.CID.create(0, index.code, hash);
    assert.strictEqual(cid$1.toString(), 'QmbSAC58x1tsuPBAoarwGuTQAgghKvdbKSBC8yp5gKCj5M');
  });
  it('deserialize ancient ipfs block with Data before Links', async () => {
    const outOfOrderNodeHex = '0a040802180612240a221220cf92fdefcdc34cac009c8b05eb662be0618db9de55ecd42785e9ec6712f8df6512240a221220cf92fdefcdc34cac009c8b05eb662be0618db9de55ecd42785e9ec6712f8df65';
    const outOfOrderNode = multiformats.bytes.fromHex(outOfOrderNodeHex);
    const node = index.decode(outOfOrderNode);
    const reencoded = index.encode(node);
    assert.notStrictEqual(multiformats.bytes.toHex(reencoded), outOfOrderNodeHex);
  });
  it('node with data between links', async () => {
    const doubleLinksNode = multiformats.bytes.fromHex('12240a221220cf92fdefcdc34cac009c8b05eb662be0618db9de55ecd42785e9ec6712f8df650a040802180612240a221220cf92fdefcdc34cac009c8b05eb662be0618db9de55ecd42785e9ec6712f8df65');
    assert.throws(() => index.decode(doubleLinksNode), /PBNode.*duplicate Links section/);
  });
  it('prepare & create with multihash bytes', () => {
    const linkHash = multiformats.bytes.fromHex('12208ab7a6c5e74737878ac73863cb76739d15d4666de44e5756bf55a2f9e9ab5f43');
    const link = {
      Name: 'hello',
      Tsize: 3,
      Hash: linkHash
    };
    const node = {
      Data: new TextEncoder().encode('some data'),
      Links: [link]
    };
    const prepared = util.prepare(node);
    assert.strictEqual(prepared.Links[0].Hash.toString(), 'QmXg9Pp2ytZ14xgmQjYEiHjVjMFXzCVVEcRTWJBmLgR39U');
    const reconstituted = index.decode(index.encode(prepared));
    assert.strictEqual(reconstituted.Links[0].Hash.toString(), 'QmXg9Pp2ytZ14xgmQjYEiHjVjMFXzCVVEcRTWJBmLgR39U');
  });
  it('prepare & create with CID string', () => {
    const linkString = 'QmXg9Pp2ytZ14xgmQjYEiHjVjMFXzCVVEcRTWJBmLgR39U';
    const link = {
      Name: 'hello',
      Tsize: 3,
      Hash: linkString
    };
    const node = {
      Data: new TextEncoder().encode('some data'),
      Links: [link]
    };
    const prepared = util.prepare(node);
    assert.strictEqual(prepared.Links[0].Hash.toString(), linkString);
    const reconstituted = index.decode(index.encode(prepared));
    assert.strictEqual(reconstituted.Links[0].Hash.toString(), linkString);
  });
  it('fail to create without hash', () => {
    const node = {
      Data: new TextEncoder().encode('some data'),
      Links: [{
          Name: 'hello',
          Tsize: 3
        }]
    };
    assert.throws(() => util.prepare(node), 'Invalid DAG-PB form');
  });
});
describe('create*() utility functions', () => {
  const data = Uint8Array.from([
    0,
    1,
    2,
    3,
    4
  ]);
  const aCid = cid.CID.parse('QmWDtUQj38YLW8v3q4A6LwPn4vYKEbuKWpgSm6bjKW6Xfe');
  const links = [
    {
      Name: 'foo',
      Hash: cid.CID.parse('QmUGhP2X8xo9dsj45vqx1H6i5WqPqLqmLQsHTTxd3ke8mp'),
      Tsize: 262158
    },
    {
      Name: 'boo',
      Hash: cid.CID.parse('QmP7SrR76KHK9A916RbHG1ufy2TzNABZgiE23PjZDMzZXy'),
      Tsize: 262158
    },
    {
      Name: 'yep',
      Hash: cid.CID.parse('QmQg1v4o9xdT3Q14wh4S7dxZkDjyZ9ssFzFzyep1YrVJBY'),
      Tsize: 262158
    }
  ];
  const linksSorted = [
    links[1],
    links[0],
    links[2]
  ];
  it('createNode()', () => {
    assert.deepStrictEqual(util.createNode(data), {
      Data: data,
      Links: []
    });
    assert.deepStrictEqual(util.createNode(data, []), {
      Data: data,
      Links: []
    });
    assert.deepStrictEqual(util.createNode(data, [links[0]]), {
      Data: data,
      Links: [links[0]]
    });
    assert.deepStrictEqual(util.createNode(data, links), {
      Data: data,
      Links: linksSorted
    });
    assert.deepStrictEqual(util.createNode(), { Links: [] });
  });
  it('createNode() errors', () => {
    const invalids = [
      [],
      true,
      100,
      () => {
      },
      Symbol.for('nope')
    ];
    for (const invalid of invalids) {
      assert.throws(() => util.createNode(invalid));
    }
  });
  it('createLink()', () => {
    assert.deepStrictEqual(util.createLink('foo', 100, aCid), {
      Hash: aCid,
      Name: 'foo',
      Tsize: 100
    });
    for (const l of links) {
      assert.deepStrictEqual(util.createLink(l.Name, l.Tsize, l.Hash), l);
    }
    assert.deepStrictEqual(util.createLink('foo', undefined, aCid), {
      Hash: aCid,
      Name: 'foo'
    });
    assert.deepStrictEqual(util.createLink(undefined, undefined, aCid), { Hash: aCid });
  });
  it('createNode() errors', () => {
    const invalids = [
      undefined,
      null,
      [],
      true,
      100,
      () => {
      },
      Symbol.for('nope'),
      {}
    ];
    for (const invalid1 of invalids) {
      for (const invalid2 of invalids) {
        for (const invalid3 of invalids) {
          assert.throws(() => util.createLink(invalid1, invalid2, invalid3));
        }
      }
    }
  });
});
