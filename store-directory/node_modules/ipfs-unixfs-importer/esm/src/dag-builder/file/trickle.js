import batch from 'it-batch';
async function trickleStream(source, reduce, options) {
  const root = new Root(options.layerRepeat);
  let iteration = 0;
  let maxDepth = 1;
  let subTree = root;
  for await (const layer of batch(source, options.maxChildrenPerNode)) {
    if (subTree.isFull()) {
      if (subTree !== root) {
        root.addChild(await subTree.reduce(reduce));
      }
      if (iteration && iteration % options.layerRepeat === 0) {
        maxDepth++;
      }
      subTree = new SubTree(maxDepth, options.layerRepeat, iteration);
      iteration++;
    }
    subTree.append(layer);
  }
  if (subTree && subTree !== root) {
    root.addChild(await subTree.reduce(reduce));
  }
  return root.reduce(reduce);
}
export default trickleStream;
class SubTree {
  constructor(maxDepth, layerRepeat, iteration = 0) {
    this.maxDepth = maxDepth;
    this.layerRepeat = layerRepeat;
    this.currentDepth = 1;
    this.iteration = iteration;
    this.root = this.node = this.parent = {
      children: [],
      depth: this.currentDepth,
      maxDepth,
      maxChildren: (this.maxDepth - this.currentDepth) * this.layerRepeat
    };
  }
  isFull() {
    if (!this.root.data) {
      return false;
    }
    if (this.currentDepth < this.maxDepth && this.node.maxChildren) {
      this._addNextNodeToParent(this.node);
      return false;
    }
    const distantRelative = this._findParent(this.node, this.currentDepth);
    if (distantRelative) {
      this._addNextNodeToParent(distantRelative);
      return false;
    }
    return true;
  }
  _addNextNodeToParent(parent) {
    this.parent = parent;
    const nextNode = {
      children: [],
      depth: parent.depth + 1,
      parent,
      maxDepth: this.maxDepth,
      maxChildren: Math.floor(parent.children.length / this.layerRepeat) * this.layerRepeat
    };
    parent.children.push(nextNode);
    this.currentDepth = nextNode.depth;
    this.node = nextNode;
  }
  append(layer) {
    this.node.data = layer;
  }
  reduce(reduce) {
    return this._reduce(this.root, reduce);
  }
  async _reduce(node, reduce) {
    let children = [];
    if (node.children.length) {
      children = await Promise.all(node.children.filter(child => child.data).map(child => this._reduce(child, reduce)));
    }
    return reduce((node.data || []).concat(children));
  }
  _findParent(node, depth) {
    const parent = node.parent;
    if (!parent || parent.depth === 0) {
      return;
    }
    if (parent.children.length === parent.maxChildren || !parent.maxChildren) {
      return this._findParent(parent, depth);
    }
    return parent;
  }
}
class Root extends SubTree {
  constructor(layerRepeat) {
    super(0, layerRepeat);
    this.root.depth = 0;
    this.currentDepth = 1;
  }
  addChild(child) {
    this.root.children.push(child);
  }
  reduce(reduce) {
    return reduce((this.root.data || []).concat(this.root.children));
  }
}