'use strict'

/**
 * @typedef {globalThis.Headers} Headers
 * @typedef {globalThis.Request} Request
 * @typedef {globalThis.Response} Response
 */

const { isElectronMain } = require('./env')

// use window.fetch if it is available, fall back to node-fetch if not
let impl = 'native-fetch'

if (isElectronMain) {
  impl = 'electron-fetch'
}

module.exports = require(impl)
