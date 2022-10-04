'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var normaliseContent = require('./normalise-content.js');
var normaliseCandidateMultiple = require('./normalise-candidate-multiple.js');

function normaliseInput(input) {
  return normaliseCandidateMultiple.normaliseCandidateMultiple(input, normaliseContent.normaliseContent);
}

exports.normaliseInput = normaliseInput;
