'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var multiaddr = require('multiaddr');
var multiAddrToUri = require('multiaddr-to-uri');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var multiAddrToUri__default = /*#__PURE__*/_interopDefaultLegacy(multiAddrToUri);

function toUrlString(url) {
  try {
    url = multiAddrToUri__default["default"](new multiaddr.Multiaddr(url));
  } catch (err) {
  }
  url = url.toString();
  return url;
}

exports.toUrlString = toUrlString;
