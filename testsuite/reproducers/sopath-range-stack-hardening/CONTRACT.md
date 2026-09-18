# Tekt contract: `SoPath` range/stack hardening

## Authority and scope

This is an implementation-blind observable contract derived from
`L0_PROMPT.md`. The immutable baseline is
`40c6372d2984a94c6ac17f059caee7b8d6374654`. The contract author may read the
prompt, public headers and build/test conventions at that baseline and may
write only this reproducer directory on the isolated contract branch. A
candidate implementation and its worktree are prohibited inputs.

The protocol regime is the full segregated-materialization protocol, limited
to the pre-implementation contract/oracle phase. This artifact does not
prescribe a validation mechanism, diagnostic text, storage representation, or
candidate patch.

## Observable contract

Let `full_length` be the number of nodes stored in the complete path. For
`SoFullPath`/`SoTempPath`, this is the length exposed by `SoFullPath`; the same
complete state is used to observe calls made through the `SoPath` API.

For `SoPath::truncate(requested)`:

| Input | Required result |
| --- | --- |
| `requested < 0` | reject without mutation |
| `requested > full_length` | reject without mutation |
| `requested == 0` | succeed and leave an empty path |
| `requested == full_length` | succeed as an exact no-op |
| `0 < requested < full_length` | retain exactly the requested prefix |

"Reject without mutation" means that the call returns normally, without an
escaping exception, abort, crash, or non-termination, and the full sequence of
node identities and child indices remains identical to the pre-call snapshot.
Diagnostics are deliberately not an oracle. Debug may emit one; Release safety
must not depend on one.

The inherited stack operations refine the range rule:

- `SoPath::pop()` and `SoFullPath::pop()` on an empty complete path return
  normally and leave it empty.
- The same requirement applies to already-compiled clients whose baseline
  inline `pop()` computes `truncate(-1)`.
- Each `pop()` on a non-empty path removes exactly the full tail and preserves
  the remaining prefix.

## Independent oracle and expected discrimination

`repro.cpp` exposes one named case per process. It snapshots the full path,
performs one operation, and checks the full node/index sequence. Positive
controls assert exact prefix semantics. `old-header-client.cpp` is compiled
against all public headers archived from the frozen baseline and linked to the
selected library, so a header-only guard in a new client cannot hide an unsafe
out-of-line `truncate` from legacy clients.

The suite rejects these semantic mutants:

- debug-only validation (Release boundary cases fail);
- lower-bound omission (`-1` cases fail);
- upper-bound omission or off-by-one (`full_length + 1` cases fail);
- validation added only to current inline `pop` methods (old-header cases fail);
- protection of `SoPath::pop` but not `SoFullPath::pop`, or vice versa;
- unconditional rejection/no-op (valid reductions and non-empty pops fail);
- mutation before rejection (full snapshots differ).

## Unknown policy and limits

`Unknown` is explicit and never counts as passing. A compile, link, setup, or
timeout failure is `Unknown` for that configuration. A call that aborts,
crashes, throws, hangs, or changes the snapshot is `Violated`. Only an expected
zero exit from a named case is `Preserved`.

The contract does not assert diagnostic wording/count, performance,
thread-safety, ABI compatibility beyond the baseline-header call path, hidden
nodekit presentation, or equivalence outside this truncate/pop fragment.

## Gates

Calibration and later verification must run both current-header and
baseline-header suites against Debug and Release shared-library builds. Cases
must also be repeated in reverse order with the identical classification.
Every positive control must be `Preserved`; every unsafe baseline boundary is
expected to be `Violated`. No implementation verdict or final certificate is
issued by this author.
