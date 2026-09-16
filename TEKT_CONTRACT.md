# Tekt contract: `SoPath` child append validation

## Frozen inputs and authority

- Baseline: `40c6372d2984a94c6ac17f059caee7b8d6374654`.
- Branch: `fix/sopath-child-append-validation`.
- L0 intent (frozen): in every build, `SoPath::append(int)` and
  `SoPath::push(int)` reject, without mutation, an empty path, a tail without
  a child list, a negative index, and an index greater than or equal to the
  number of children. `SoPath::append(SoNode *)` rejects a tail without
  children and a node which is not a child. `SoPath::append(SoPath *)` rejects
  a tail without children. Diagnostics may continue only under `COIN_DEBUG`.
  Valid operations and hidden-child behaviour are preserved.
- Role: contract author and independent A/B test author only. The allowed
  inputs were this L0 intent, the frozen baseline's public headers, and its
  test/build conventions. Candidate implementation and other worktrees/labs
  are forbidden inputs. Writable outputs are this contract and
  `testsuite/reproducers/sopath-child-append-validation/` only.
- Protocol regime: full segregated-materialization protocol, limited to the
  pre-implementation contract/oracle phase assigned here.

This document and its test are intentionally implementation-blind. They do
not prescribe a validation mechanism, diagnostic text, or internal data
structure.

## Observable contract

For this contract, **reject without mutation** means all of the following:

1. The call returns normally (no crash, abort, exception escaping the API, or
   non-termination).
2. The path remains equal to a copy taken immediately before the call.
3. Its public length, head, tail, visible nodes, and visible child indices are
   unchanged.
4. A rejected call does not prevent normal destruction/cleanup of the path
   and scene graph.

Diagnostics are deliberately not an oracle. A `COIN_DEBUG` build may emit a
diagnostic for a rejection. A non-`COIN_DEBUG` build must not depend on a
diagnostic to reject safely. Neither the presence nor exact wording of a
diagnostic is required.

The overload preconditions and outcomes are:

| Operation | Input class | Required result |
| --- | --- | --- |
| `append(int)`, `push(int)` | empty path | reject without mutation |
| `append(int)`, `push(int)` | tail has no child list | reject without mutation |
| `append(int)`, `push(int)` | index `< 0` | reject without mutation |
| `append(int)`, `push(int)` | index `>= child_count` (including zero children) | reject without mutation |
| `append(SoNode *)` | tail has no child list, or has zero children | reject without mutation |
| `append(SoNode *)` | node is not an immediate child of the full tail | reject without mutation |
| `append(SoPath *)` | tail has no child list, or has zero children | reject without mutation |
| all three overloads and `push` | valid immediate-child relation | preserve existing successful append semantics |

“Tail” for validation means the full stored tail, including a tail hidden by
the public `SoPath` view. This is necessary to preserve existing nodekit
hidden-child paths: a valid append below a hidden node must still succeed, and
an invalid append below it must remain non-mutating. The public hidden view
(visible length and visible tail) must not be exposed or otherwise changed by
the validation.

The `append(SoPath *)` positive case in the oracle uses a suffix whose head is
an immediate child of the receiver's full tail; the remainder of that suffix
is valid. The contract does not broaden the scope to unrelated malformed
suffixes or null arguments.

## Independent oracle

The standalone executable in
`testsuite/reproducers/sopath-child-append-validation/repro.cpp` exposes one
named case per process. `run.sh` compiles it against a selected Coin build and
runs every case independently, so one invalid-input crash cannot hide later
results.

Negative cases cover both `append(int)` and its `push(int)` alias, leaf tails,
zero-child group tails, both index boundaries, non-child nodes, and the path
overload. Positive controls cover every overload and `push(int)`. A nodekit
case checks successful continuation below a hidden child as well as rejection
without mutation below that same hidden tail.

Expected discrimination:

- A candidate which only adds debug assertions is rejected by Release runs.
- A candidate with an off-by-one upper bound is rejected by the
  `index == child_count` cases.
- A candidate which omits negative-index validation is rejected separately.
- A candidate which validates the visible tail instead of the full hidden
  tail is rejected by the nodekit case.
- A candidate which rejects all appends is rejected by the positive controls.
- A candidate which mutates before reporting an error is rejected by the
  before/after snapshots.

## `Unknown` policy and limits

`Unknown` is explicit and never counts as `Preserved` or as a passing test.
It is used when an observation cannot be built or run against the requested
configuration, when identity/equality is ambiguous, or when an opaque
construction cannot expose the required state through public API. A crash is
`Violated`, not `Unknown`. A compile/link/setup failure is `Unknown` for that
configuration and prevents a conclusive verdict.

The suite does not assert diagnostic wording/count, thread-safety, null-node
or null-path handling, malformed suffixes beyond the stated tail condition,
ABI compatibility, or functional equivalence outside this append fragment.
The nodekit case observes hidden-state mutation through path equality and the
unchanged public hidden view; if a platform's public equality cannot observe
the full stored path, that sub-observation is `Unknown` and must be reported.

## Gates and sealing policy

Before sealing, run the suite against at least one Release and one Debug
build. Each named case must pass in both. Re-run in a different case order; the
classification must be identical. The intended mutation score is `1.0`: all
listed semantic mutants must be rejected while every positive control passes.
Any surviving mutant, `Unknown`, or changed frozen input prevents sealing.

This author does not issue the final implementation verdict or certificate.
Those belong to an independent verifier after candidate integration.
