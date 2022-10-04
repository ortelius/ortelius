'use strict'

const batch = require('it-batch')

/**
 * @template T
 * @typedef {{ok:true, value:T}} Success
 */

/**
 * @typedef {{ok:false, err:Error}} Failure
 */

/**
 * Takes an (async) iterator that emits promise-returning functions,
 * invokes them in parallel and emits the results as they become available but
 * in the same order as the input
 *
 * @template T
 * @param {AsyncIterable<() => Promise<T>>|Iterable<() => Promise<T>>} source
 * @param {number} [size=1]
 * @returns {AsyncIterable<T>}
 */
async function * parallelBatch (source, size = 1) {
  for await (const tasks of batch(source, size)) {
    /** @type {Promise<Success<T>|Failure>[]} */
    const things = tasks.map(
      /**
       * @param {() => Promise<T>} p
       */
      p => {
        return p().then(value => ({ ok: true, value }), err => ({ ok: false, err }))
      })

    for (let i = 0; i < things.length; i++) {
      const result = await things[i]

      if (result.ok) {
        yield result.value
      } else {
        throw result.err
      }
    }
  }
}

module.exports = parallelBatch
