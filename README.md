# Fil-TCC 0.1.0

Fil-TCC is [TinyCC](https://github.com/TinyCC/tinycc) plus an optional
`-filc` memory-safety mode. The mode is off unless you pass `-filc`.
A pointer stays a normal 64-bit address. Each object also has a
capability: a lower bound and, lazily, one capability word per 8
payload bytes. An out-of-bounds access or a use after free panics.
An integer store does not create or clear a capability.

This is not the [Fil-C](https://github.com/pizlonator/fil-c) compiler.
Fil-C is a clang/LLVM toolchain that recompiles libc. Fil-TCC does
not. System libc, libdl, libpthread, and the C startup files stay
the ones on the host. `lib/filc_rt.c` is compiled without `-filc`
and calls glibc. `malloc` is still glibc's. This release does not
make a memory-safe kernel or desktop.

## Version

| | |
| --- | --- |
| Fil-TCC | 0.1.0, git tag `v0.1.0` |
| TinyCC base | 0.9.28rc |
| Base commit | [`e23dabed154a8495f3ac1ff3cc9d04fa1c207893`](https://github.com/TinyCC/tinycc/commit/e23dabed154a8495f3ac1ff3cc9d04fa1c207893) (mob, 2026-10-16) |

`tcc -dumpversion` prints `0.9.28rc`. `tcc -v` also prints
`fil-tcc 0.1.0`. The base version is `VERSION` and, after
`./configure`, `TCC_VERSION` in `config.h`. `FILTCC_VERSION` is in
`tcc.h`.

## License

Fil-TCC is a modified TinyCC. It is distributed under the GNU Lesser
General Public License, version 2 or any later version, as the source
headers state. The license text in [`COPYING`](COPYING) is version
2.1. The same terms cover `lib/filc_rt.c` and the `-filc` tests.

TinyCC is Copyright (C) 2001-2006 Fabrice Bellard and the TinyCC
contributors. Upstream is <https://github.com/TinyCC/tinycc> and
<https://repo.or.cz/tinycc.git>.

[`RELICENSING`](RELICENSING) is upstream's unfinished request to move
TinyCC to an MIT-style license. The list is not complete: at least
one author declined for `arm-gen.c`, and some entries are unmarked.
This fork does not relicense that code. The license stays LGPL.

### Fil-C

The `-filc` design follows the published InvisiCaps model:

- Author: Filip Pizlo
- Project: <https://github.com/pizlonator/fil-c>
- Description: <https://fil-c.org/invisicaps>

No Fil-C source is in this repository. The Fil-C compiler is LLVM and
is under the LLVM license. The Fil-C runtime is libpas, copyright
Apple Inc., Epic Games, Inc., and Filip Pizlo, under the BSD-style
license in `libpas/LICENSE.txt` in that repository. Those licenses
apply to that code. They do not cover Fil-TCC, because this tree does
not copy it. The implementation in `lib/filc_rt.c`, the compiler
changes, and `tests/filc/` was written here.

## Other attribution

Warning and error colors follow Nicolas Sauzede's TinyCC mob branch
`mob_nsauzede/colored_warning_error`
([`70f8148b192103e5ec023fc19788c9904fd20295`](https://repo.or.cz/tinycc.git/commit/70f8148b192103e5ec023fc19788c9904fd20295)).
Help-text coloring was added in this fork. Color is off when
`NO_COLOR` is set, when `TERM` is `dumb`, or when the stream is not a
terminal (<https://no-color.org/>).

The original TinyCC readme is [`README`](README). Build notes for
Windows are `win32/tcc-win32.txt`.

## Build

```sh
./configure
make
make -C lib ../filc_rt.o
```

`make` builds the compiler with the host compiler. It does not rebuild
`filc_rt.o`. From this directory, pass `-B.` so the compiler finds its
libraries:

```sh
./tcc -B. hello.c
./tcc -B. -filc hello.c
```

`libtcc1.a` stays uninstrumented, so a program built without `-filc`
is ordinary TinyCC. Instrumented compiler helpers are
`libtcc1-filc.a`, and they are linked only for `-filc`.

`-run` cannot execute a `-filc` program. The runtime uses
thread-local storage, which `tcc -run` does not provide.

## What `-filc` checks

Direct and indirect calls to a large subset of libc are checked:
strings, memory, stdio, wide and multibyte conversion, `malloc`,
`dlopen` and the related functions, and the numeric conversions that
the tests cover. The tests are `tests/filc/`. They are not part of
`make test`.

Still ordinary unchecked libc: threads, sockets, directories, `fork`,
and any library you did not compile with `-filc`. The locale forms
`strcasecmp_l`, `strncasecmp_l`, `strcoll_l`, and `strxfrm_l` are not
intercepted. An unchecked callee does not publish a capability for a
pointer it returns.
