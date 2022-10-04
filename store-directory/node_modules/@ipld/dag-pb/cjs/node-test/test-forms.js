'use strict';

var chai = require('chai');
var cid = require('multiformats/cid');
var index = require('../src/index.js');
var util = require('../src/util.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var chai__default = /*#__PURE__*/_interopDefaultLegacy(chai);

const {assert} = chai__default["default"];
const acid = cid.CID.parse('bafkqabiaaebagba');
describe('Forms (Data Model)', () => {
  it('validate good forms', () => {
    const doesntThrow = good => {
      util.validate(good);
      const byts = index.encode(good);
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
        util.validate(bad);
      });
      assert.throws(() => {
        index.encode(bad);
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
