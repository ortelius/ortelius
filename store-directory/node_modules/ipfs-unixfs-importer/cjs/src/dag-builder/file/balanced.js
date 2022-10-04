'use strict';

var batch = require('it-batch');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var batch__default = /*#__PURE__*/_interopDefaultLegacy(batch);

function balanced(source, reduce, options) {
  return reduceToParents(source, reduce, options);
}
async function reduceToParents(source, reduce, options) {
  const roots = [];
  for await (const chunked of batch__default["default"](source, options.maxChildrenPerNode)) {
    roots.push(await reduce(chunked));
  }
  if (roots.length > 1) {
    return reduceToParents(roots, reduce, options);
  }
  return roots[0];
}

module.exports = balanced;
