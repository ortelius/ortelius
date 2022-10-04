'use strict';

var errCode = require('err-code');
var extractDataFromBlock = require('../utils/extract-data-from-block.js');
var validateOffsetAndLength = require('../utils/validate-offset-and-length.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);

const rawContent = node => {
  async function* contentGenerator(options = {}) {
    const {offset, length} = validateOffsetAndLength(node.length, options.offset, options.length);
    yield extractDataFromBlock(node, 0, offset, offset + length);
  }
  return contentGenerator;
};
const resolve = async (cid, name, path, toResolve, resolve, depth, blockstore, options) => {
  if (toResolve.length) {
    throw errCode__default["default"](new Error(`No link named ${ path } found in raw node ${ cid }`), 'ERR_NOT_FOUND');
  }
  const block = await blockstore.get(cid, options);
  return {
    entry: {
      type: 'raw',
      name,
      path,
      cid,
      content: rawContent(block),
      depth,
      size: block.length,
      node: block
    }
  };
};

module.exports = resolve;
