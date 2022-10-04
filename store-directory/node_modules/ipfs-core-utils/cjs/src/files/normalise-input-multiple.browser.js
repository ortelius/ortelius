'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var normaliseContent_browser = require('./normalise-content.browser.js');
var normaliseCandidateMultiple = require('./normalise-candidate-multiple.js');

function normaliseInput(input) {
  return normaliseCandidateMultiple.normaliseCandidateMultiple(input, normaliseContent_browser.normaliseContent, true);
}

exports.normaliseInput = normaliseInput;
