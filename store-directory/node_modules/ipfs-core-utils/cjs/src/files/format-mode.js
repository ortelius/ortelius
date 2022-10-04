'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

const S_ISUID = parseInt('4000', 8);
const S_ISGID = parseInt('2000', 8);
const S_ISVTX = parseInt('1000', 8);
const S_IRUSR = parseInt('400', 8);
const S_IWUSR = parseInt('200', 8);
const S_IXUSR = parseInt('100', 8);
const S_IRGRP = parseInt('40', 8);
const S_IWGRP = parseInt('20', 8);
const S_IXGRP = parseInt('10', 8);
const S_IROTH = parseInt('4', 8);
const S_IWOTH = parseInt('2', 8);
const S_IXOTH = parseInt('1', 8);
function checkPermission(mode, perm, type, output) {
  if ((mode & perm) === perm) {
    output.push(type);
  } else {
    output.push('-');
  }
}
function formatMode(mode, isDirectory) {
  const output = [];
  if (isDirectory) {
    output.push('d');
  } else {
    output.push('-');
  }
  checkPermission(mode, S_IRUSR, 'r', output);
  checkPermission(mode, S_IWUSR, 'w', output);
  if ((mode & S_ISUID) === S_ISUID) {
    output.push('s');
  } else {
    checkPermission(mode, S_IXUSR, 'x', output);
  }
  checkPermission(mode, S_IRGRP, 'r', output);
  checkPermission(mode, S_IWGRP, 'w', output);
  if ((mode & S_ISGID) === S_ISGID) {
    output.push('s');
  } else {
    checkPermission(mode, S_IXGRP, 'x', output);
  }
  checkPermission(mode, S_IROTH, 'r', output);
  checkPermission(mode, S_IWOTH, 'w', output);
  if ((mode & S_ISVTX) === S_ISVTX) {
    output.push('t');
  } else {
    checkPermission(mode, S_IXOTH, 'x', output);
  }
  return output.join('');
}

exports.formatMode = formatMode;
