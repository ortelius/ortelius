/* eslint-env mocha, browser */

'use strict'

const all = require('it-all')
const { expect } = require('chai')
const toIt = require('./index')

describe('blob-to-it', () => {
  it('should convert a blob to an async iterator', async () => {
    const content = [Uint8Array.from([0, 1, 2, 3, 4])]
    const blob = new Blob(content)
    const result = await all(toIt(blob))

    expect(content).to.deep.equal(result)
  })
})
