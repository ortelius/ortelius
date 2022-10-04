const last = require('./')
const test = require('ava')

test('Should return only the last result from an async iterator', async (t) => {
  const values = [0, 1, 2, 3, 4]

  const res = await last(values)

  t.is(res, 4)
})
