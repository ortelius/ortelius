import { normaliseInput as normaliseInputSingle } from 'ipfs-core-utils/files/normalise-input-single';
import { normaliseInput as normaliseInputMultiple } from 'ipfs-core-utils/files/normalise-input-multiple';
function isBytes(obj) {
    return ArrayBuffer.isView(obj) || obj instanceof ArrayBuffer;
}
function isBlob(obj) {
    return Boolean(obj.constructor) &&
        (obj.constructor.name === 'Blob' || obj.constructor.name === 'File') &&
        typeof obj.stream === 'function';
}
function isSingle(input) {
    return typeof input === 'string' || input instanceof String || isBytes(input) || isBlob(input) || '_readableState' in input;
}
/**
 * Get a single or multiple normaliser depending on the input.
 */
export function getNormaliser(input) {
    if (isSingle(input)) {
        return normaliseInputSingle(input);
    }
    else {
        return normaliseInputMultiple(input);
    }
}
