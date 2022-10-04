'use strict';

var chai = require('chai');
var multiformats = require('multiformats');
var pbDecode = require('../src/pb-decode.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var chai__default = /*#__PURE__*/_interopDefaultLegacy(chai);

const {assert} = chai__default["default"];
describe('Protobuf format', () => {
  describe('PBNode', () => {
    it('bad wireType', () => {
      const block = multiformats.bytes.fromHex('0a0500010203040');
      for (let i = 0; i < 8; i++) {
        if (i === 2) {
          continue;
        }
        block[0] = 1 << 3 | i;
        assert.throws(() => pbDecode.decodeNode(block), /PBNode.*wireType/);
      }
    });
    it('bad fieldNum', () => {
      const block = multiformats.bytes.fromHex('0a0500010203040');
      for (let i = 0; i < 32; i++) {
        if (i === 1 || i === 2) {
          continue;
        }
        block[0] = i << 3 | 2;
        assert.throws(() => pbDecode.decodeNode(block), /PBNode.*fieldNum/);
      }
    });
    it('duplicate Data', () => {
      assert.throws(() => {
        pbDecode.decodeNode(multiformats.bytes.fromHex('0a0500010203040a050001020304'));
      }, /PBNode.*duplicate Data/);
    });
  });
  describe('PBLink', () => {
    it('bad wireType for Hash', () => {
      const block = multiformats.bytes.fromHex('120b0a09015500050001020304');
      for (let i = 0; i < 8; i++) {
        if (i === 2) {
          continue;
        }
        block[2] = 1 << 3 | i;
        assert.throws(() => pbDecode.decodeNode(block), /PBLink.*wireType.*Hash/);
      }
    });
    it('bad wireType for Name', () => {
      const block = multiformats.bytes.fromHex('12160a090155000500010203041209736f6d65206e616d65');
      for (let i = 0; i < 8; i++) {
        if (i === 2) {
          continue;
        }
        block[13] = 2 << 3 | i;
        assert.throws(() => pbDecode.decodeNode(block), /PBLink.*wireType.*Name/);
      }
    });
    it('bad wireType for Tsize', () => {
      const block = multiformats.bytes.fromHex('120e0a0901550005000102030418f207');
      for (let i = 0; i < 8; i++) {
        if (i === 0) {
          continue;
        }
        block[13] = 3 << 3 | i;
        assert.throws(() => pbDecode.decodeNode(block), /PBLink.*wireType.*Tsize/);
      }
    });
    it('bad fieldNum', () => {
      const block = multiformats.bytes.fromHex('120b0a09015500050001020304');
      for (let i = 0; i < 32; i++) {
        if (i === 1 || i === 2 || i === 3) {
          continue;
        }
        block[2] = i << 3 | 2;
        assert.throws(() => pbDecode.decodeNode(block), /fieldNum/);
      }
    });
    it('Name before Hash', () => {
      assert.throws(() => {
        pbDecode.decodeNode(multiformats.bytes.fromHex('120d12000a09015500050001020304'));
      }, /PBLink.*Name before Hash/);
    });
    it('Tsize before Hash', () => {
      assert.throws(() => {
        pbDecode.decodeNode(multiformats.bytes.fromHex('120e18f2070a09015500050001020304'));
      }, /PBLink.*Tsize before Hash/);
    });
    it('Tsize before Name', () => {
      assert.throws(() => {
        pbDecode.decodeNode(multiformats.bytes.fromHex('120518f2071200'));
      }, /PBLink.*Tsize before Name/);
    });
    it('duplicate Hash', () => {
      assert.throws(() => {
        pbDecode.decodeNode(multiformats.bytes.fromHex('12160a090155000500010203040a09015500050001020304'));
      }, /PBLink.*duplicate Hash/);
    });
    it('duplicate Name', () => {
      assert.throws(() => {
        pbDecode.decodeNode(multiformats.bytes.fromHex('12210a090155000500010203041209736f6d65206e616d651209736f6d65206e616d65'));
      }, /PBLink.*duplicate Name/);
    });
    it('duplicate Tsize', () => {
      assert.throws(() => {
        pbDecode.decodeNode(multiformats.bytes.fromHex('12110a0901550005000102030418f20718f207'));
      }, /PBLink.*duplicate Tsize/);
    });
  });
});
