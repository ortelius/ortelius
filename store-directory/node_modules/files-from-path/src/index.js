import Path from 'path'
import fs from 'graceful-fs'
import { promisify } from 'util'
import glob from 'it-glob'
import errCode from 'err-code'

// https://github.com/isaacs/node-graceful-fs/issues/160
const fsStat = promisify(fs.stat)

// ported to esm from https://github.com/ipfs/js-ipfs-utils/blob/d7e7bde061ef928d72f34c17d4d6310a7215bd73/src/files/glob-source.js
// converts path -> name and content -> stream to fit the web3.storage expectation

/**
 * @typedef FromPathOptions
 * @property {boolean} [hidden] - Include .dot files in matched paths
 * @property {Array<string>} [ignore] - Glob paths to ignore
 * @property {boolean} [followSymlinks] - follow symlinks
 * @property {boolean} [preserveMode] - preserve mode
 * @property {boolean} [preserveMtime] - preserve mtime
 * @property {number} [mode] - mode to use - if preserveMode is true this will be ignored
 * @property {import('ipfs-unixfs').MtimeLike} [mtime] - mtime to use - if preserveMtime is true this will be ignored
 * @property {string} [pathPrefix] - base path prefix that will get stripped out of the filenames yielded
 *
 * @typedef FileObject
 * @property {string} name
 * @property {() => fs.ReadStream} stream
 */

/**
 * Gets all the FileObjects that match requested file paths.
 *
 * @param {Iterable<string> | AsyncIterable<string> | string} paths - File system path(s) to glob from
 * @param {FromPathOptions} [options] - options
 * @returns {Promise<FileObject[]>}
 */
export async function getFilesFromPath (paths, options) {
  const files = []

  for await (const file of filesFromPath(paths, options)) {
    files.push(file)
  }

  return files
}

/**
 * Create an async iterator that yields paths that match requested file paths.
 *
 * @param {Iterable<string> | AsyncIterable<string> | string} paths - File system path(s) to glob from
 * @param {FromPathOptions} [options] - options
 * @yields {FileObject}
 */
export async function * filesFromPath (paths, options) {
  options = options || {}

  if (typeof paths === 'string') {
    paths = [paths]
  }

  const globSourceOptions = {
    recursive: true,
    glob: {
      dot: Boolean(options.hidden),
      ignore: Array.isArray(options.ignore) ? options.ignore : [],
      follow: options.followSymlinks != null ? options.followSymlinks : true
    }
  }

  // Check the input paths comply with options.recursive and convert to glob sources
  for await (const path of paths) {
    if (typeof path !== 'string') {
      throw errCode(
        new Error('Path must be a string'),
        'ERR_INVALID_PATH',
        { path }
      )
    }

    const absolutePath = Path.resolve(process.cwd(), path)
    const stat = await fsStat(absolutePath)
    const prefix = options.pathPrefix || Path.dirname(absolutePath)

    let mode = options.mode

    if (options.preserveMode) {
      // @ts-ignore
      mode = stat.mode
    }

    let mtime = options.mtime

    if (options.preserveMtime) {
      // @ts-ignore
      mtime = stat.mtime
    }

    yield * toGlobSource({
      path,
      type: stat.isDirectory() ? 'dir' : 'file',
      prefix,
      mode,
      mtime,
      size: stat.size,
      preserveMode: options.preserveMode,
      preserveMtime: options.preserveMtime
    }, globSourceOptions)
  }
}

// @ts-ignore
async function * toGlobSource ({ path, type, prefix, mode, mtime, size, preserveMode, preserveMtime }, options) {
  options = options || {}

  const baseName = Path.basename(path)

  if (type === 'file') {
    yield {
      name: `/${baseName.replace(prefix, '')}`,
      stream: () => fs.createReadStream(Path.isAbsolute(path) ? path : Path.join(process.cwd(), path)),
      mode,
      mtime,
      size
    }

    return
  }

  const globOptions = Object.assign({}, options.glob, {
    cwd: path,
    nodir: false,
    realpath: false,
    absolute: true
  })

  for await (const p of glob(path, '**/*', globOptions)) {
    const stat = await fsStat(p)

    if (!stat.isFile()) {
      // skip dirs.
      continue
    }

    if (preserveMode || preserveMtime) {
      if (preserveMode) {
        mode = stat.mode
      }

      if (preserveMtime) {
        mtime = stat.mtime
      }
    }

    yield {
      name: toPosix(p.replace(prefix, '')),
      stream: () => fs.createReadStream(p),
      mode,
      mtime,
      size: stat.size
    }
  }
}

/**
 * @param {string} path
 */
const toPosix = path => path.replace(/\\/g, '/')
