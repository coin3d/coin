# `SoTempPath::replaceTail()` empty-path contract

## Frozen inputs and authority

- Baseline: `40c6372d2984a94c6ac17f059caee7b8d6374654`.
- Coin 4 obligation: calling `replaceTail(node, index)` on an empty
  `SoTempPath` returns normally, performs no mutation and performs no
  out-of-bounds access in Debug and Release builds. On a non-empty path its
  existing replacement behavior is preserved.
- Contract/test author may read the obligation and baseline and write only
  this contract oracle. It must not read or edit an implementation candidate.
- A later implementer must treat this directory and its CMake registration as
  protected input.

This is the contract/oracle phase of the Tekt full protocol. It is segregated
from the future implementation, but no claim of final verification or general
`SoTempPath` correctness is made here.

## Observable contract

`empty-noop` requires all of the following:

1. `replaceTail(NULL, -1)` returns normally for a newly constructed path.
2. The path remains empty (`getLength() == 0`, `getHead() == NULL`).
3. The path remains reusable by a subsequent valid `simpleAppend(NULL, -1)`.
4. Process termination is normal, including object destruction.

`valid-sentinel` is one positive preservation control. `NULL/-1` is an
intentional SoAction sentinel in an already non-empty `SoTempPath`, so replacing
that tail with `NULL/-1` must retain length one, null tail and index `-1`.
`valid-replacement` prevents a vacuous no-op implementation from satisfying the
contract: it replaces one real child tail/index with another valid child and
requires the prefix, length and new tail/index to be exact.
Diagnostics on the empty call may differ between Debug and Release; return,
state, reusability and memory safety may not.

The frozen baseline is expected to preserve both positive controls and violate
`empty-noop`: it computes tail index `-1` and writes before both empty lists. The
reuse step exposes that corruption without depending solely on ASan.

## `Unknown` policy

The oracle makes no claim about:

- non-empty replacement with an invalid parent/child relation;
- dangling non-null node pointers;
- arbitrary negative indices other than the established `NULL/-1` sentinel;
- exact diagnostic text, allocation capacity, object layout or ABI;
- inherited empty-path behavior of `SoFullPath::getTail()`, `pop()` or indexed
  accessors.

These cases are `Unknown`, never implicit successes. An unrecognized runner
case exits `2` rather than being classified as preserved.

## Commands

With tests enabled in a configured build:

```sh
cmake --build build --target SoTempPathReplaceTailContract
ctest --test-dir build -R '^SoTempPathReplaceTail_' --output-on-failure
```

Run against an existing shared-library build:

```sh
sh testsuite/reproducers/sotemppath-empty-replace-tail/run.sh \
  /absolute/path/to/build/lib valid-sentinel
sh testsuite/reproducers/sotemppath-empty-replace-tail/run.sh \
  /absolute/path/to/build/lib valid-replacement
sh testsuite/reproducers/sotemppath-empty-replace-tail/run.sh \
  /absolute/path/to/build/lib empty-noop
```

Repeat both commands for Debug and Release. Sanitized builds may add, for
example, `CXXFLAGS='-fsanitize=address,undefined'` and matching `LDFLAGS`.
