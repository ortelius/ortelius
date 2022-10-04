import all from 'it-all';
async function flat(source, reduce) {
  return reduce(await all(source));
}
export default flat;