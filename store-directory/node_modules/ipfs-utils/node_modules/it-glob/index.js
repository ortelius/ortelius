'use strict'

const fs = require('fs').promises
const path = require('path')
const minimatch = require('minimatch')

/**
 * @typedef {string} Glob
 * @typedef {object} OptionsExt
 * @property {string} [cwd=process.cwd()]
 * @property {boolean} [absolute=false] - If true produces absolute paths
 * @property {boolean} [nodir] - If true yields file paths and skip directories
 *
 * @typedef {OptionsExt & minimatch.IOptions} Options
 */

/**
 * Async iterable filename pattern matcher
 *
 * @param {string} dir
 * @param {string} pattern
 * @param {Options} [options]
 * @returns {AsyncIterable<string>}
 */
async function * glob (dir, pattern, options = {}) {
  const absoluteDir = path.resolve(dir)
  const relativeDir = path.relative(options.cwd || process.cwd(), dir)

  const stats = await fs.stat(absoluteDir)

  if (stats.isDirectory()) {
    for await (const entry of _glob(absoluteDir, '', pattern, options)) {
      yield entry
    }

    return
  }

  if (minimatch(relativeDir, pattern, options)) {
    yield options.absolute ? absoluteDir : relativeDir
  }
}

/**
 * @param {string} base
 * @param {string} dir
 * @param {Glob} pattern
 * @param {Options} options
 * @returns {AsyncIterable<string>}
 */
async function * _glob (base, dir, pattern, options) {
  for await (const entry of await fs.opendir(path.join(base, dir))) {
    const relativeEntryPath = path.join(dir, entry.name)
    const absoluteEntryPath = path.join(base, dir, entry.name)

    let match = minimatch(relativeEntryPath, pattern, options)

    const isDirectory = entry.isDirectory()

    if (isDirectory && options.nodir) {
      match = false
    }

    if (match) {
      yield options.absolute ? absoluteEntryPath : relativeEntryPath
    }

    if (isDirectory) {
      yield * _glob(base, relativeEntryPath, pattern, options)
    }
  }
}

module.exports = glob
