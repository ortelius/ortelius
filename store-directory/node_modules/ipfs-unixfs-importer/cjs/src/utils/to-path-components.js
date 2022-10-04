'use strict';

const toPathComponents = (path = '') => {
  return (path.trim().match(/([^\\/]|\\\/)+/g) || []).filter(Boolean);
};

module.exports = toPathComponents;
