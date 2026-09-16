# Tekt A/B contract: legacy `SoPath` accessor bounds

## Frozen inputs and authority

- Regime: segregated A/B contract and independent tests.
- L0 intent: `getNode()` and `getNodeFromTail()` return `NULL`, while
  `getIndex()` and `getIndexFromTail()` return `-1`, for negative indices,
  the relevant limit, values above that limit, and empty paths, in Debug and
  Release, without mutation. Valid indices and the visible/full distinction
  remain unchanged. Diagnostics may remain in `COIN_DEBUG` only.
- Baseline: commit `40c6372d2984a94c6ac17f059caee7b8d6374654` on branch
  `fix/sopath-legacy-accessor-bounds`.
- Environment: `/tmp/coin-sopath-accessors`; no candidate implementation,
  primary worktree, or implementation lab was an input.
- Authority: the executor may write this contract and files below
  `testsuite/reproducers/sopath-legacy-accessor-bounds/`, plus test-only CMake
  registration. The executor may not write production code or issue the final
  implementation verdict.

This document is the canonical rendering of the supplied L0 intent for this
A/B artifact. The original chat transport has no stable repository path or
byte representation, so it has no independently reproducible file hash.

## Observable contract

Let `F` be the full path length and `V` the visible `SoPath` length.

| Accessor called through `SoPath` | Valid domain | Invalid result |
|---|---:|---:|
| `getNode(i)` | `0 <= i < F` | `NULL` |
| `getIndex(i)` | `0 <= i < F` | `-1` |
| `getNodeFromTail(i)` | `0 <= i < V` | `NULL` |
| `getIndexFromTail(i)` | `0 <= i < V` | `-1` |

The invalid domain includes every negative integer and every integer greater
than or equal to the relevant limit. When the path is empty, `F == V == 0`,
so every integer is invalid.

For a valid direct index `i`, the returned node and child index remain those
at full-path position `i`. For a valid tail index `i`, the returned node and
child index remain those at visible position `V - i - 1`. The head child index
remains the baseline value `0`; `-1` remains reserved here for an
invalid-index sentinel.

The visible/full distinction is preserved: a path with a hidden nodekit part
may have `V < F`; direct access through `SoPath` still observes the full
position, tail access through `SoPath` observes only the visible suffix, and
valid access through `SoFullPath` continues to observe the full suffix.
`SoNodeKitPath`'s private index accessors and bounds behavior of derived-class
overloads are outside this change; their valid full-path behavior is only a
positive control here.

An invalid accessor call must not alter either length, the sequence of valid
node pointers, or the sequence of valid child indices. It must return normally
with its sentinel. A crash, assertion termination, sanitizer finding, thrown
exception, or non-sentinel value is a violation, not an alternate diagnostic.

Diagnostics for these invalid calls are optional when the library was built
with `COIN_DEBUG`. They must not be emitted by these calls when `COIN_DEBUG` is
false. Diagnostics never replace the required return value.

## Independent A/B oracle

`testsuite/reproducers/sopath-legacy-accessor-bounds/repro.cpp` derives its
expectations only from the table above. CTest starts a fresh process for each
accessor/boundary pair so that one unsafe baseline access cannot mask another.
The boundary representatives are `-1`, `INT_MIN`, the relevant limit,
limit + 1, and `INT_MAX`; index zero is exercised on an empty path. Positive
controls cover every valid position, the baseline head child index, the
visible/full distinction, and post-call immutability.

Required configurations are independently configured and executed Debug and
Release builds. Passing only one configuration is insufficient.

## Classification and `Unknown`

- `Preserved`: every registered case exits zero in both Debug and Release,
  with the frozen test artifacts unchanged.
- `Violated`: any case observes a wrong valid value, wrong invalid sentinel,
  mutation, Release diagnostic, crash, signal, assertion termination,
  sanitizer finding, or nonzero exit caused by the exercised behavior.
- `Unknown`: a required configuration was not actually built; tests were not
  discovered; the executable could not start because of an environmental or
  dependency failure; a timeout occurred before the relevant observation; or
  the baseline/contract/test hashes do not match the frozen inputs.

`Unknown` is never success and must not be converted to `Preserved`. An
ordinary crash while evaluating an invalid accessor is classifiable as
`Violated`, not `Unknown`.

## Reproduction gate

For each of Debug and Release, configure and build the repository normally,
then run:

```sh
ctest --test-dir <build-dir> --output-on-failure \
  -L sopath-legacy-accessor-contract
```

The final verifier must record the generator, compiler, configuration flags,
test output, baseline and implementation commits, and SHA-256 hashes of this
contract, the reproducer, and the test registration. This A/B artifact covers
only the observable fragment stated above; it does not attest general
functional equivalence or implementation quality.
