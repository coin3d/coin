# Tekt calibration receipt

- Manifest SHA-256:
  `6d22bd958395cecef5a071ceb9df3c3ba98a8a925c6b1282e93e06d20f0cdac6`
- Baseline commit:
  `2adeaa8a039067a76d098e8e2be52b431f97d18e`
- Baseline tree:
  `154e044578b2aadf3d77298efbbe5c60f5754acd`
- Contract worktree: `/tmp/coin-sopath-copy-contract`
- Contract branch: `codex/contract-sopath-copy-range`
- Compiler: `c++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0`
- CMake: `3.28.3`
- Calibration date: `2026-09-16` (`America/Sao_Paulo`)

## Reproduction commands

The immutable baseline libraries were configured and built with:

```sh
cmake -S /tmp/coin-sopath-copy-contract \
  -B /tmp/coin-sopath-copy-contract-build-debug \
  -DCMAKE_BUILD_TYPE=Debug -DCOIN_BUILD_SHARED_LIBS=ON \
  -DCOIN_BUILD_DOCUMENTATION=OFF -DCOIN_BUILD_TESTS=OFF
cmake --build /tmp/coin-sopath-copy-contract-build-debug --target Coin -j4

cmake -S /tmp/coin-sopath-copy-contract \
  -B /tmp/coin-sopath-copy-contract-build-release \
  -DCMAKE_BUILD_TYPE=Release -DCOIN_BUILD_SHARED_LIBS=ON \
  -DCOIN_BUILD_DOCUMENTATION=OFF -DCOIN_BUILD_TESTS=OFF
cmake --build /tmp/coin-sopath-copy-contract-build-release --target Coin -j4
```

The resulting library hashes were:

| Configuration | Library SHA-256 |
| --- | --- |
| Debug | `6af14474d05a64e30423ff0cb17b16f3506e6de1e886424446bb3c161d7f095f` |
| Release | `83b5af757915ac6a324d9017340462943ab6d7e74770cbe22fa6859a5d330968` |

For each configuration and each `ORDER` in `forward reverse`:

```sh
sh testsuite/reproducers/sopath-copy-range-hardening/run.sh \
  /tmp/coin-sopath-copy-contract-build-CONFIG/lib ORDER
sh testsuite/reproducers/sopath-copy-range-hardening/run-old-header.sh \
  /tmp/coin-sopath-copy-contract-build-CONFIG/lib ORDER \
  2adeaa8a039067a76d098e8e2be52b431f97d18e
```

Each named operation was bounded to ten seconds and 512 MiB of virtual
address space. Operational signals, aborts, resource-limit terminations, and
exceptions were classified `Violated`, not `Unknown`.

## Calibration revisions and cost

The declared budget allowed at most two oracle revisions and two complete
corpus rounds. Two revisions and one complete round were consumed.

Revision 0 correctly separated the basic range/head/auditing controls, but the
hidden-path fixture instantiated `SoBaseKit` without initializing nodekits.
Debug aborted in fixture setup and Release did not establish the intended
hidden-node precondition. Public reason code:
`ORACLE_NODEKIT_INIT_MISSING`. Revision 1 added `SoNodeKit::init()` and reran
only `full_default`, the two hidden cases, and the `SoTempPath` auditing
control in both configurations. No previously passing control regressed.

Revision 1 then exposed a second fixture-observation defect:
`SoTempPath *` selected `SoFullPath::getLength()` statically, so the visible
length precondition was compared to 2 using a full-length observation of 4.
Public reason code: `ORACLE_VISIBLE_VIEW_STATIC_DISPATCH`. Revision 2 measured
the visible length through an explicit `SoPath *` view. The focused rerun then
classified `hidden_full_default` as `Preserved` and
`hidden_start_after_visible` as `Violated` solely because of the copied head
index, in both Debug and Release. The full current-header and old-header
corpora were then run in both orders. No further oracle revision was attempted
after the budget was consumed.

## Final baseline classification

The vector was identical in forward and reverse order within each build.
Debug and Release intentionally differ where the baseline guards are compiled
only under `COIN_DEBUG`.

| Current-header case | Debug | Release |
| --- | --- | --- |
| `empty_default` | Preserved | Violated |
| `start_negative` | Preserved | Violated |
| `start_equal_full_length` | Preserved | Violated |
| `start_above_full_length` | Preserved | Violated |
| `start_int_max` | Preserved | Violated |
| `count_negative` | Preserved | Violated |
| `count_int_min` | Preserved | Violated |
| `full_default` | Preserved | Preserved |
| `full_explicit` | Preserved | Preserved |
| `partial_from_head` | Preserved | Preserved |
| `partial_new_head_one` | Violated | Violated |
| `partial_new_head_exact` | Violated | Violated |
| `zero_to_end_new_head` | Violated | Violated |
| `excessive_from_head` | Violated | Violated |
| `excessive_new_head` | Violated | Violated |
| `int_max_count` | Violated | Violated |
| `hidden_full_default` | Preserved | Preserved |
| `hidden_start_after_visible` | Violated | Violated |
| `temppath_result_audits` | Preserved | Preserved |

Each current-header Debug run reports
`preserved=12 violated=7 unknown=0`; each Release run reports
`preserved=5 violated=14 unknown=0`.

| Frozen-header case | Debug | Release |
| --- | --- | --- |
| `old_header_empty_default` | Preserved | Violated |
| `old_header_partial_new_head` | Violated | Violated |
| `old_header_int_max_count` | Violated | Violated |
| `old_header_temppath_result_audits` | Preserved | Preserved |

Each frozen-header Debug run reports
`preserved=2 violated=2 unknown=0`; each Release run reports
`preserved=1 violated=3 unknown=0`.

The observed product reason codes are:

- `BASELINE_COPY_VALIDATION_DEBUG_ONLY`: invalid starts and negative counts
  return `NULL` in Debug but are unsafe or return a non-`NULL` result in
  Release;
- `BASELINE_COPY_HEAD_INHERITS_PARENT_INDEX`: every partial copy starting
  after node zero retains the source relationship index as its new head index;
- `BASELINE_COPY_EXCESS_NOT_CLAMPED`: ordinary excessive counts are rejected
  in Debug and can read beyond the source in Release instead of clamping;
- `BASELINE_COPY_INTMAX_HOSTILE_ALLOCATION`: `INT_MAX` reaches capacity
  allocation and terminates with `std::bad_alloc` under the bounded runner.

The controls show that the baseline already preserves complete copies, exact
prefixes starting at the original head, full-length treatment across a hidden
node, and the historical rule that a copy of `SoTempPath` audits normally.

## Verdict and limits

The immutable baseline is **Violated** for the frozen `SoPath::copy` contract
in both Debug and Release, with no `Unknown` results and order-stable
classification. The Release build additionally demonstrates that its safety
depends on build-conditional validation.

This receipt calibrates only the contract/oracle fragment identified by the
manifest. It does not report a mutation score, seal a future candidate,
inspect or modify an implementation candidate, issue a refinement verdict, or
act as the final Tekt certificate. Those gates require separately authorized
adversary, implementer, and independent-verifier roles.
