'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var Path = require('path');
var fs = require('graceful-fs');
var util = require('util');
var glob = require('it-glob');
var errCode = require('err-code');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var Path__default = /*#__PURE__*/_interopDefaultLegacy(Path);
var fs__default = /*#__PURE__*/_interopDefaultLegacy(fs);
var glob__default = /*#__PURE__*/_interopDefaultLegacy(glob);
var errCode__default = /*#__PURE__*/_interopDefaultLegacy(errCode);

const fsStat = util.promisify(fs__default["default"].stat);
async function getFilesFromPath(paths, options) {
  const files = [];
  for await (const file of filesFromPath(paths, options)) {
    files.push(file);
  }
  return files;
}
async function* filesFromPath(paths, options) {
  options = options || {};
  if (typeof paths === 'string') {
    paths = [paths];
  }
  const globSourceOptions = {
    recursive: true,
    glob: {
      dot: Boolean(options.hidden),
      ignore: Array.isArray(options.ignore) ? options.ignore : [],
      follow: options.followSymlinks != null ? options.followSymlinks : true
    }
  };
  for await (const path of paths) {
    if (typeof path !== 'string') {
      throw errCode__default["default"](new Error('Path must be a string'), 'ERR_INVALID_PATH', { path });
    }
    const absolutePath = Path__default["default"].resolve(process.cwd(), path);
    const stat = await fsStat(absolutePath);
    const prefix = options.pathPrefix || Path__default["default"].dirname(absolutePath);
    let mode = options.mode;
    if (options.preserveMode) {
      mode = stat.mode;
    }
    let mtime = options.mtime;
    if (options.preserveMtime) {
      mtime = stat.mtime;
    }
    yield* toGlobSource({
      path,
      type: stat.isDirectory() ? 'dir' : 'file',
      prefix,
      mode,
      mtime,
      size: stat.size,
      preserveMode: options.preserveMode,
      preserveMtime: options.preserveMtime
    }, globSourceOptions);
  }
}
async function* toGlobSource({path, type, prefix, mode, mtime, size, preserveMode, preserveMtime}, options) {
  options = options || {};
  const baseName = Path__default["default"].basename(path);
  if (type === 'file') {
    yield {
      name: `/${ baseName.replace(prefix, '') }`,
      stream: () => fs__default["default"].createReadStream(Path__default["default"].isAbsolute(path) ? path : Path__default["default"].join(process.cwd(), path)),
      mode,
      mtime,
      size
    };
    return;
  }
  const globOptions = Object.assign({}, options.glob, {
    cwd: path,
    nodir: false,
    realpath: false,
    absolute: true
  });
  for await (const p of glob__default["default"](path, '**/*', globOptions)) {
    const stat = await fsStat(p);
    if (!stat.isFile()) {
      continue;
    }
    if (preserveMode || preserveMtime) {
      if (preserveMode) {
        mode = stat.mode;
      }
      if (preserveMtime) {
        mtime = stat.mtime;
      }
    }
    yield {
      name: toPosix(p.replace(prefix, '')),
      stream: () => fs__default["default"].createReadStream(p),
      mode,
      mtime,
      size: stat.size
    };
  }
}
const toPosix = path => path.replace(/\\/g, '/');

exports.filesFromPath = filesFromPath;
exports.getFilesFromPath = getFilesFromPath;
