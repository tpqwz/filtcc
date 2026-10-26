# Restoring the Fil-C snapshot

This checkout is a colocated Jujutsu and Git repository. There are three
restore points. `filc-restore-3` is the later one. `filc-restore-2`
stays at change `wqqtzoou`. `filc-restore` stays at change `knsozxvl`.
Look a bookmark up rather than trusting a Git hash copied from somewhere
else:

```
jj log -r 'filc-restore-3 | filc-restore-2 | filc-restore'
```

`mob` stays on the upstream commit. In Git each bookmark is a branch of
the same name.

## Latest restore point

`filc-restore-3` is change `wrsxwzzo`, the child of `filc-restore-2`.
Do not move it. Do not edit that change in place.

It adds, on top of the second snapshot:

- `getcwd` checks a caller buffer for a writable `size` bytes and keeps
  that object's capability. A null buffer is the glibc extension: the
  result is a filc heap object, and the libc buffer is freed.
- `sscanf`, `fscanf`, `scanf`, `vsscanf`, `vfscanf`, and `vscanf` check
  each storing conversion. `va_start` binds the register save area to
  the variadic frame, so a wrapper can pass the `va_list` on.
  Unbounded `%s`, scansets, `%p`, `%m`, wide, and positional conversions
  panic with "unchecked sscanf".
- `va_list.c` is part of `libtcc1-filc.a` and is compiled with `-filc`.
  Aligning the overflow cursor keeps the capability. Plain `libtcc1.a`
  stays uninstrumented. This snapshot did not rebuild that archive.
- `dlopen`, `dlsym`, `dlclose`, and `dlerror` check strings and handles.
  A handle is a zero-size capability. A function from `dlsym` can be
  called and cannot be read. Data is limited to the symbol size.
- `vprintf`, `vfprintf`, `vsprintf`, `vsnprintf`, and indirect
  `printf`, `fprintf`, `sprintf`, `snprintf`, and `dprintf` check `%s`
  and `%n`. A positional `%N$` conversion stops after the format
  string. A dynamic `.*` precision still requires a terminator.

On 2026-10-27 the Fil-TCC suite was 188/188. The three-stage self-host
and `make test` were not re-run for this snapshot. The self-host that
exited 42 is the one recorded on `filc-restore-2`. System libc was not
rebuilt. `-filc` is not full Fil-C.

Start new work from this snapshot with:

```
jj new filc-restore-3
```

## Second restore point

`filc-restore-2` is change `wqqtzoou`, the child of `filc-restore`.
Do not move it. Do not edit that change in place.

It adds, on top of the first snapshot:

- VLA scope leave uncovers objects by the track depth saved at the
  first VLA. `tests/filc/vla_shrink.c` aborts on the out-of-bounds write.
- `strtol`, `strtoll`, `strtoul`, `strtoull`, `strtof`, `strtod`,
  `strtold`, `atoi`, `atol`, `atoll`, and `atof` require a terminator
  inside the source object. The end pointer keeps that object's
  capability.
- `-filc` links `libtcc1-filc.a`. Plain `libtcc1.a` stays
  uninstrumented so ordinary programs and `make test` still link.
  `va_list.o` inside the checked archive stays uninstrumented, because
  `__va_arg` walks a compiler-built `va_list`.
- `tests/filc/gc_pause.c` waits with `sigsuspend` so a wake signal is
  not lost.

Build outputs are still not in the snapshot. `make` rebuilds
`libtcc1-filc.a` with the gcc-built `./tcc -filc`. The self-host
binaries under `/tmp/filtcc-self/` (`now/tcc`, `s4/tcc`, `s5/tcc`) are
outside the tree.

On 2026-10-24 this tree completed another three-stage self-host. The
last binary compiled a program with `-filc` that exited 42, and it
compiled `lib/filc_rt.c` without `-filc`. The Fil-TCC suite was 152/152
and `make test` passed. Those two runs used the gcc-built `./tcc`.
`-filc` is not full Fil-C. System libc was not rebuilt.

## First restore point

The bookmark `filc-restore` is TinyCC mob `e23dabed` plus the first
optional `-filc` mode. The change id is `knsozxvl`. In Git that
bookmark is the branch `filc-restore`.

## What filc-restore contains

Parent: `mob` / `mob@origin` at `e23dabed` (`nqylzvwr`), message
`stdatomic: fix store argument evaluation and signal fence macros`.

Included:

- the `-filc` compiler changes
- `lib/filc_rt.c`
- `tests/filc/`

Not included: build outputs (`tcc`, `*.o`, `config.h`, `config.mak`) and
the self-host binaries under `/tmp/filtcc-self/`. Those are gitignored
or live outside this tree. After a restore, run `./configure` and
`make` again.

On 2026-10-24 this snapshot had completed the self-host:

1. Host `./tcc -filc` built a Fil-TCC.
2. That binary compiled the compiler sources with `-filc`, including
   `tccpp.c` and `tccgen.c`, and linked.
3. That binary compiled the same sources again with `-filc`, and
   compiled `lib/filc_rt.c` without `-filc`.
4. The binary from that second compile ran a program that exited 42.

`make test` and the Fil-TCC suite were not re-run for this snapshot.
`-filc` is not full Fil-C.

## See it

```
jj log -r 'filc-restore-3 | filc-restore-2 | filc-restore'
jj show filc-restore-3
jj bookmark list filc-restore-3
jj bookmark list filc-restore-2
jj bookmark list filc-restore
```

`filc-restore-3` should stay change `wrsxwzzo`. `filc-restore-2` should
stay change `wqqtzoou`. `filc-restore` should stay change `knsozxvl`.

## Return the working copy to it

Do not edit `filc-restore` in place. The working copy is a real commit,
and edits rewrite it. `jj edit filc-restore` checks that commit out, so
the next save would change the restore point.

Start a new change on top of the snapshot. The new working copy matches
its tree and leaves the bookmark where it is:

```
jj new filc-restore
```

If `@` is already a child of `filc-restore` and you only want to drop
uncommitted edits:

```
jj restore
```

If `@` is a later change and you want its files to match the snapshot
without deleting the change:

```
jj restore --from filc-restore
```

If you want to delete the current change and start over:

```
jj abandon @
jj new filc-restore
```

`jj abandon` removes that change. It does not move `filc-restore`.

## Keep the bookmark put

These commands retarget bookmarks. Do not aim them at `filc-restore`,
`filc-restore-2`, or `filc-restore-3` unless you mean to replace that
snapshot:

```
jj bookmark set
jj bookmark move
```

`jj commit` does not move `filc-restore`, `filc-restore-2`, or
`filc-restore-3`. After any restore, check:

```
jj bookmark list filc-restore
jj bookmark list filc-restore-2
jj bookmark list filc-restore-3
```

## Undo a jj command

Every jj command is recorded in the operation log. That log is how you
undo a bad restore, bookmark move, or abandon.

```
jj op log
jj undo
```

`jj undo` walks back one operation at a time. `jj redo` walks forward
again. To jump to a specific operation id from `jj op log`:

```
jj op restore <operation>
```

`jj op restore` restores the repo and local bookmarks to that
operation. It creates a new operation; it does not delete history from
the log. Preview first:

```
jj --at-op=<operation> log
```

`jj undo` refuses an operation made in another workspace unless you
pass `--allow-cross-workspace`.

## Git

`.git` and `.jj` are both present. Use jj for commits and restores.
`git status` and `git diff` still work. Do not `git commit` the working
copy out from under jj.

Git HEAD is detached. That is normal here. jj moves HEAD with the
working copy and does not slide the `mob` branch forward. `mob` stays
at `origin/mob`. The branches `filc-restore`, `filc-restore-2`, and
`filc-restore-3` are the snapshots.

`mob` tracks `mob@origin`. Fetch with `jj git fetch`. Nothing in this
setup has been pushed. Pushing a restore bookmark is a separate decision.
