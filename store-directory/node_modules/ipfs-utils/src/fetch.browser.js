'use strict'

/**
 * @typedef {globalThis.Headers} Headers
 * @typedef {globalThis.Request} Request
 * @typedef {globalThis.Response} Response
 */

// use window.fetch if it is available, fall back to node-fetch if not
module.exports = require('native-fetch')
