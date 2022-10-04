import DirSharded from './dir-sharded.js';
import DirFlat from './dir-flat.js';
async function flatToShard(child, dir, threshold, options) {
  let newDir = dir;
  if (dir instanceof DirFlat && dir.directChildrenCount() >= threshold) {
    newDir = await convertToShard(dir, options);
  }
  const parent = newDir.parent;
  if (parent) {
    if (newDir !== dir) {
      if (child) {
        child.parent = newDir;
      }
      if (!newDir.parentKey) {
        throw new Error('No parent key found');
      }
      await parent.put(newDir.parentKey, newDir);
    }
    return flatToShard(newDir, parent, threshold, options);
  }
  return newDir;
}
async function convertToShard(oldDir, options) {
  const newDir = new DirSharded({
    root: oldDir.root,
    dir: true,
    parent: oldDir.parent,
    parentKey: oldDir.parentKey,
    path: oldDir.path,
    dirty: oldDir.dirty,
    flat: false,
    mtime: oldDir.mtime,
    mode: oldDir.mode
  }, options);
  for await (const {key, child} of oldDir.eachChildSeries()) {
    await newDir.put(key, child);
  }
  return newDir;
}
export default flatToShard;