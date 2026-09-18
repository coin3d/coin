# Tekt calibration receipt

- Manifest SHA-256:
  `c473f214fc569eced60562081ab870224754f9d982d8bb7980869366ed011958`
- Baseline commit:
  `40c6372d2984a94c6ac17f059caee7b8d6374654`
- Contract worktree: `/tmp/coin-sopath-range-contract`
- Contract branch: `codex/contract-sopath-range-stack`
- Compiler: `c++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0`
- CMake: `3.28.3`
- Calibration date: `2026-09-16` (`America/Sao_Paulo`)

## Reproduction commands

The baseline libraries were configured and built with:

```sh
cmake -S /tmp/coin-sopath-range-contract \
  -B /tmp/coin-sopath-range-contract-build-debug \
  -DCMAKE_BUILD_TYPE=Debug -DCOIN_BUILD_SHARED_LIBS=ON \
  -DCOIN_BUILD_DOCUMENTATION=OFF -DCOIN_BUILD_TESTS=OFF
cmake --build /tmp/coin-sopath-range-contract-build-debug --target Coin -j4

cmake -S /tmp/coin-sopath-range-contract \
  -B /tmp/coin-sopath-range-contract-build-release \
  -DCMAKE_BUILD_TYPE=Release -DCOIN_BUILD_SHARED_LIBS=ON \
  -DCOIN_BUILD_DOCUMENTATION=OFF -DCOIN_BUILD_TESTS=OFF
cmake --build /tmp/coin-sopath-range-contract-build-release --target Coin -j4
```

The resulting library hashes were:

| Configuration | Library SHA-256 |
| --- | --- |
| Debug | `572b5ab9486fa5eb4800407b34c95cc62dcea69fa235a757509a41c00fda079b` |
| Release | `194242b3c8b3048016c5bbbc560de290f055c28c3fa73e8e5675c85c256e3352` |

For each configuration and each `ORDER` in `forward reverse`:

```sh
sh testsuite/reproducers/sopath-range-stack-hardening/run.sh \
  /tmp/coin-sopath-range-contract-build-CONFIG/lib ORDER
sh testsuite/reproducers/sopath-range-stack-hardening/run-old-header.sh \
  /tmp/coin-sopath-range-contract-build-CONFIG/lib ORDER \
  40c6372d2984a94c6ac17f059caee7b8d6374654
```

## Calibration revisions

Revision 0 exposed two oracle defects while still demonstrating the baseline
boundary failures: an empty snapshot did not retain its observed length, and
three positive prefix checks assumed a root-index encoding instead of deriving
the expected prefix from public pre-call observations. The dominant oracle
reason code was `ORACLE_SNAPSHOT_LENGTH_AND_PREFIX_EXPECTATION`.

Revision 1 added the observed full length to snapshots and derives every valid
prefix from the snapshot taken immediately before the call. Focused reruns of
the four boundary classes and three positive prefix controls showed no positive
regressions and removed all false classifications. The complete corpus was
then rerun in both orders. This was one oracle revision within the budget; no
second no-gain revision occurred.

## Final baseline classification

The classification vector was identical in forward and reverse order and in
Debug and Release.

| Current-header case | Debug | Release |
| --- | --- | --- |
| `truncate_negative_empty` | Violated | Violated |
| `truncate_negative_nonempty` | Violated | Violated |
| `truncate_above_empty` | Violated | Violated |
| `truncate_above_nonempty` | Violated | Violated |
| `truncate_zero` | Preserved | Preserved |
| `truncate_current` | Preserved | Preserved |
| `truncate_intermediate` | Preserved | Preserved |
| `sopath_pop_empty` | Violated | Violated |
| `sofullpath_pop_empty` | Violated | Violated |
| `sopath_pop_nonempty` | Preserved | Preserved |
| `sofullpath_pop_nonempty` | Preserved | Preserved |

Each current-header run therefore reports
`preserved=5 violated=6 unknown=0`.

| Baseline-header client case | Debug | Release |
| --- | --- | --- |
| `old_header_sopath_pop_empty` | Violated | Violated |
| `old_header_sofullpath_pop_empty` | Violated | Violated |

Each baseline-header run reports `preserved=0 violated=2 unknown=0`.

Debug violations terminate at the baseline range assertion. Release calls
return with an invalid/mutated full length, which the complete snapshot (or the
legacy client's explicit full-length observation) rejects. The public reason
code for the product failure is `BASELINE_RANGE_REJECTION_NOT_RUNTIME_SAFE`.

## Verdict and limits

The immutable baseline is **Violated** for all six current-header invalid-call
observations and both baseline-header inline-pop observations, while all five
valid-operation controls are **Preserved**. There were no `Unknown` results.

This receipt calibrates the contract and proves order-stable discrimination of
the baseline failure in Debug and Release. It does not report a mutation score,
seal a candidate implementation, issue a refinement verdict, or act as the
final Tekt certificate. Those gates require an independent verifier and a
separately authored candidate.
