"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.listRootsInCar = exports.listCidsInCar = exports.listFilesInCar = void 0;
const fs_1 = __importDefault(require("fs"));
const car_1 = require("@ipld/car");
const unpack_1 = require("../unpack");
async function listFilesInCar({ input }) {
    const carReader = await car_1.CarIndexedReader.fromFile(input);
    for await (const file of (0, unpack_1.unpack)(carReader)) {
        // tslint:disable-next-line: no-console
        console.log(file.path);
    }
}
exports.listFilesInCar = listFilesInCar;
async function listCidsInCar({ input }) {
    const carIterator = await car_1.CarCIDIterator.fromIterable(fs_1.default.createReadStream(input));
    for await (const cid of carIterator) {
        // tslint:disable-next-line: no-console
        console.log(cid.toString());
    }
}
exports.listCidsInCar = listCidsInCar;
async function listRootsInCar({ input }) {
    const carIterator = await car_1.CarCIDIterator.fromIterable(fs_1.default.createReadStream(input));
    const roots = await carIterator.getRoots();
    for (const root of roots) {
        // tslint:disable-next-line: no-console
        console.log(root.toString());
    }
}
exports.listRootsInCar = listRootsInCar;
