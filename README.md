# CCC

CCC is intended to be a modern replacement for the C standard library.

## Goals

- **Compilation with a standard compiler** - Code written with *ccc* should
  compile with a standard-compliant compiler, without using any custom
  preprocessors or other code transformers. Currently, the minimum standard
  requirement is *C11*.
- **Performance, relying on compiler optimisation** - The library is designed to
  be as performant as possible, but only when compiler optimisations are taken
  into consideration. An example of this is the formatting module, which uses
  dynamically dispatched polymorphism for its methods. Modern compilers (GCC,
  Clang) do a good job of inlining these methods and removing the abstraction
  altogether, however compilers that cannot perform these optimisations will
  produce *'less-than-optimal'* code.
- **Ergonomic API** - CCC is intended to bring an experience closer to the likes
  of the Rust standard library and tries to **push modern C to its limits**
  (while still being sane and safe, of course).
- **Safety** - The library is built on safer primitives like *fixed-size
  integers and bounded buffers and strings*, the preference of *passing by
  value* over passing by pointer, *strongly typed pointers*, *avoiding null
  pointers*, etc.

  It is planned to further expand this by implementing a custom [borrow
  checker](https://rustc-dev-guide.rust-lang.org/borrow_check.html).

## Requirements

- C11
	- *C99* or lower could be supported with compiler extensions and is
	  currently being considered.
- Support for `#pragma once` (soon to be removed).
- Support for `uint64_t` and `int64_t` (note that these can be, and usually are,
  defined even on 32-bit or smaller architectures).
- Full range of 8-64 bit integers (8, 16, 32, 64) available in regular
  variable-size integer types (`char`, `short`, `int`, `long`, `long long`).
- `size_t` no wider than 64 bits (128-bit `size_t` support added with the
  `CCC_ENABLE_SIZE128` feature flag.

## Feature Flags

These features can be enabled by defining the appropriate flag (e.g. via `-D`).

### Default Features

By default, CCC enables these features:

- `CCC_MODULE_ATOMICS`
- `CCC_MODULE_IO`

This can be overridden with the `CCC_NO_DEFAULT` flag.

### Groups

These are blanket flags that enable multiple features. **Do not use if you don't
want all the features.** Instead, enable selectively.

| Group                | Features                  | Description |
|----------------------|---------------------------|-------------|
| `CCC_DEBUG`          | `CCC_DEBUG_ASSERTIONS`    | Enables runtime checks and other things useful for debugging. |
| `CCC_BORROW_CHECKER` | `CCC_RESTRICT_REFERENCES` | Enables features that should only be used with the borrow checker. |

### Features

| Flag                      | Description |
|---------------------------|-------------|
| `CCC_MODULE_ATOMICS`      | Enables the *atomic* module. This is currently implemented on top of `stdatomic.h` and imports this header. |
| `CCC_MODULE_IO`           | Enables the *io* module, which is an interface to the standard I/O. This **requires the *atomic* module**. If disabled, you will have to implement a custom *panic handler*. |
| `CCC_DEBUG_ASSERTIONS`    | *Activates* various `DEBUG_` macros (e.g. `DEBUG_ASSERT`). |
| `CCC_USE_LIBC_INTRINSICS` | Intrinsics use libc functions like `memcpy` and `strlen`. This communicates the intent more clearly to the compiler, usually allowing for better optimisations. It will cause additional libc headers to be included and will require linking with *libc*. |
| `CCC_ENABLE_GNU_FEATURES` | Some helpers like `try` need GNU extensions like `__auto_type`. |
| `CCC_RESTRICT_REFERENCES` | References have the `restrict` keyword on the underlying pointers. **Should not be used without the borrow checker. Aliasing WILL result in undefined behaviour.** |
| `CCC_ENABLE_SIZE128`      | Enables the support for architectures that have a 128-bit `size_t`. |

## Reserved Keywords

CCC reserves anything that starts with `ccc_`, `_ccc_`, `__ccc_`, `CCC_`,
`_CCC_` or `__CCC_`, in addition to the keywords found in
[RESERVED_KEYWORDS.txt](RESERVED_KEYWORDS.txt).

Please note that at this stage, this is subject to massive changes.

## Imported libC Headers

- `stdatomic.h`
- `unistd.h`
- `stdint.h`
- `limits.h`
- `stddef.h`

Some of these are here to stay, as for the rest, work is being done to eliminate
them.
