# Fil-TCC

TinyCC 0.9.28rc plus an optional `-filc` mode. The mode is off unless the compiler is passed `-filc`. This is not the Fil-C clang toolchain, and it does not rebuild system libc.

Configure once, then:

```sh
./configure
make
make test
```

`make test` is the TinyCC suite and the proof for a change outside `-filc`. The `-filc` programs live in `tests/filc/`. A change to that mode is proved by compiling those programs with `./tcc -filc`. `-filc` is not full Fil-C.

`lib/filc_rt.c` is compiled without `-filc`. Leave `libtcc1.a` uninstrumented so ordinary programs and `make test` still link.

The tree is LGPL-2.1-or-later, like TinyCC. Do not relicense it.

clangd can answer hover and definition when `compile_commands.json` sits at the root. Generate it with `bear -- make` after `./configure`. A sanitizer report is still read with the `sanitizers` skill. `make` already has a `sani-test` path that builds `tcc.c` with AddressSanitizer and UndefinedBehaviorSanitizer.
