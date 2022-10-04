(function(f){if(typeof exports==="object"&&typeof module!=="undefined"){module.exports=f()}else if(typeof define==="function"&&define.amd){define([],f)}else{var g;if(typeof window!=="undefined"){g=window}else if(typeof global!=="undefined"){g=global}else if(typeof self!=="undefined"){g=self}else{g=this}g.Rabin = f()}})(function(){var define,module,exports;return (function(){function r(e,n,t){function o(i,f){if(!n[i]){if(!e[i]){var c="function"==typeof require&&require;if(!f&&c)return c(i,!0);if(u)return u(i,!0);var a=new Error("Cannot find module '"+i+"'");throw a.code="MODULE_NOT_FOUND",a}var p=n[i]={exports:{}};e[i][0].call(p.exports,function(r){var n=e[i][1][r];return o(n||r)},p,p.exports,r,e,n,t)}return n[i].exports}for(var u="function"==typeof require&&require,i=0;i<t.length;i++)o(t[i]);return o}return r})()({1:[function(require,module,exports){

const { instantiate } = require("@assemblyscript/loader");

loadWebAssembly.supported = typeof WebAssembly !== 'undefined'

function loadWebAssembly (imp = {}) {
  if (!loadWebAssembly.supported) return null
  
  var wasm = new Uint8Array([0,97,115,109,1,0,0,0,1,78,14,96,2,127,126,0,96,1,127,1,126,96,2,127,127,0,96,1,127,1,127,96,1,127,0,96,2,127,127,1,127,96,3,127,127,127,1,127,96,0,0,96,3,127,127,127,0,96,0,1,127,96,4,127,127,127,127,0,96,5,127,127,127,127,127,1,127,96,1,126,1,127,96,2,126,126,1,126,2,13,1,3,101,110,118,5,97,98,111,114,116,0,10,3,54,53,2,2,8,9,3,5,2,8,6,5,3,4,2,6,9,12,13,2,5,11,3,2,3,2,3,2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,6,7,7,4,4,5,3,1,0,1,6,47,9,127,1,65,0,11,127,1,65,0,11,127,0,65,3,11,127,0,65,4,11,127,1,65,0,11,127,1,65,0,11,127,1,65,0,11,127,0,65,240,2,11,127,0,65,6,11,7,240,5,41,6,109,101,109,111,114,121,2,0,7,95,95,97,108,108,111,99,0,10,8,95,95,114,101,116,97,105,110,0,11,9,95,95,114,101,108,101,97,115,101,0,12,9,95,95,99,111,108,108,101,99,116,0,51,11,95,95,114,116,116,105,95,98,97,115,101,3,7,13,73,110,116,51,50,65,114,114,97,121,95,73,68,3,2,13,85,105,110,116,56,65,114,114,97,121,95,73,68,3,3,6,100,101,103,114,101,101,0,16,3,109,111,100,0,17,5,82,97,98,105,110,3,8,16,82,97,98,105,110,35,103,101,116,58,119,105,110,100,111,119,0,21,16,82,97,98,105,110,35,115,101,116,58,119,105,110,100,111,119,0,22,21,82,97,98,105,110,35,103,101,116,58,119,105,110,100,111,119,95,115,105,122,101,0,23,21,82,97,98,105,110,35,115,101,116,58,119,105,110,100,111,119,95,115,105,122,101,0,24,14,82,97,98,105,110,35,103,101,116,58,119,112,111,115,0,25,14,82,97,98,105,110,35,115,101,116,58,119,112,111,115,0,26,15,82,97,98,105,110,35,103,101,116,58,99,111,117,110,116,0,27,15,82,97,98,105,110,35,115,101,116,58,99,111,117,110,116,0,28,13,82,97,98,105,110,35,103,101,116,58,112,111,115,0,29,13,82,97,98,105,110,35,115,101,116,58,112,111,115,0,30,15,82,97,98,105,110,35,103,101,116,58,115,116,97,114,116,0,31,15,82,97,98,105,110,35,115,101,116,58,115,116,97,114,116,0,32,16,82,97,98,105,110,35,103,101,116,58,100,105,103,101,115,116,0,33,16,82,97,98,105,110,35,115,101,116,58,100,105,103,101,115,116,0,34,21,82,97,98,105,110,35,103,101,116,58,99,104,117,110,107,95,115,116,97,114,116,0,35,21,82,97,98,105,110,35,115,101,116,58,99,104,117,110,107,95,115,116,97,114,116,0,36,22,82,97,98,105,110,35,103,101,116,58,99,104,117,110,107,95,108,101,110,103,116,104,0,37,22,82,97,98,105,110,35,115,101,116,58,99,104,117,110,107,95,108,101,110,103,116,104,0,38,31,82,97,98,105,110,35,103,101,116,58,99,104,117,110,107,95,99,117,116,95,102,105,110,103,101,114,112,114,105,110,116,0,39,31,82,97,98,105,110,35,115,101,116,58,99,104,117,110,107,95,99,117,116,95,102,105,110,103,101,114,112,114,105,110,116,0,40,20,82,97,98,105,110,35,103,101,116,58,112,111,108,121,110,111,109,105,97,108,0,41,20,82,97,98,105,110,35,115,101,116,58,112,111,108,121,110,111,109,105,97,108,0,42,17,82,97,98,105,110,35,103,101,116,58,109,105,110,115,105,122,101,0,43,17,82,97,98,105,110,35,115,101,116,58,109,105,110,115,105,122,101,0,44,17,82,97,98,105,110,35,103,101,116,58,109,97,120,115,105,122,101,0,45,17,82,97,98,105,110,35,115,101,116,58,109,97,120,115,105,122,101,0,46,14,82,97,98,105,110,35,103,101,116,58,109,97,115,107,0,47,14,82,97,98,105,110,35,115,101,116,58,109,97,115,107,0,48,17,82,97,98,105,110,35,99,111,110,115,116,114,117,99,116,111,114,0,20,17,82,97,98,105,110,35,102,105,110,103,101,114,112,114,105,110,116,0,49,8,1,50,10,165,31,53,199,1,1,4,127,32,1,40,2,0,65,124,113,34,2,65,128,2,73,4,127,32,2,65,4,118,33,4,65,0,5,32,2,65,31,32,2,103,107,34,3,65,4,107,118,65,16,115,33,4,32,3,65,7,107,11,33,3,32,1,40,2,20,33,2,32,1,40,2,16,34,5,4,64,32,5,32,2,54,2,20,11,32,2,4,64,32,2,32,5,54,2,16,11,32,1,32,0,32,4,32,3,65,4,116,106,65,2,116,106,40,2,96,70,4,64,32,0,32,4,32,3,65,4,116,106,65,2,116,106,32,2,54,2,96,32,2,69,4,64,32,0,32,3,65,2,116,106,32,0,32,3,65,2,116,106,40,2,4,65,1,32,4,116,65,127,115,113,34,1,54,2,4,32,1,69,4,64,32,0,32,0,40,2,0,65,1,32,3,116,65,127,115,113,54,2,0,11,11,11,11,226,2,1,6,127,32,1,40,2,0,33,3,32,1,65,16,106,32,1,40,2,0,65,124,113,106,34,4,40,2,0,34,5,65,1,113,4,64,32,3,65,124,113,65,16,106,32,5,65,124,113,106,34,2,65,240,255,255,255,3,73,4,64,32,0,32,4,16,1,32,1,32,2,32,3,65,3,113,114,34,3,54,2,0,32,1,65,16,106,32,1,40,2,0,65,124,113,106,34,4,40,2,0,33,5,11,11,32,3,65,2,113,4,64,32,1,65,4,107,40,2,0,34,2,40,2,0,34,6,65,124,113,65,16,106,32,3,65,124,113,106,34,7,65,240,255,255,255,3,73,4,64,32,0,32,2,16,1,32,2,32,7,32,6,65,3,113,114,34,3,54,2,0,32,2,33,1,11,11,32,4,32,5,65,2,114,54,2,0,32,4,65,4,107,32,1,54,2,0,32,0,32,3,65,124,113,34,2,65,128,2,73,4,127,32,2,65,4,118,33,4,65,0,5,32,2,65,31,32,2,103,107,34,2,65,4,107,118,65,16,115,33,4,32,2,65,7,107,11,34,3,65,4,116,32,4,106,65,2,116,106,40,2,96,33,2,32,1,65,0,54,2,16,32,1,32,2,54,2,20,32,2,4,64,32,2,32,1,54,2,16,11,32,0,32,4,32,3,65,4,116,106,65,2,116,106,32,1,54,2,96,32,0,32,0,40,2,0,65,1,32,3,116,114,54,2,0,32,0,32,3,65,2,116,106,32,0,32,3,65,2,116,106,40,2,4,65,1,32,4,116,114,54,2,4,11,119,1,1,127,32,2,2,127,32,0,40,2,160,12,34,2,4,64,32,2,32,1,65,16,107,70,4,64,32,2,40,2,0,33,3,32,1,65,16,107,33,1,11,11,32,1,11,107,34,2,65,48,73,4,64,15,11,32,1,32,3,65,2,113,32,2,65,32,107,65,1,114,114,54,2,0,32,1,65,0,54,2,16,32,1,65,0,54,2,20,32,1,32,2,106,65,16,107,34,2,65,2,54,2,0,32,0,32,2,54,2,160,12,32,0,32,1,16,2,11,155,1,1,3,127,35,0,34,0,69,4,64,65,1,63,0,34,0,74,4,127,65,1,32,0,107,64,0,65,0,72,5,65,0,11,4,64,0,11,65,176,3,34,0,65,0,54,2,0,65,208,15,65,0,54,2,0,3,64,32,1,65,23,73,4,64,32,1,65,2,116,65,176,3,106,65,0,54,2,4,65,0,33,2,3,64,32,2,65,16,73,4,64,32,1,65,4,116,32,2,106,65,2,116,65,176,3,106,65,0,54,2,96,32,2,65,1,106,33,2,12,1,11,11,32,1,65,1,106,33,1,12,1,11,11,65,176,3,65,224,15,63,0,65,16,116,16,3,65,176,3,36,0,11,32,0,11,45,0,32,0,65,240,255,255,255,3,79,4,64,65,32,65,224,0,65,201,3,65,29,16,0,0,11,32,0,65,15,106,65,112,113,34,0,65,16,32,0,65,16,75,27,11,169,1,1,1,127,32,0,32,1,65,128,2,73,4,127,32,1,65,4,118,33,1,65,0,5,32,1,65,248,255,255,255,1,73,4,64,32,1,65,1,65,27,32,1,103,107,116,106,65,1,107,33,1,11,32,1,65,31,32,1,103,107,34,2,65,4,107,118,65,16,115,33,1,32,2,65,7,107,11,34,2,65,2,116,106,40,2,4,65,127,32,1,116,113,34,1,4,127,32,0,32,1,104,32,2,65,4,116,106,65,2,116,106,40,2,96,5,32,0,40,2,0,65,127,32,2,65,1,106,116,113,34,1,4,127,32,0,32,0,32,1,104,34,0,65,2,116,106,40,2,4,104,32,0,65,4,116,106,65,2,116,106,40,2,96,5,65,0,11,11,11,111,1,1,127,63,0,34,2,32,1,65,248,255,255,255,1,73,4,127,32,1,65,1,65,27,32,1,103,107,116,65,1,107,106,5,32,1,11,65,16,32,0,40,2,160,12,32,2,65,16,116,65,16,107,71,116,106,65,255,255,3,106,65,128,128,124,113,65,16,118,34,1,32,2,32,1,74,27,64,0,65,0,72,4,64,32,1,64,0,65,0,72,4,64,0,11,11,32,0,32,2,65,16,116,63,0,65,16,116,16,3,11,113,1,2,127,32,1,40,2,0,34,3,65,124,113,32,2,107,34,4,65,32,79,4,64,32,1,32,2,32,3,65,2,113,114,54,2,0,32,2,32,1,65,16,106,106,34,1,32,4,65,16,107,65,1,114,54,2,0,32,0,32,1,16,2,5,32,1,32,3,65,126,113,54,2,0,32,1,65,16,106,32,1,40,2,0,65,124,113,106,32,1,65,16,106,32,1,40,2,0,65,124,113,106,40,2,0,65,125,113,54,2,0,11,11,91,1,2,127,32,0,32,1,16,5,34,4,16,6,34,3,69,4,64,65,1,36,1,65,0,36,1,32,0,32,4,16,6,34,3,69,4,64,32,0,32,4,16,7,32,0,32,4,16,6,33,3,11,11,32,3,65,0,54,2,4,32,3,32,2,54,2,8,32,3,32,1,54,2,12,32,0,32,3,16,1,32,0,32,3,32,4,16,8,32,3,11,13,0,16,4,32,0,32,1,16,9,65,16,106,11,33,1,1,127,32,0,65,172,3,75,4,64,32,0,65,16,107,34,1,32,1,40,2,4,65,1,106,54,2,4,11,32,0,11,18,0,32,0,65,172,3,75,4,64,32,0,65,16,107,16,52,11,11,140,3,1,1,127,2,64,32,1,69,13,0,32,0,65,0,58,0,0,32,0,32,1,106,65,1,107,65,0,58,0,0,32,1,65,2,77,13,0,32,0,65,1,106,65,0,58,0,0,32,0,65,2,106,65,0,58,0,0,32,0,32,1,106,34,2,65,2,107,65,0,58,0,0,32,2,65,3,107,65,0,58,0,0,32,1,65,6,77,13,0,32,0,65,3,106,65,0,58,0,0,32,0,32,1,106,65,4,107,65,0,58,0,0,32,1,65,8,77,13,0,32,1,65,0,32,0,107,65,3,113,34,1,107,33,2,32,0,32,1,106,34,0,65,0,54,2,0,32,0,32,2,65,124,113,34,1,106,65,4,107,65,0,54,2,0,32,1,65,8,77,13,0,32,0,65,4,106,65,0,54,2,0,32,0,65,8,106,65,0,54,2,0,32,0,32,1,106,34,2,65,12,107,65,0,54,2,0,32,2,65,8,107,65,0,54,2,0,32,1,65,24,77,13,0,32,0,65,12,106,65,0,54,2,0,32,0,65,16,106,65,0,54,2,0,32,0,65,20,106,65,0,54,2,0,32,0,65,24,106,65,0,54,2,0,32,0,32,1,106,34,2,65,28,107,65,0,54,2,0,32,2,65,24,107,65,0,54,2,0,32,2,65,20,107,65,0,54,2,0,32,2,65,16,107,65,0,54,2,0,32,0,32,0,65,4,113,65,24,106,34,2,106,33,0,32,1,32,2,107,33,1,3,64,32,1,65,32,79,4,64,32,0,66,0,55,3,0,32,0,65,8,106,66,0,55,3,0,32,0,65,16,106,66,0,55,3,0,32,0,65,24,106,66,0,55,3,0,32,1,65,32,107,33,1,32,0,65,32,106,33,0,12,1,11,11,11,11,178,1,1,3,127,32,1,65,240,255,255,255,3,32,2,118,75,4,64,65,144,1,65,192,1,65,23,65,56,16,0,0,11,32,1,32,2,116,34,3,65,0,16,10,34,2,32,3,16,13,32,0,69,4,64,65,12,65,2,16,10,34,0,65,172,3,75,4,64,32,0,65,16,107,34,1,32,1,40,2,4,65,1,106,54,2,4,11,11,32,0,65,0,54,2,0,32,0,65,0,54,2,4,32,0,65,0,54,2,8,32,2,34,1,32,0,40,2,0,34,4,71,4,64,32,1,65,172,3,75,4,64,32,1,65,16,107,34,5,32,5,40,2,4,65,1,106,54,2,4,11,32,4,16,12,11,32,0,32,1,54,2,0,32,0,32,2,54,2,4,32,0,32,3,54,2,8,32,0,11,46,1,2,127,65,12,65,5,16,10,34,0,65,172,3,75,4,64,32,0,65,16,107,34,1,32,1,40,2,4,65,1,106,54,2,4,11,32,0,65,128,2,65,3,16,14,11,9,0,65,63,32,0,121,167,107,11,49,1,2,127,65,63,32,1,121,167,107,33,2,3,64,65,63,32,0,121,167,107,32,2,107,34,3,65,0,78,4,64,32,0,32,1,32,3,172,134,133,33,0,12,1,11,11,32,0,11,40,0,32,1,32,0,40,2,8,79,4,64,65,128,2,65,192,2,65,163,1,65,44,16,0,0,11,32,1,32,0,40,2,4,106,65,0,58,0,0,11,38,0,32,1,32,0,40,2,8,79,4,64,65,128,2,65,192,2,65,152,1,65,44,16,0,0,11,32,1,32,0,40,2,4,106,45,0,0,11,254,5,2,1,127,4,126,32,0,69,4,64,65,232,0,65,6,16,10,34,0,65,172,3,75,4,64,32,0,65,16,107,34,5,32,5,40,2,4,65,1,106,54,2,4,11,11,32,0,65,0,54,2,0,32,0,65,0,54,2,4,32,0,65,0,54,2,8,32,0,66,0,55,3,16,32,0,66,0,55,3,24,32,0,66,0,55,3,32,32,0,66,0,55,3,40,32,0,66,0,55,3,48,32,0,66,0,55,3,56,32,0,66,0,55,3,64,32,0,66,0,55,3,72,32,0,66,0,55,3,80,32,0,66,0,55,3,88,32,0,66,0,55,3,96,32,0,32,2,173,55,3,80,32,0,32,3,173,55,3,88,65,12,65,4,16,10,34,2,65,172,3,75,4,64,32,2,65,16,107,34,3,32,3,40,2,4,65,1,106,54,2,4,11,32,2,32,4,65,0,16,14,33,2,32,0,40,2,0,16,12,32,0,32,2,54,2,0,32,0,32,4,54,2,4,32,0,66,1,32,1,173,134,66,1,125,55,3,96,32,0,66,243,130,183,218,216,230,232,30,55,3,72,35,4,69,4,64,65,0,33,2,3,64,32,2,65,128,2,72,4,64,32,2,65,255,1,113,173,33,6,32,0,41,3,72,34,7,33,8,65,63,32,7,121,167,107,33,1,3,64,65,63,32,6,121,167,107,32,1,107,34,3,65,0,78,4,64,32,6,32,8,32,3,172,134,133,33,6,12,1,11,11,65,0,33,4,3,64,32,4,32,0,40,2,4,65,1,107,72,4,64,32,6,66,8,134,33,6,32,0,41,3,72,34,7,33,8,65,63,32,7,121,167,107,33,1,3,64,65,63,32,6,121,167,107,32,1,107,34,3,65,0,78,4,64,32,6,32,8,32,3,172,134,133,33,6,12,1,11,11,32,4,65,1,106,33,4,12,1,11,11,35,6,40,2,4,32,2,65,3,116,106,32,6,55,3,0,32,2,65,1,106,33,2,12,1,11,11,65,63,32,0,41,3,72,121,167,107,172,33,7,65,0,33,2,3,64,32,2,65,128,2,72,4,64,35,5,33,1,32,2,172,32,7,134,34,8,33,6,65,63,32,0,41,3,72,34,9,121,167,107,33,3,3,64,65,63,32,6,121,167,107,32,3,107,34,4,65,0,78,4,64,32,6,32,9,32,4,172,134,133,33,6,12,1,11,11,32,1,40,2,4,32,2,65,3,116,106,32,6,32,8,132,55,3,0,32,2,65,1,106,33,2,12,1,11,11,65,1,36,4,11,32,0,66,0,55,3,24,32,0,66,0,55,3,32,65,0,33,2,3,64,32,2,32,0,40,2,4,72,4,64,32,0,40,2,0,32,2,16,18,32,2,65,1,106,33,2,12,1,11,11,32,0,66,0,55,3,40,32,0,65,0,54,2,8,32,0,66,0,55,3,16,32,0,66,0,55,3,40,32,0,40,2,0,32,0,40,2,8,16,19,33,1,32,0,40,2,8,32,0,40,2,0,40,2,4,106,65,1,58,0,0,32,0,32,0,41,3,40,35,6,40,2,4,32,1,65,3,116,106,41,3,0,133,55,3,40,32,0,32,0,40,2,8,65,1,106,32,0,40,2,4,111,54,2,8,32,0,35,5,40,2,4,32,0,41,3,40,34,6,66,45,136,167,65,3,116,106,41,3,0,32,6,66,8,134,66,1,132,133,55,3,40,32,0,11,38,1,1,127,32,0,40,2,0,34,0,65,172,3,75,4,64,32,0,65,16,107,34,1,32,1,40,2,4,65,1,106,54,2,4,11,32,0,11,55,1,2,127,32,1,32,0,40,2,0,34,2,71,4,64,32,1,65,172,3,75,4,64,32,1,65,16,107,34,3,32,3,40,2,4,65,1,106,54,2,4,11,32,2,16,12,11,32,0,32,1,54,2,0,11,7,0,32,0,40,2,4,11,9,0,32,0,32,1,54,2,4,11,7,0,32,0,40,2,8,11,9,0,32,0,32,1,54,2,8,11,7,0,32,0,41,3,16,11,9,0,32,0,32,1,55,3,16,11,7,0,32,0,41,3,24,11,9,0,32,0,32,1,55,3,24,11,7,0,32,0,41,3,32,11,9,0,32,0,32,1,55,3,32,11,7,0,32,0,41,3,40,11,9,0,32,0,32,1,55,3,40,11,7,0,32,0,41,3,48,11,9,0,32,0,32,1,55,3,48,11,7,0,32,0,41,3,56,11,9,0,32,0,32,1,55,3,56,11,7,0,32,0,41,3,64,11,9,0,32,0,32,1,55,3,64,11,7,0,32,0,41,3,72,11,9,0,32,0,32,1,55,3,72,11,7,0,32,0,41,3,80,11,9,0,32,0,32,1,55,3,80,11,7,0,32,0,41,3,88,11,9,0,32,0,32,1,55,3,88,11,7,0,32,0,41,3,96,11,9,0,32,0,32,1,55,3,96,11,172,4,2,5,127,1,126,32,2,65,172,3,75,4,64,32,2,65,16,107,34,4,32,4,40,2,4,65,1,106,54,2,4,11,32,2,33,4,65,0,33,2,32,1,40,2,8,33,5,32,1,40,2,4,33,6,3,64,2,127,65,0,33,3,3,64,32,3,32,5,72,4,64,32,3,32,6,106,45,0,0,33,1,32,0,40,2,0,32,0,40,2,8,16,19,33,7,32,0,40,2,8,32,0,40,2,0,40,2,4,106,32,1,58,0,0,32,0,32,0,41,3,40,35,6,40,2,4,32,7,65,3,116,106,41,3,0,133,55,3,40,32,0,32,0,40,2,8,65,1,106,32,0,40,2,4,111,54,2,8,32,0,35,5,40,2,4,32,0,41,3,40,34,8,66,45,136,167,65,3,116,106,41,3,0,32,1,173,32,8,66,8,134,132,133,55,3,40,32,0,32,0,41,3,16,66,1,124,55,3,16,32,0,32,0,41,3,24,66,1,124,55,3,24,32,0,41,3,16,32,0,41,3,80,90,4,127,32,0,41,3,40,32,0,41,3,96,131,80,5,65,0,11,4,127,65,1,5,32,0,41,3,16,32,0,41,3,88,90,11,4,64,32,0,32,0,41,3,32,55,3,48,32,0,32,0,41,3,16,55,3,56,32,0,32,0,41,3,40,55,3,64,65,0,33,1,3,64,32,1,32,0,40,2,4,72,4,64,32,0,40,2,0,32,1,16,18,32,1,65,1,106,33,1,12,1,11,11,32,0,66,0,55,3,40,32,0,65,0,54,2,8,32,0,66,0,55,3,16,32,0,66,0,55,3,40,32,0,40,2,0,32,0,40,2,8,16,19,33,1,32,0,40,2,8,32,0,40,2,0,40,2,4,106,65,1,58,0,0,32,0,32,0,41,3,40,35,6,40,2,4,32,1,65,3,116,106,41,3,0,133,55,3,40,32,0,32,0,40,2,8,65,1,106,32,0,40,2,4,111,54,2,8,32,0,35,5,40,2,4,32,0,41,3,40,34,8,66,45,136,167,65,3,116,106,41,3,0,32,8,66,8,134,66,1,132,133,55,3,40,32,3,65,1,106,12,3,11,32,3,65,1,106,33,3,12,1,11,11,65,127,11,34,1,65,0,78,4,64,32,5,32,1,107,33,5,32,1,32,6,106,33,6,32,2,34,1,65,1,106,33,2,32,4,40,2,4,32,1,65,2,116,106,32,0,41,3,56,62,2,0,12,1,11,11,32,4,11,10,0,16,15,36,5,16,15,36,6,11,3,0,1,11,73,1,2,127,32,0,40,2,4,34,1,65,255,255,255,255,0,113,34,2,65,1,70,4,64,32,0,65,16,106,16,53,32,0,32,0,40,2,0,65,1,114,54,2,0,35,0,32,0,16,2,5,32,0,32,2,65,1,107,32,1,65,128,128,128,128,127,113,114,54,2,4,11,11,58,0,2,64,2,64,2,64,32,0,65,8,107,40,2,0,14,7,0,0,1,1,1,1,1,2,11,15,11,32,0,40,2,0,34,0,4,64,32,0,65,172,3,79,4,64,32,0,65,16,107,16,52,11,11,15,11,0,11,11,137,3,7,0,65,16,11,55,40,0,0,0,1,0,0,0,1,0,0,0,40,0,0,0,97,0,108,0,108,0,111,0,99,0,97,0,116,0,105,0,111,0,110,0,32,0,116,0,111,0,111,0,32,0,108,0,97,0,114,0,103,0,101,0,65,208,0,11,45,30,0,0,0,1,0,0,0,1,0,0,0,30,0,0,0,126,0,108,0,105,0,98,0,47,0,114,0,116,0,47,0,116,0,108,0,115,0,102,0,46,0,116,0,115,0,65,128,1,11,43,28,0,0,0,1,0,0,0,1,0,0,0,28,0,0,0,73,0,110,0,118,0,97,0,108,0,105,0,100,0,32,0,108,0,101,0,110,0,103,0,116,0,104,0,65,176,1,11,53,38,0,0,0,1,0,0,0,1,0,0,0,38,0,0,0,126,0,108,0,105,0,98,0,47,0,97,0,114,0,114,0,97,0,121,0,98,0,117,0,102,0,102,0,101,0,114,0,46,0,116,0,115,0,65,240,1,11,51,36,0,0,0,1,0,0,0,1,0,0,0,36,0,0,0,73,0,110,0,100,0,101,0,120,0,32,0,111,0,117,0,116,0,32,0,111,0,102,0,32,0,114,0,97,0,110,0,103,0,101,0,65,176,2,11,51,36,0,0,0,1,0,0,0,1,0,0,0,36,0,0,0,126,0,108,0,105,0,98,0,47,0,116,0,121,0,112,0,101,0,100,0,97,0,114,0,114,0,97,0,121,0,46,0,116,0,115,0,65,240,2,11,53,7,0,0,0,16,0,0,0,0,0,0,0,16,0,0,0,0,0,0,0,16,0,0,0,0,0,0,0,145,4,0,0,2,0,0,0,49,0,0,0,2,0,0,0,17,1,0,0,2,0,0,0,16,0,34,16,115,111,117,114,99,101,77,97,112,112,105,110,103,85,82,76,16,46,47,114,97,98,105,110,46,119,97,115,109,46,109,97,112])
  // make it work async because browsers throw when a wasm module is bigger than 4kb and load sync
  return instantiate(new Response(new Blob([wasm], {type: 'application/wasm'})), imp)
}
module.exports = loadWebAssembly

},{"@assemblyscript/loader":2}],2:[function(require,module,exports){
"use strict";

// Runtime header offsets
const ID_OFFSET = -8;
const SIZE_OFFSET = -4;

// Runtime ids
const ARRAYBUFFER_ID = 0;
const STRING_ID = 1;
const ARRAYBUFFERVIEW_ID = 2;

// Runtime type information
const ARRAYBUFFERVIEW = 1 << 0;
const ARRAY = 1 << 1;
const SET = 1 << 2;
const MAP = 1 << 3;
const VAL_ALIGN_OFFSET = 5;
const VAL_ALIGN = 1 << VAL_ALIGN_OFFSET;
const VAL_SIGNED = 1 << 10;
const VAL_FLOAT = 1 << 11;
const VAL_NULLABLE = 1 << 12;
const VAL_MANAGED = 1 << 13;
const KEY_ALIGN_OFFSET = 14;
const KEY_ALIGN = 1 << KEY_ALIGN_OFFSET;
const KEY_SIGNED = 1 << 19;
const KEY_FLOAT = 1 << 20;
const KEY_NULLABLE = 1 << 21;
const KEY_MANAGED = 1 << 22;

// Array(BufferView) layout
const ARRAYBUFFERVIEW_BUFFER_OFFSET = 0;
const ARRAYBUFFERVIEW_DATASTART_OFFSET = 4;
const ARRAYBUFFERVIEW_DATALENGTH_OFFSET = 8;
const ARRAYBUFFERVIEW_SIZE = 12;
const ARRAY_LENGTH_OFFSET = 12;
const ARRAY_SIZE = 16;

const BIGINT = typeof BigUint64Array !== "undefined";
const THIS = Symbol();
const CHUNKSIZE = 1024;

/** Gets a string from an U32 and an U16 view on a memory. */
function getStringImpl(buffer, ptr) {
  const U32 = new Uint32Array(buffer);
  const U16 = new Uint16Array(buffer);
  var length = U32[(ptr + SIZE_OFFSET) >>> 2] >>> 1;
  var offset = ptr >>> 1;
  if (length <= CHUNKSIZE) return String.fromCharCode.apply(String, U16.subarray(offset, offset + length));
  const parts = [];
  do {
    const last = U16[offset + CHUNKSIZE - 1];
    const size = last >= 0xD800 && last < 0xDC00 ? CHUNKSIZE - 1 : CHUNKSIZE;
    parts.push(String.fromCharCode.apply(String, U16.subarray(offset, offset += size)));
    length -= size;
  } while (length > CHUNKSIZE);
  return parts.join("") + String.fromCharCode.apply(String, U16.subarray(offset, offset + length));
}

/** Prepares the base module prior to instantiation. */
function preInstantiate(imports) {
  const baseModule = {};

  function getString(memory, ptr) {
    if (!memory) return "<yet unknown>";
    return getStringImpl(memory.buffer, ptr);
  }

  // add common imports used by stdlib for convenience
  const env = (imports.env = imports.env || {});
  env.abort = env.abort || function abort(mesg, file, line, colm) {
    const memory = baseModule.memory || env.memory; // prefer exported, otherwise try imported
    throw Error("abort: " + getString(memory, mesg) + " at " + getString(memory, file) + ":" + line + ":" + colm);
  }
  env.trace = env.trace || function trace(mesg, n) {
    const memory = baseModule.memory || env.memory;
    console.log("trace: " + getString(memory, mesg) + (n ? " " : "") + Array.prototype.slice.call(arguments, 2, 2 + n).join(", "));
  }
  imports.Math = imports.Math || Math;
  imports.Date = imports.Date || Date;

  return baseModule;
}

/** Prepares the final module once instantiation is complete. */
function postInstantiate(baseModule, instance) {
  const rawExports = instance.exports;
  const memory = rawExports.memory;
  const table = rawExports.table;
  const alloc = rawExports["__alloc"];
  const retain = rawExports["__retain"];
  const rttiBase = rawExports["__rtti_base"] || ~0; // oob if not present

  /** Gets the runtime type info for the given id. */
  function getInfo(id) {
    const U32 = new Uint32Array(memory.buffer);
    const count = U32[rttiBase >>> 2];
    if ((id >>>= 0) >= count) throw Error("invalid id: " + id);
    return U32[(rttiBase + 4 >>> 2) + id * 2];
  }

  /** Gets the runtime base id for the given id. */
  function getBase(id) {
    const U32 = new Uint32Array(memory.buffer);
    const count = U32[rttiBase >>> 2];
    if ((id >>>= 0) >= count) throw Error("invalid id: " + id);
    return U32[(rttiBase + 4 >>> 2) + id * 2 + 1];
  }

  /** Gets the runtime alignment of a collection's values. */
  function getValueAlign(info) {
    return 31 - Math.clz32((info >>> VAL_ALIGN_OFFSET) & 31); // -1 if none
  }

  /** Gets the runtime alignment of a collection's keys. */
  function getKeyAlign(info) {
    return 31 - Math.clz32((info >>> KEY_ALIGN_OFFSET) & 31); // -1 if none
  }

  /** Allocates a new string in the module's memory and returns its retained pointer. */
  function __allocString(str) {
    const length = str.length;
    const ptr = alloc(length << 1, STRING_ID);
    const U16 = new Uint16Array(memory.buffer);
    for (var i = 0, p = ptr >>> 1; i < length; ++i) U16[p + i] = str.charCodeAt(i);
    return ptr;
  }

  baseModule.__allocString = __allocString;

  /** Reads a string from the module's memory by its pointer. */
  function __getString(ptr) {
    const buffer = memory.buffer;
    const id = new Uint32Array(buffer)[ptr + ID_OFFSET >>> 2];
    if (id !== STRING_ID) throw Error("not a string: " + ptr);
    return getStringImpl(buffer, ptr);
  }

  baseModule.__getString = __getString;

  /** Gets the view matching the specified alignment, signedness and floatness. */
  function getView(alignLog2, signed, float) {
    const buffer = memory.buffer;
    if (float) {
      switch (alignLog2) {
        case 2: return new Float32Array(buffer);
        case 3: return new Float64Array(buffer);
      }
    } else {
      switch (alignLog2) {
        case 0: return new (signed ? Int8Array : Uint8Array)(buffer);
        case 1: return new (signed ? Int16Array : Uint16Array)(buffer);
        case 2: return new (signed ? Int32Array : Uint32Array)(buffer);
        case 3: return new (signed ? BigInt64Array : BigUint64Array)(buffer);
      }
    }
    throw Error("unsupported align: " + alignLog2);
  }

  /** Allocates a new array in the module's memory and returns its retained pointer. */
  function __allocArray(id, values) {
    const info = getInfo(id);
    if (!(info & (ARRAYBUFFERVIEW | ARRAY))) throw Error("not an array: " + id + " @ " + info);
    const align = getValueAlign(info);
    const length = values.length;
    const buf = alloc(length << align, ARRAYBUFFER_ID);
    const arr = alloc(info & ARRAY ? ARRAY_SIZE : ARRAYBUFFERVIEW_SIZE, id);
    const U32 = new Uint32Array(memory.buffer);
    U32[arr + ARRAYBUFFERVIEW_BUFFER_OFFSET >>> 2] = retain(buf);
    U32[arr + ARRAYBUFFERVIEW_DATASTART_OFFSET >>> 2] = buf;
    U32[arr + ARRAYBUFFERVIEW_DATALENGTH_OFFSET >>> 2] = length << align;
    if (info & ARRAY) U32[arr + ARRAY_LENGTH_OFFSET >>> 2] = length;
    const view = getView(align, info & VAL_SIGNED, info & VAL_FLOAT);
    if (info & VAL_MANAGED) {
      for (let i = 0; i < length; ++i) view[(buf >>> align) + i] = retain(values[i]);
    } else {
      view.set(values, buf >>> align);
    }
    return arr;
  }

  baseModule.__allocArray = __allocArray;

  /** Gets a live view on an array's values in the module's memory. Infers the array type from RTTI. */
  function __getArrayView(arr) {
    const U32 = new Uint32Array(memory.buffer);
    const id = U32[arr + ID_OFFSET >>> 2];
    const info = getInfo(id);
    if (!(info & ARRAYBUFFERVIEW)) throw Error("not an array: " + id);
    const align = getValueAlign(info);
    var buf = U32[arr + ARRAYBUFFERVIEW_DATASTART_OFFSET >>> 2];
    const length = info & ARRAY
      ? U32[arr + ARRAY_LENGTH_OFFSET >>> 2]
      : U32[buf + SIZE_OFFSET >>> 2] >>> align;
    return getView(align, info & VAL_SIGNED, info & VAL_FLOAT)
          .subarray(buf >>>= align, buf + length);
  }

  baseModule.__getArrayView = __getArrayView;

  /** Copies an array's values from the module's memory. Infers the array type from RTTI. */
  function __getArray(arr) {
    const input = __getArrayView(arr);
    const len = input.length;
    const out = new Array(len);
    for (let i = 0; i < len; i++) out[i] = input[i];
    return out;
  }

  baseModule.__getArray = __getArray;

  /** Copies an ArrayBuffer's value from the module's memory. */
  function __getArrayBuffer(ptr) {
    const buffer = memory.buffer;
    const length = new Uint32Array(buffer)[ptr + SIZE_OFFSET >>> 2];
    return buffer.slice(ptr, ptr + length);
  }

  baseModule.__getArrayBuffer = __getArrayBuffer;

  /** Copies a typed array's values from the module's memory. */
  function getTypedArray(Type, alignLog2, ptr) {
    return new Type(getTypedArrayView(Type, alignLog2, ptr));
  }

  /** Gets a live view on a typed array's values in the module's memory. */
  function getTypedArrayView(Type, alignLog2, ptr) {
    const buffer = memory.buffer;
    const U32 = new Uint32Array(buffer);
    const bufPtr = U32[ptr + ARRAYBUFFERVIEW_DATASTART_OFFSET >>> 2];
    return new Type(buffer, bufPtr, U32[bufPtr + SIZE_OFFSET >>> 2] >>> alignLog2);
  }

  baseModule.__getInt8Array = getTypedArray.bind(null, Int8Array, 0);
  baseModule.__getInt8ArrayView = getTypedArrayView.bind(null, Int8Array, 0);
  baseModule.__getUint8Array = getTypedArray.bind(null, Uint8Array, 0);
  baseModule.__getUint8ArrayView = getTypedArrayView.bind(null, Uint8Array, 0);
  baseModule.__getUint8ClampedArray = getTypedArray.bind(null, Uint8ClampedArray, 0);
  baseModule.__getUint8ClampedArrayView = getTypedArrayView.bind(null, Uint8ClampedArray, 0);
  baseModule.__getInt16Array = getTypedArray.bind(null, Int16Array, 1);
  baseModule.__getInt16ArrayView = getTypedArrayView.bind(null, Int16Array, 1);
  baseModule.__getUint16Array = getTypedArray.bind(null, Uint16Array, 1);
  baseModule.__getUint16ArrayView = getTypedArrayView.bind(null, Uint16Array, 1);
  baseModule.__getInt32Array = getTypedArray.bind(null, Int32Array, 2);
  baseModule.__getInt32ArrayView = getTypedArrayView.bind(null, Int32Array, 2);
  baseModule.__getUint32Array = getTypedArray.bind(null, Uint32Array, 2);
  baseModule.__getUint32ArrayView = getTypedArrayView.bind(null, Uint32Array, 2);
  if (BIGINT) {
    baseModule.__getInt64Array = getTypedArray.bind(null, BigInt64Array, 3);
    baseModule.__getInt64ArrayView = getTypedArrayView.bind(null, BigInt64Array, 3);
    baseModule.__getUint64Array = getTypedArray.bind(null, BigUint64Array, 3);
    baseModule.__getUint64ArrayView = getTypedArrayView.bind(null, BigUint64Array, 3);
  }
  baseModule.__getFloat32Array = getTypedArray.bind(null, Float32Array, 2);
  baseModule.__getFloat32ArrayView = getTypedArrayView.bind(null, Float32Array, 2);
  baseModule.__getFloat64Array = getTypedArray.bind(null, Float64Array, 3);
  baseModule.__getFloat64ArrayView = getTypedArrayView.bind(null, Float64Array, 3);

  /** Tests whether an object is an instance of the class represented by the specified base id. */
  function __instanceof(ptr, baseId) {
    const U32 = new Uint32Array(memory.buffer);
    var id = U32[(ptr + ID_OFFSET) >>> 2];
    if (id <= U32[rttiBase >>> 2]) {
      do if (id == baseId) return true;
      while (id = getBase(id));
    }
    return false;
  }

  baseModule.__instanceof = __instanceof;

  // Pull basic exports to baseModule so code in preInstantiate can use them
  baseModule.memory = baseModule.memory || memory;
  baseModule.table  = baseModule.table  || table;

  // Demangle exports and provide the usual utility on the prototype
  return demangle(rawExports, baseModule);
}

function isResponse(o) {
  return typeof Response !== "undefined" && o instanceof Response;
}

/** Asynchronously instantiates an AssemblyScript module from anything that can be instantiated. */
async function instantiate(source, imports) {
  if (isResponse(source = await source)) return instantiateStreaming(source, imports);
  return postInstantiate(
    preInstantiate(imports || (imports = {})),
    await WebAssembly.instantiate(
      source instanceof WebAssembly.Module
        ? source
        : await WebAssembly.compile(source),
      imports
    )
  );
}

exports.instantiate = instantiate;

/** Synchronously instantiates an AssemblyScript module from a WebAssembly.Module or binary buffer. */
function instantiateSync(source, imports) {
  return postInstantiate(
    preInstantiate(imports || (imports = {})),
    new WebAssembly.Instance(
      source instanceof WebAssembly.Module
        ? source
        : new WebAssembly.Module(source),
      imports
    )
  )
}

exports.instantiateSync = instantiateSync;

/** Asynchronously instantiates an AssemblyScript module from a response, i.e. as obtained by `fetch`. */
async function instantiateStreaming(source, imports) {
  if (!WebAssembly.instantiateStreaming) {
    return instantiate(
      isResponse(source = await source)
        ? source.arrayBuffer()
        : source,
      imports
    );
  }
  return postInstantiate(
    preInstantiate(imports || (imports = {})),
    (await WebAssembly.instantiateStreaming(source, imports)).instance
  );
}

exports.instantiateStreaming = instantiateStreaming;

/** Demangles an AssemblyScript module's exports to a friendly object structure. */
function demangle(exports, baseModule) {
  var module = baseModule ? Object.create(baseModule) : {};
  var setArgumentsLength = exports["__argumentsLength"]
    ? function(length) { exports["__argumentsLength"].value = length; }
    : exports["__setArgumentsLength"] || exports["__setargc"] || function() {};
  for (let internalName in exports) {
    if (!Object.prototype.hasOwnProperty.call(exports, internalName)) continue;
    const elem = exports[internalName];
    let parts = internalName.split(".");
    let curr = module;
    while (parts.length > 1) {
      let part = parts.shift();
      if (!Object.prototype.hasOwnProperty.call(curr, part)) curr[part] = {};
      curr = curr[part];
    }
    let name = parts[0];
    let hash = name.indexOf("#");
    if (hash >= 0) {
      let className = name.substring(0, hash);
      let classElem = curr[className];
      if (typeof classElem === "undefined" || !classElem.prototype) {
        let ctor = function(...args) {
          return ctor.wrap(ctor.prototype.constructor(0, ...args));
        };
        ctor.prototype = {
          valueOf: function valueOf() {
            return this[THIS];
          }
        };
        ctor.wrap = function(thisValue) {
          return Object.create(ctor.prototype, { [THIS]: { value: thisValue, writable: false } });
        };
        if (classElem) Object.getOwnPropertyNames(classElem).forEach(name =>
          Object.defineProperty(ctor, name, Object.getOwnPropertyDescriptor(classElem, name))
        );
        curr[className] = ctor;
      }
      name = name.substring(hash + 1);
      curr = curr[className].prototype;
      if (/^(get|set):/.test(name)) {
        if (!Object.prototype.hasOwnProperty.call(curr, name = name.substring(4))) {
          let getter = exports[internalName.replace("set:", "get:")];
          let setter = exports[internalName.replace("get:", "set:")];
          Object.defineProperty(curr, name, {
            get: function() { return getter(this[THIS]); },
            set: function(value) { setter(this[THIS], value); },
            enumerable: true
          });
        }
      } else {
        if (name === 'constructor') {
          (curr[name] = (...args) => {
            setArgumentsLength(args.length);
            return elem(...args);
          }).original = elem;
        } else { // instance method
          (curr[name] = function(...args) { // !
            setArgumentsLength(args.length);
            return elem(this[THIS], ...args);
          }).original = elem;
        }
      }
    } else {
      if (/^(get|set):/.test(name)) {
        if (!Object.prototype.hasOwnProperty.call(curr, name = name.substring(4))) {
          Object.defineProperty(curr, name, {
            get: exports[internalName.replace("set:", "get:")],
            set: exports[internalName.replace("get:", "set:")],
            enumerable: true
          });
        }
      } else if (typeof elem === "function" && elem !== setArgumentsLength) {
        (curr[name] = (...args) => {
          setArgumentsLength(args.length);
          return elem(...args);
        }).original = elem;
      } else {
        curr[name] = elem;
      }
    }
  }
  return module;
}

exports.demangle = demangle;

},{}],3:[function(require,module,exports){
const Rabin = require('./rabin')
const getRabin = require('../dist/rabin-wasm.node.js')

const create = async (avg, min, max, windowSize, polynomial) => {
    const compiled = await getRabin()
    return new Rabin(compiled, avg, min, max, windowSize, polynomial)
}

module.exports = {
    Rabin,
    create
}

},{"../dist/rabin-wasm.node.js":1,"./rabin":4}],4:[function(require,module,exports){
/**
 * Rabin fingerprinting
 *
 * @class Rabin
 */
class Rabin {
    /**
     * Creates an instance of Rabin.
     * @param { import("./../dist/rabin-wasm") } asModule
     * @param {number} [bits=12]
     * @param {number} [min=8 * 1024]
     * @param {number} [max=32 * 1024]
     * @param {number} polynomial
     * @memberof Rabin
     */
    constructor(asModule, bits = 12, min = 8 * 1024, max = 32 * 1024, windowSize = 64, polynomial) {
        this.bits = bits
        this.min = min
        this.max = max
        this.asModule = asModule
        this.rabin = new asModule.Rabin(bits, min, max, windowSize, polynomial)
        this.polynomial = polynomial
    }

    /**
     * Fingerprints the buffer
     *
     * @param {Uint8Array} buf
     * @returns {Array<number>}
     * @memberof Rabin
     */
    fingerprint(buf) {
        const {
            __retain,
            __release,
            __allocArray,
            __getInt32Array,
            Int32Array_ID,
            Uint8Array_ID
        } = this.asModule

        const lengths = new Int32Array(Math.ceil(buf.length/this.min))
        const lengthsPtr = __retain(__allocArray(Int32Array_ID, lengths))
        const pointer = __retain(__allocArray(Uint8Array_ID, buf))

        const out = this.rabin.fingerprint(pointer, lengthsPtr)
        const processed = __getInt32Array(out)

        __release(pointer)
        __release(lengthsPtr)

        const end = processed.indexOf(0);
        return end >= 0 ? processed.subarray(0, end) : processed;
    }
}

module.exports = Rabin
},{}]},{},[3])(3)
});
