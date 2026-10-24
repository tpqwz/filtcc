# Restoring the Fil-C snapshot

This checkout is a colocated Jujutsu and Git repository. The bookmark
`filc-restore` is the restore point: TinyCC mob `e23dabed` plus the
optional `-filc` mode. The change id is `knsozxvl`. Look it up rather
than trusting a Git hash copied from somewhere else:

```
jj log -r filc-restore
```

`mob` stays on the upstream commit. `filc-restore` is the bookmark that
holds the Fil-C work. In Git that bookmark is the branch `filc-restore`.

## What the snapshot contains

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
jj log -r filc-restore
jj show filc-restore
jj bookmark list filc-restore
```

The bookmark target should stay change `knsozxvl`.

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

These commands retarget bookmarks. Do not aim them at `filc-restore`
unless you mean to replace the snapshot:

```
jj bookmark set
jj bookmark move
```

`jj commit` does not move `filc-restore`. After any restore, check:

```
jj bookmark list filc-restore
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
at `origin/mob`. The branch `filc-restore` is the snapshot.

`mob` tracks `mob@origin`. Fetch with `jj git fetch`. Nothing in this
setup has been pushed. Pushing `filc-restore` is a separate decision.
