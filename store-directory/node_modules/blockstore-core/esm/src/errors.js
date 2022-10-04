import errCode from 'err-code';
export function notFoundError(err) {
  err = err || new Error('Not Found');
  return errCode(err, 'ERR_NOT_FOUND');
}
export function abortedError(err) {
  err = err || new Error('Aborted');
  return errCode(err, 'ERR_ABORTED');
}