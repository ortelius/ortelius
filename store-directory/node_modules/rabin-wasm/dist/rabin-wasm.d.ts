declare module ASModule {
  type i8 = number;
  type i16 = number;
  type i32 = number;
  type i64 = BigInt;
  type isize = number;
  type u8 = number;
  type u16 = number;
  type u32 = number;
  type u64 = BigInt;
  type usize = number;
  type f32 = number;
  type f64 = number;
  type bool = any;
  export function __alloc(size: usize, id: u32): usize;
  export function __retain(ptr: usize): usize;
  export function __release(ptr: usize): void;
  export function __collect(): void;
  export var __rtti_base: usize;
  export var Int32Array_ID: u32;
  export var Uint8Array_ID: u32;
  export function degree(polynom: u64): i32;
  export function mod(x: u64, p: u64): u64;
  export class Rabin {
    window: usize;
    window_size: i32;
    wpos: i32;
    count: u64;
    pos: u64;
    start: u64;
    digest: u64;
    chunk_start: u64;
    chunk_length: u64;
    chunk_cut_fingerprint: u64;
    polynomial: u64;
    minsize: u64;
    maxsize: u64;
    mask: u64;
    constructor(average_bits: u32, minsize: u32, maxsize: u32, window_size: i32);
    fingerprint(buf: usize, lengths: usize): usize;
  }
}
export default ASModule;
