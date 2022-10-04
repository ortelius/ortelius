import Path from 'path';
import fs from 'graceful-fs';
import { promisify } from 'util';
import glob from 'it-glob';
import errCode from 'err-code';
const fsStat = promisify(fs.stat);
export async function getFilesFromPath(paths, options) {
  const files = [];
  for await (const file of filesFromPath(paths, options)) {
    files.push(file);
  }
  return files;
}
export async function* filesFromPath(paths, options) {
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
      throw errCode(new Error('Path must be a string'), 'ERR_INVALID_PATH', { path });
    }
    const absolutePath = Path.resolve(process.cwd(), path);
    const stat = await fsStat(absolutePath);
    const prefix = options.pathPrefix || Path.dirname(absolutePath);
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
  const baseName = Path.basename(path);
  if (type === 'file') {
    yield {
      name: `/${ baseName.replace(prefix, '') }`,
      stream: () => fs.createReadStream(Path.isAbsolute(path) ? path : Path.join(process.cwd(), path)),
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
  for await (const p of glob(path, '**/*', globOptions)) {
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
      stream: () => fs.createReadStream(p),
      mode,
      mtime,
      size: stat.size
    };
  }
}
const toPosix = path => path.replace(/\\/g, '/');