// @ts-check
"use strict"

import * as polyfill from "./form-data.js"

// Electron-renderer should get the browser implementation instead of node
// which is why we check global first.
export const FormData =
  typeof globalThis.FormData === "function"
    ? globalThis.FormData
    : polyfill.FormData
