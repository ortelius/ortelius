'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

function modeToString(mode) {
  if (mode == null) {
    return undefined;
  }
  if (typeof mode === 'string') {
    return mode;
  }
  return mode.toString(8).padStart(4, '0');
}

exports.modeToString = modeToString;
