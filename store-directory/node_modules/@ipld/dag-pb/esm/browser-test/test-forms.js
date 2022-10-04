import chai from 'chai';
import { CID } from 'multiformats/cid';
import {
  encode,
  validate
} from '../src/index.js';
const {assert} = chai;
const acid = CID.parse('bafkqabiaaebagba');
describe('Forms (Data Model)', () => {
  it('validate good forms', () => {
    const doesntThrow = good => {
      validate(good);
      const byts = encode(good);
      assert.instanceOf(byts, Uint8Array);
    };
    doesntThrow({ Links: [] });
    doesntThrow({
      Data: Uint8Array.from([
        1,
        2,
        3
      ]),
      Links: []
    });
    doesntThrow({
      Links: [
        { Hash: acid },
        {
          Hash: acid,
          Name: 'bar'
        },
        {
          Hash: acid,
          Name: 'foo'
        }
      ]
    });
    doesntThrow({
      Links: [
        { Hash: acid },
        {
          Hash: acid,
          Name: 'a'
        },
        {
          Hash: acid,
          Name: 'a'
        }
      ]
    });
    const l = {
      Hash: acid,
      Name: 'a'
    };
    doesntThrow({
      Links: [
        l,
        l
      ]
    });
  });
  it('validate fails bad forms', () => {
    const throws = bad => {
      assert.throws(() => {
        validate(bad);
      });
      assert.throws(() => {
        encode(bad);
      });
    };
    for (const bad of [
        true,
        false,
        null,
        0,
        101,
        -101,
        'blip',
        [],
        Infinity,
        Symbol.for('boop'),
        Uint8Array.from([
          1,
          2,
          3
        ])
      ]) {
      throws(bad);
    }
    throws({});
    throws({
      Data: null,
      Links: null
    });
    throws({
      Data: null,
      Links: []
    });
    throws({ Links: null });
    throws({ Links: [{}] });
    throws({
      Data: acid.bytes,
      extraneous: true
    });
    throws({
      Links: [{
          Hash: acid,
          extraneous: true
        }]
    });
    for (const bad of [
        true,
        false,
        0,
        101,
        -101,
        'blip',
        Infinity,
        Symbol.for('boop'),
        []
      ]) {
      throws({
        Data: bad,
        Links: []
      });
    }
    for (const bad of [
        true,
        false,
        0,
        101,
        -101,
        'blip',
        Infinity,
        Symbol.for('boop'),
        Uint8Array.from([
          1,
          2,
          3
        ])
      ]) {
      throws({ Links: bad });
    }
    for (const bad of [
        true,
        false,
        0,
        101,
        -101,
        'blip',
        Infinity,
        Symbol.for('boop'),
        Uint8Array.from([
          1,
          2,
          3
        ])
      ]) {
      throws({ Links: [bad] });
    }
    for (const bad of [
        true,
        false,
        0,
        101,
        -101,
        [],
        {},
        Infinity,
        Symbol.for('boop'),
        Uint8Array.from([
          1,
          2,
          3
        ])
      ]) {
      throws({ Links: [{ Hash: bad }] });
    }
    for (const bad of [
        true,
        false,
        0,
        101,
        -101,
        [],
        {},
        Infinity,
        Symbol.for('boop'),
        Uint8Array.from([
          1,
          2,
          3
        ])
      ]) {
      throws({
        Links: [{
            Hash: acid,
            Name: bad
          }]
      });
    }
    for (const bad of [
        true,
        false,
        [],
        'blip',
        {},
        Symbol.for('boop'),
        Uint8Array.from([
          1,
          2,
          3
        ])
      ]) {
      throws({
        Links: [{
            Hash: acid,
            Tsize: bad
          }]
      });
    }
    throws({
      Links: [
        { Hash: acid },
        {
          Hash: acid,
          Name: 'foo'
        },
        {
          Hash: acid,
          Name: 'bar'
        }
      ]
    });
    throws({
      Links: [
        { Hash: acid },
        {
          Hash: acid,
          Name: 'aa'
        },
        {
          Hash: acid,
          Name: 'a'
        }
      ]
    });
  });
});