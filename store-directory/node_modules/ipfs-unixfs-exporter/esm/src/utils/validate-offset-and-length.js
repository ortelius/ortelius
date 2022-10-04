import errCode from 'err-code';
const validateOffsetAndLength = (size, offset, length) => {
  if (!offset) {
    offset = 0;
  }
  if (offset < 0) {
    throw errCode(new Error('Offset must be greater than or equal to 0'), 'ERR_INVALID_PARAMS');
  }
  if (offset > size) {
    throw errCode(new Error('Offset must be less than the file size'), 'ERR_INVALID_PARAMS');
  }
  if (!length && length !== 0) {
    length = size - offset;
  }
  if (length < 0) {
    throw errCode(new Error('Length must be greater than or equal to 0'), 'ERR_INVALID_PARAMS');
  }
  if (offset + length > size) {
    length = size - offset;
  }
  return {
    offset,
    length
  };
};
export default validateOffsetAndLength;