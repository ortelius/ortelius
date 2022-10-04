import chai from 'chai';
import { bytes } from 'multiformats';
import { decodeNode } from '../src/pb-decode.js';
const {assert} = chai;
describe('Protobuf format', () => {
  describe('PBNode', () => {
    it('bad wireType', () => {
      const block = bytes.fromHex('0a0500010203040');
      for (let i = 0; i < 8; i++) {
        if (i === 2) {
          continue;
        }
        block[0] = 1 << 3 | i;
        assert.throws(() => decodeNode(block), /PBNode.*wireType/);
      }
    });
    it('bad fieldNum', () => {
      const block = bytes.fromHex('0a0500010203040');
      for (let i = 0; i < 32; i++) {
        if (i === 1 || i === 2) {
          continue;
        }
        block[0] = i << 3 | 2;
        assert.throws(() => decodeNode(block), /PBNode.*fieldNum/);
      }
    });
    it('duplicate Data', () => {
      assert.throws(() => {
        decodeNode(bytes.fromHex('0a0500010203040a050001020304'));
      }, /PBNode.*duplicate Data/);
    });
  });
  describe('PBLink', () => {
    it('bad wireType for Hash', () => {
      const block = bytes.fromHex('120b0a09015500050001020304');
      for (let i = 0; i < 8; i++) {
        if (i === 2) {
          continue;
        }
        block[2] = 1 << 3 | i;
        assert.throws(() => decodeNode(block), /PBLink.*wireType.*Hash/);
      }
    });
    it('bad wireType for Name', () => {
      const block = bytes.fromHex('12160a090155000500010203041209736f6d65206e616d65');
      for (let i = 0; i < 8; i++) {
        if (i === 2) {
          continue;
        }
        block[13] = 2 << 3 | i;
        assert.throws(() => decodeNode(block), /PBLink.*wireType.*Name/);
      }
    });
    it('bad wireType for Tsize', () => {
      const block = bytes.fromHex('120e0a0901550005000102030418f207');
      for (let i = 0; i < 8; i++) {
        if (i === 0) {
          continue;
        }
        block[13] = 3 << 3 | i;
        assert.throws(() => decodeNode(block), /PBLink.*wireType.*Tsize/);
      }
    });
    it('bad fieldNum', () => {
      const block = bytes.fromHex('120b0a09015500050001020304');
      for (let i = 0; i < 32; i++) {
        if (i === 1 || i === 2 || i === 3) {
          continue;
        }
        block[2] = i << 3 | 2;
        assert.throws(() => decodeNode(block), /fieldNum/);
      }
    });
    it('Name before Hash', () => {
      assert.throws(() => {
        decodeNode(bytes.fromHex('120d12000a09015500050001020304'));
      }, /PBLink.*Name before Hash/);
    });
    it('Tsize before Hash', () => {
      assert.throws(() => {
        decodeNode(bytes.fromHex('120e18f2070a09015500050001020304'));
      }, /PBLink.*Tsize before Hash/);
    });
    it('Tsize before Name', () => {
      assert.throws(() => {
        decodeNode(bytes.fromHex('120518f2071200'));
      }, /PBLink.*Tsize before Name/);
    });
    it('duplicate Hash', () => {
      assert.throws(() => {
        decodeNode(bytes.fromHex('12160a090155000500010203040a09015500050001020304'));
      }, /PBLink.*duplicate Hash/);
    });
    it('duplicate Name', () => {
      assert.throws(() => {
        decodeNode(bytes.fromHex('12210a090155000500010203041209736f6d65206e616d651209736f6d65206e616d65'));
      }, /PBLink.*duplicate Name/);
    });
    it('duplicate Tsize', () => {
      assert.throws(() => {
        decodeNode(bytes.fromHex('12110a0901550005000102030418f20718f207'));
      }, /PBLink.*duplicate Tsize/);
    });
  });
});