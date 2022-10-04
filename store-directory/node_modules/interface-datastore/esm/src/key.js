import { nanoid } from 'nanoid';
import { toString as uint8ArrayToString } from 'uint8arrays/to-string';
import { fromString as uint8ArrayFromString } from 'uint8arrays/from-string';
const pathSepS = '/';
const pathSepB = new TextEncoder().encode(pathSepS);
const pathSep = pathSepB[0];
export class Key {
  constructor(s, clean) {
    if (typeof s === 'string') {
      this._buf = uint8ArrayFromString(s);
    } else if (s instanceof Uint8Array) {
      this._buf = s;
    } else {
      throw new Error('Invalid key, should be String of Uint8Array');
    }
    if (clean == null) {
      clean = true;
    }
    if (clean) {
      this.clean();
    }
    if (this._buf.byteLength === 0 || this._buf[0] !== pathSep) {
      throw new Error('Invalid key');
    }
  }
  toString(encoding = 'utf8') {
    return uint8ArrayToString(this._buf, encoding);
  }
  uint8Array() {
    return this._buf;
  }
  get [Symbol.toStringTag]() {
    return `Key(${ this.toString() })`;
  }
  static withNamespaces(list) {
    return new Key(list.join(pathSepS));
  }
  static random() {
    return new Key(nanoid().replace(/-/g, ''));
  }
  static asKey(other) {
    if (other instanceof Uint8Array || typeof other === 'string') {
      return new Key(other);
    }
    if (other.uint8Array) {
      return new Key(other.uint8Array());
    }
    return null;
  }
  clean() {
    if (!this._buf || this._buf.byteLength === 0) {
      this._buf = pathSepB;
    }
    if (this._buf[0] !== pathSep) {
      const bytes = new Uint8Array(this._buf.byteLength + 1);
      bytes.fill(pathSep, 0, 1);
      bytes.set(this._buf, 1);
      this._buf = bytes;
    }
    while (this._buf.byteLength > 1 && this._buf[this._buf.byteLength - 1] === pathSep) {
      this._buf = this._buf.subarray(0, -1);
    }
  }
  less(key) {
    const list1 = this.list();
    const list2 = key.list();
    for (let i = 0; i < list1.length; i++) {
      if (list2.length < i + 1) {
        return false;
      }
      const c1 = list1[i];
      const c2 = list2[i];
      if (c1 < c2) {
        return true;
      } else if (c1 > c2) {
        return false;
      }
    }
    return list1.length < list2.length;
  }
  reverse() {
    return Key.withNamespaces(this.list().slice().reverse());
  }
  namespaces() {
    return this.list();
  }
  baseNamespace() {
    const ns = this.namespaces();
    return ns[ns.length - 1];
  }
  list() {
    return this.toString().split(pathSepS).slice(1);
  }
  type() {
    return namespaceType(this.baseNamespace());
  }
  name() {
    return namespaceValue(this.baseNamespace());
  }
  instance(s) {
    return new Key(this.toString() + ':' + s);
  }
  path() {
    let p = this.parent().toString();
    if (!p.endsWith(pathSepS)) {
      p += pathSepS;
    }
    p += this.type();
    return new Key(p);
  }
  parent() {
    const list = this.list();
    if (list.length === 1) {
      return new Key(pathSepS);
    }
    return new Key(list.slice(0, -1).join(pathSepS));
  }
  child(key) {
    if (this.toString() === pathSepS) {
      return key;
    } else if (key.toString() === pathSepS) {
      return this;
    }
    return new Key(this.toString() + key.toString(), false);
  }
  isAncestorOf(other) {
    if (other.toString() === this.toString()) {
      return false;
    }
    return other.toString().startsWith(this.toString());
  }
  isDecendantOf(other) {
    if (other.toString() === this.toString()) {
      return false;
    }
    return this.toString().startsWith(other.toString());
  }
  isTopLevel() {
    return this.list().length === 1;
  }
  concat(...keys) {
    return Key.withNamespaces([
      ...this.namespaces(),
      ...flatten(keys.map(key => key.namespaces()))
    ]);
  }
}
function namespaceType(ns) {
  const parts = ns.split(':');
  if (parts.length < 2) {
    return '';
  }
  return parts.slice(0, -1).join(':');
}
function namespaceValue(ns) {
  const parts = ns.split(':');
  return parts[parts.length - 1];
}
function flatten(arr) {
  return [].concat(...arr);
}