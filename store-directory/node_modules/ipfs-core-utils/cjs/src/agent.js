'use strict';

var http = require('http');
var https = require('https');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var http__default = /*#__PURE__*/_interopDefaultLegacy(http);
var https__default = /*#__PURE__*/_interopDefaultLegacy(https);

var agent = url => {
  if (!url) {
    throw new Error('URL required');
  }
  return url.protocol.startsWith('https') ? https__default["default"].Agent : http__default["default"].Agent;
};

module.exports = agent;
