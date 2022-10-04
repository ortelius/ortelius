import {
  encode,
  prepare
} from '@ipld/dag-pb';
import { UnixFS } from 'ipfs-unixfs';
import Dir from './dir.js';
import persist from './utils/persist.js';
class DirFlat extends Dir {
  constructor(props, options) {
    super(props, options);
    this._children = {};
  }
  async put(name, value) {
    this.cid = undefined;
    this.size = undefined;
    this._children[name] = value;
  }
  get(name) {
    return Promise.resolve(this._children[name]);
  }
  childCount() {
    return Object.keys(this._children).length;
  }
  directChildrenCount() {
    return this.childCount();
  }
  onlyChild() {
    return this._children[Object.keys(this._children)[0]];
  }
  async *eachChildSeries() {
    const keys = Object.keys(this._children);
    for (let i = 0; i < keys.length; i++) {
      const key = keys[i];
      yield {
        key: key,
        child: this._children[key]
      };
    }
  }
  async *flush(block) {
    const children = Object.keys(this._children);
    const links = [];
    for (let i = 0; i < children.length; i++) {
      let child = this._children[children[i]];
      if (child instanceof Dir) {
        for await (const entry of child.flush(block)) {
          child = entry;
          yield child;
        }
      }
      if (child.size != null && child.cid) {
        links.push({
          Name: children[i],
          Tsize: child.size,
          Hash: child.cid
        });
      }
    }
    const unixfs = new UnixFS({
      type: 'directory',
      mtime: this.mtime,
      mode: this.mode
    });
    const node = {
      Data: unixfs.marshal(),
      Links: links
    };
    const buffer = encode(prepare(node));
    const cid = await persist(buffer, block, this.options);
    const size = buffer.length + node.Links.reduce((acc, curr) => acc + (curr.Tsize == null ? 0 : curr.Tsize), 0);
    this.cid = cid;
    this.size = size;
    yield {
      cid,
      unixfs,
      path: this.path,
      size
    };
  }
}
export default DirFlat;