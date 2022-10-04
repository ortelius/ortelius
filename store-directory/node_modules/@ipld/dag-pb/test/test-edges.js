/* eslint-env mocha */

import chai from 'chai'
import { bytes } from 'multiformats'
import { encodeNode } from '../src/pb-encode.js'
import { decodeNode } from '../src/pb-decode.js'

const { assert } = chai
const acidBytes = Uint8Array.from([1, 85, 0, 5, 0, 1, 2, 3, 4])

describe('Edge cases', () => {
  it('fail to encode large int', () => {
    // sanity check maximum forms

    let form = { Links: [{ Hash: acidBytes, Tsize: Number.MAX_SAFE_INTEGER - 1 }] }
    let expected = '12140a0901550005000102030418feffffffffffff0f'
    assert.strictEqual(
      bytes.toHex(
        // @ts-ignore RawPBLink needs Name but we don't have one
        encodeNode(form))
      , expected)
    assert.deepEqual(decodeNode(bytes.fromHex(expected)), form)

    form = { Links: [{ Hash: acidBytes, Tsize: Number.MAX_SAFE_INTEGER }] }
    expected = '12140a0901550005000102030418ffffffffffffff0f'
    assert.strictEqual(
      bytes.toHex(
        // @ts-ignore RawPBLink needs Name but we don't have one
        encodeNode(form))
      , expected)
    assert.deepEqual(decodeNode(bytes.fromHex(expected)), form)

    // too big, we can decode but not encode, it's a tiny bit too hard to bother
    form = { Links: [{ Hash: acidBytes, Tsize: Number.MAX_SAFE_INTEGER + 1 }] }
    expected = '12140a09015500050001020304188080808080808010'
    assert.throws(() => {
      // @ts-ignore RawPBLink needs Name but we don't have one
      encodeNode(form)
    }, /too large/)
    assert.deepEqual(decodeNode(bytes.fromHex(expected)), form)
  })

  it('fail to encode negative large', () => {
    assert.throws(() => {
      encodeNode({ Links: [{ Hash: acidBytes, Name: 'yoik', Tsize: -1 }], Data: new Uint8Array(0) })
    }, /negative/)
  })

  it('encode awkward tsize values ', () => {
    // testing len64() to make sure we can properly calculate the encoded length
    // of various awkward values
    const cases = [6779297111, 5368709120, 4831838208, 4294967296, 3758096384,
      3221225472, 2813203579, 2147483648, 1932735283, 1610612736, 1073741824]
    for (const Tsize of cases) {
      const node = {
        Links: [{ Hash: acidBytes, Name: 'big.bin', Tsize }],
        Data: new Uint8Array([8, 1])
      }
      const encoded = encodeNode(node)
      assert.deepEqual(decodeNode(encoded), node)
    }
  })
})
