'use strict';

var all = require('it-all');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var all__default = /*#__PURE__*/_interopDefaultLegacy(all);

async function flat(source, reduce) {
  return reduce(await all__default["default"](source));
}

module.exports = flat;
