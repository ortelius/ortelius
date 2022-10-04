class Dir {
  constructor(props, options) {
    this.options = options || {};
    this.root = props.root;
    this.dir = props.dir;
    this.path = props.path;
    this.dirty = props.dirty;
    this.flat = props.flat;
    this.parent = props.parent;
    this.parentKey = props.parentKey;
    this.unixfs = props.unixfs;
    this.mode = props.mode;
    this.mtime = props.mtime;
    this.cid = undefined;
    this.size = undefined;
  }
  async put(name, value) {
  }
  get(name) {
    return Promise.resolve(this);
  }
  async *eachChildSeries() {
  }
  async *flush(blockstore) {
  }
}
export default Dir;