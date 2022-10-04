'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var normaliseContent_browser = require('./normalise-content.browser.js');
var normaliseCandidateSingle = require('./normalise-candidate-single.js');

function normaliseInput(input) {
  return normaliseCandidateSingle.normaliseCandidateSingle(input, normaliseContent_browser.normaliseContent);
}

exports.normaliseInput = normaliseInput;
