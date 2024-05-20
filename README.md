# CCC

CCC is intended to be a modern replacement for the C standard library.

## Goals

- **Performance, relying on compiler optimisation** - the library is designed to
  be as performant as possible, but only when compiler optimisations are taken
  into consideration. An example of this is the formatting module, which uses
  dynamically dispatched polymorphism for its methods. Modern compilers (GCC,
  Clang) do a good job of inlining these methods and removing the abstraction,
  however compilers that cannot perform these optimisations will produce
  *'less-than-optimal'* code.
- **Ergonomic API** - CCC is intended to bring an experience closer to the likes
  of the Rust standard library, and tries to **push the limitations of modern C
  to its limits**.
- **Safety** - the library is built on safer primitives like *fixed-size
  integers and bounded buffers and strings*, the preference of *passing by
  value* over passing by pointer, *strongly typed pointers*, *avoiding null
  pointers*, etc.

## Requirements

- C11 (C99 support currently being considered).
- Support for `#pragma once`.
- Support for `uint64_t` and `int64_t` (note that these can be, and usually are,
  defined even on 32-bit or smaller architectures).
- Full range of 8-64 bit integers (8, 16, 32, 64) available in regular
  variable-size integer types (`char`, `short`, `int`, `long`, `long long`).
- `size_t` no wider than 64 bits (128-bit `size_t` support added with the
  `CCC_ENABLE_SIZE128` feature flag.

## Feature Flags

These features can be enabled by defining the appropriate flag (e.g. via `-D`).

| Flag                      | Description |
|---------------------------|-------------|
| `CCC_USE_LIBC_INTRINSICS` | Intrinsics use libc functions like `memcpy` and `strlen`. This communicates the intent more clearly to the compiler, usually allowing for better optimisations. This will cause additional libc headers to be included. |
| `CCC_ENABLE_GNU_FEATURES` | Some helpers like `try` need GNU extensions like `__auto_type`. |
| `CCC_RESTRICT_REFERENCES` | References have the `restrict` keyword on the underlying pointers. |
| `CCC_ENABLE_SIZE128`      | Enables the support for architectures that have a 128-bit `size_t`. |
