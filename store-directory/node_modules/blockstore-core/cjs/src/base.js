'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var drain = require('it-drain');
var filter = require('it-filter');
var take = require('it-take');
var all = require('it-all');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var drain__default = /*#__PURE__*/_interopDefaultLegacy(drain);
var filter__default = /*#__PURE__*/_interopDefaultLegacy(filter);
var take__default = /*#__PURE__*/_interopDefaultLegacy(take);
var all__default = /*#__PURE__*/_interopDefaultLegacy(all);

const sortAll = (iterable, sorter) => {
  return async function* () {
    const values = await all__default["default"](iterable);
    yield* values.sort(sorter);
  }();
};
class BaseBlockstore {
  open() {
    return Promise.reject(new Error('.open is not implemented'));
  }
  close() {
    return Promise.reject(new Error('.close is not implemented'));
  }
  put(key, val, options) {
    return Promise.reject(new Error('.put is not implemented'));
  }
  get(key, options) {
    return Promise.reject(new Error('.get is not implemented'));
  }
  has(key, options) {
    return Promise.reject(new Error('.has is not implemented'));
  }
  delete(key, options) {
    return Promise.reject(new Error('.delete is not implemented'));
  }
  async *putMany(source, options = {}) {
    for await (const {key, value} of source) {
      await this.put(key, value, options);
      yield {
        key,
        value
      };
    }
  }
  async *getMany(source, options = {}) {
    for await (const key of source) {
      yield this.get(key, options);
    }
  }
  async *deleteMany(source, options = {}) {
    for await (const key of source) {
      await this.delete(key, options);
      yield key;
    }
  }
  batch() {
    let puts = [];
    let dels = [];
    return {
      put(key, value) {
        puts.push({
          key,
          value
        });
      },
      delete(key) {
        dels.push(key);
      },
      commit: async options => {
        await drain__default["default"](this.putMany(puts, options));
        puts = [];
        await drain__default["default"](this.deleteMany(dels, options));
        dels = [];
      }
    };
  }
  async *_all(q, options) {
    throw new Error('._all is not implemented');
  }
  async *_allKeys(q, options) {
    throw new Error('._allKeys is not implemented');
  }
  query(q, options) {
    let it = this._all(q, options);
    if (q.prefix != null) {
      it = filter__default["default"](it, e => e.key.toString().startsWith(q.prefix || ''));
    }
    if (Array.isArray(q.filters)) {
      it = q.filters.reduce((it, f) => filter__default["default"](it, f), it);
    }
    if (Array.isArray(q.orders)) {
      it = q.orders.reduce((it, f) => sortAll(it, f), it);
    }
    if (q.offset != null) {
      let i = 0;
      it = filter__default["default"](it, () => i++ >= (q.offset || 0));
    }
    if (q.limit != null) {
      it = take__default["default"](it, q.limit);
    }
    return it;
  }
  queryKeys(q, options) {
    let it = this._allKeys(q, options);
    if (q.prefix != null) {
      it = filter__default["default"](it, cid => cid.toString().startsWith(q.prefix || ''));
    }
    if (Array.isArray(q.filters)) {
      it = q.filters.reduce((it, f) => filter__default["default"](it, f), it);
    }
    if (Array.isArray(q.orders)) {
      it = q.orders.reduce((it, f) => sortAll(it, f), it);
    }
    if (q.offset != null) {
      let i = 0;
      it = filter__default["default"](it, () => i++ >= q.offset);
    }
    if (q.limit != null) {
      it = take__default["default"](it, q.limit);
    }
    return it;
  }
}

exports.BaseBlockstore = BaseBlockstore;
