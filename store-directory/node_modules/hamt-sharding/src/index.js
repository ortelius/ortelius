'use strict'

const Bucket = require('./bucket')
const wrapHash = require('./consumable-hash')

/**
 * @typedef {object} UserBucketOptions
 * @property {(value: Uint8Array) => Promise<Uint8Array>} hashFn
 * @property {number} [bits=8]
 */

/**
 * @param {UserBucketOptions} options
 */
function createHAMT (options) {
  if (!options || !options.hashFn) {
    throw new Error('please define an options.hashFn')
  }

  const bucketOptions = {
    bits: options.bits || 8,
    hash: wrapHash(options.hashFn)
  }

  return new Bucket(bucketOptions)
}

module.exports = {
  createHAMT,
  Bucket
}
