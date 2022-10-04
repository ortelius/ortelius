/* eslint-env mocha */

import chai from 'chai'
import { CID } from 'multiformats/cid'
import { encode, validate } from '@ipld/dag-pb'

const { assert } = chai

/**
 * @typedef {import('@ipld/dag-pb').PBNode} PBNode
 */

const acid = CID.parse('bafkqabiaaebagba')

describe('Forms (Data Model)', () => {
  it('validate good forms', () => {
    const doesntThrow = (/** @type {PBNode} */ good) => {
      validate(good)
      const byts = encode(good)
      assert.instanceOf(byts, Uint8Array)
    }

    doesntThrow({ Links: [] })

    doesntThrow({ Data: Uint8Array.from([1, 2, 3]), Links: [] })
    doesntThrow({
      Links: [
        { Hash: acid },
        { Hash: acid, Name: 'bar' },
        { Hash: acid, Name: 'foo' }
      ]
    })
    doesntThrow({
      Links: [
        { Hash: acid },
        { Hash: acid, Name: 'a' },
        { Hash: acid, Name: 'a' }
      ]
    })
    const l = { Hash: acid, Name: 'a' }
    doesntThrow({ Links: [l, l] })
  })

  it('validate fails bad forms', () => {
    const throws = (/** @type {any} */ bad) => {
      assert.throws(() => {
        // @ts-ignore because type checking is the point
        validate(bad)
      })
      assert.throws(() => {
        // @ts-ignore because type checking is the point
        encode(bad)
      })
    }

    for (const bad of [true, false, null, 0, 101, -101, 'blip', [], Infinity, Symbol.for('boop'), Uint8Array.from([1, 2, 3])]) {
      throws(bad)
    }

    throws({})
    throws({ Data: null, Links: null })
    throws({ Data: null, Links: [] })
    throws({ Links: null })

    // empty link
    throws({ Links: [{}] })

    throws({ Data: acid.bytes, extraneous: true })
    throws({ Links: [{ Hash: acid, extraneous: true }] })

    // bad Data forms
    for (const bad of [true, false, 0, 101, -101, 'blip', Infinity, Symbol.for('boop'), []]) {
      throws({ Data: bad, Links: [] })
    }

    // bad Link array forms
    for (const bad of [true, false, 0, 101, -101, 'blip', Infinity, Symbol.for('boop'), Uint8Array.from([1, 2, 3])]) {
      throws({ Links: bad })
    }

    // bad Link forms
    for (const bad of [true, false, 0, 101, -101, 'blip', Infinity, Symbol.for('boop'), Uint8Array.from([1, 2, 3])]) {
      throws({ Links: [bad] })
    }

    // bad Link.Hash forms
    for (const bad of [true, false, 0, 101, -101, [], {}, Infinity, Symbol.for('boop'), Uint8Array.from([1, 2, 3])]) {
      throws({ Links: [{ Hash: bad }] })
    }

    // bad Link.Name forms
    for (const bad of [true, false, 0, 101, -101, [], {}, Infinity, Symbol.for('boop'), Uint8Array.from([1, 2, 3])]) {
      throws({ Links: [{ Hash: acid, Name: bad }] })
    }

    // bad Link.Tsize forms
    for (const bad of [true, false, [], 'blip', {}, Symbol.for('boop'), Uint8Array.from([1, 2, 3])]) {
      throws({ Links: [{ Hash: acid, Tsize: bad }] })
    }

    // bad sort
    throws({
      Links: [
        { Hash: acid },
        { Hash: acid, Name: 'foo' },
        { Hash: acid, Name: 'bar' }
      ]
    })
    throws({
      Links: [
        { Hash: acid },
        { Hash: acid, Name: 'aa' },
        { Hash: acid, Name: 'a' }
      ]
    })
  })
})
