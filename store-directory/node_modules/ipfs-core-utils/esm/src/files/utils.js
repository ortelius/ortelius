export function isBytes(obj) {
  return ArrayBuffer.isView(obj) || obj instanceof ArrayBuffer;
}
export function isBlob(obj) {
  return obj.constructor && (obj.constructor.name === 'Blob' || obj.constructor.name === 'File') && typeof obj.stream === 'function';
}
export function isFileObject(obj) {
  return typeof obj === 'object' && (obj.path || obj.content);
}
export const isReadableStream = value => value && typeof value.getReader === 'function';