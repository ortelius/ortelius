import test from 'ava'
import Path from 'path'
import process from 'process'
import os from 'os'
import fs from 'graceful-fs'
import unlimited from 'unlimited'
import { promisify } from 'util'
import { filesFromPath, getFilesFromPath } from '../src/index.js'

// https://github.com/isaacs/node-graceful-fs/issues/160
const fsMkdir = promisify(fs.mkdir)
const fsWriteFile = promisify(fs.writeFile)
const fsRm = promisify(fs.rm)

test('yields files from fixtures folder', async t => {
  const files = []
  for await (const f of filesFromPath(`${process.cwd()}/test/fixtures`)) {
    files.push(f)
  }

  t.true(files.length === 2)
})

test('gets files from fixtures folder', async t => {
  const files = await getFilesFromPath(`${process.cwd()}/test/fixtures`)

  t.true(files.length === 2)
})

test('removes custom prefix', async t => {
  const files = await getFilesFromPath(`${process.cwd()}/test/fixtures`)

  const pathPrefix = Path.join(process.cwd(), 'test', 'fixtures')
  const filesWithoutPrefix = await getFilesFromPath(`${process.cwd()}/test/fixtures`, { pathPrefix })

  files.forEach(f => {
    t.true(f.name.includes('fixtures'))
  })

  filesWithoutPrefix.forEach(f => {
    t.false(f.name.includes('fixtures'))
  })
})

test('allows read of more files than ulimit maxfiles', async t => {
  const totalFiles = 256
  const dir = await generateTestData(totalFiles)

  try {
    const files = await getFilesFromPath(dir)
    t.is(files.length, totalFiles)

    // Restrict open files to less than the total files we'll read.
    unlimited(totalFiles - 1)

    // Make sure we can read ALL of these files at the same time.
    await t.notThrowsAsync(() => Promise.all(files.map(async f => {
      for await (const _ of f.stream()) { // eslint-disable-line no-unused-vars
        // make slow so we open all the files
        await new Promise(resolve => setTimeout(resolve, 5000))
      }
    })))
  } finally {
    await fsRm(dir, { recursive: true, force: true })
  }
})

async function generateTestData (n) {
  const dirName = Path.join(os.tmpdir(), `files-from-path-test-${Date.now()}`)
  await fsMkdir(dirName)
  for (let i = 0; i < n; i++) {
    await fsWriteFile(Path.join(dirName, `${i}.json`), '{}')
  }
  return dirName
}
