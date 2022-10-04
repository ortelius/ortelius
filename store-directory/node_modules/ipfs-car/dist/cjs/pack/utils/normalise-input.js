"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.getNormaliser = void 0;
const normalise_input_single_1 = require("ipfs-core-utils/files/normalise-input-single");
const normalise_input_multiple_1 = require("ipfs-core-utils/files/normalise-input-multiple");
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
function getNormaliser(input) {
    if (isSingle(input)) {
        return (0, normalise_input_single_1.normaliseInput)(input);
    }
    else {
        return (0, normalise_input_multiple_1.normaliseInput)(input);
    }
}
exports.getNormaliser = getNormaliser;
