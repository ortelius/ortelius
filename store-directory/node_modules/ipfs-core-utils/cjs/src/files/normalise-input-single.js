'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var normaliseContent = require('./normalise-content.js');
var normaliseCandidateSingle = require('./normalise-candidate-single.js');

function normaliseInput(input) {
  return normaliseCandidateSingle.normaliseCandidateSingle(input, normaliseContent.normaliseContent);
}

exports.normaliseInput = normaliseInput;
