'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

function formatMtime(mtime) {
  if (mtime == null) {
    return '-';
  }
  const date = new Date(mtime.secs * 1000 + Math.round((mtime.nsecs || 0) / 1000));
  return date.toLocaleDateString(Intl.DateTimeFormat().resolvedOptions().locale, {
    year: 'numeric',
    month: 'short',
    day: 'numeric',
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit',
    timeZoneName: 'short'
  });
}

exports.formatMtime = formatMtime;
