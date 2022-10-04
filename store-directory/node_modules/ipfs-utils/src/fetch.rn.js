// @ts-nocheck
'use strict'
// @ts-ignore
const { Headers, Request, Response, fetch } = require('react-native-fetch-api')

/** @type {import('electron-fetch').default} */
const rnFetch = fetch
/** @type {import('electron-fetch').Headers} */
const rnHeaders = Headers
/** @type {import('electron-fetch').Request} */
const rnRequest = Request
/** @type {import('electron-fetch').Response} */
const rnResponse = Response
module.exports = rnFetch
module.exports.Headers = rnHeaders
module.exports.Request = rnRequest
module.exports.Response = rnResponse
module.exports.default = rnFetch
