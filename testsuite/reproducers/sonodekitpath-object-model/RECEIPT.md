# Candidate-contract execution receipt

This receipt records characterization of frozen baseline
`cb0b61e34cef75781967a7c9c6c747685b2045e7`.  It is not a verdict on a future
implementation and is not a protocol seal by itself.

## Artifact hashes

```text
40f3d422f315f59bb2d918d5d3be22f0d56679aa2d4770669c8414f4739cdba1  CONTRACT.md
a25b0c252235f76dfe012a9ea19487c0b74eebe1cd62f185f242b84d26e0edfb  MANIFEST.md
b844f4c70f168163ee8669e93867df9c527d22004a195001d2084c9ffd62bf7f  README.md
43cb9b2e8371a44739680b5b1e6996edbd07240e8c4228d38ee79b0d3f4af0dd  repro.cpp
1da8ae54d626169c27c7f919a1a77c31c967cfeefae85bd2da5b54274f4cb66b  frompath.cpp
0f971fca6b77b350e2ad0a2aac09231e2476796b05e71981eab97795a488394f  run.sh
```

## Build commands

```sh
cmake -S . -B /tmp/coin-nodekit-contract-debug \
  -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -DCOIN_BUILD_TESTS=OFF
cmake --build /tmp/coin-nodekit-contract-debug --parallel 8

cmake -S . -B /tmp/coin-nodekit-contract-release \
  -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DCOIN_BUILD_TESTS=OFF
cmake --build /tmp/coin-nodekit-contract-release --parallel 8
```

Both library builds completed successfully.

Each row below used:

```sh
sh testsuite/reproducers/sonodekitpath-object-model/run.sh \
  BUILD/lib contract CASE
```

The factory and additive-API probes used `factory` and `frompath` in place of
`contract CASE`.  `run.sh` compiled callers with
`-fsanitize=undefined,vptr -fno-sanitize-recover=undefined`.

## Baseline characterization

| Case | Debug | Release | Witness |
|---|---|---|---|
| `views` | Violated | Violated | non-kit head omitted from `K` |
| `equality` | Violated | Violated | inherited `!=` not complement of nodekit `==` |
| `truncate` | Preserved | Preserved | `truncate(M)` kept full route |
| `truncate-pop` | Preserved | Preserved | boundary/interior projected states |
| `contains` | Preserved | Preserved | present/absent nodekits |
| `append-child` | Violated | Violated (`SIGSEGV`) | stale non-kit full tail not removed |
| `append-route` | Violated | Violated | source child indices reconstructed as first route, for empty and non-empty destinations |
| `append-atomic` | Violated | Violated | later reachable kit partially appended after earlier join failure |
| `factory-part` | Violated | Violated | UBSan/vptr reports `SoPath` object behind `SoNodeKitPath *` |
| `factory-any` | Violated | Violated | protected factory independently returns the same false dynamic type |
| `frompath` | Violated at compile | Violated at compile | required additive member absent |

Debug diagnostics differed, but the semantic classifications matched.  The
Release crash in `append-child` is itself a stronger violation and was not
converted to a passing sanitizer test.

## Adversarial refinement

An independent adversarial pass identified missing controls for membership,
empty forks, truncation/pop boundaries, head-sensitive equality, comparison of
later `K` members, bidirectional copy independence, fork over equal `K` with
different full indices, both public/protected factories, layout/public
inheritance, and non-empty-destination route preservation.  The oracle was
extended without changing the frozen contract.

No `Unknown` case was counted as preserved.  External inability to locate a
compiler, generated headers, or a requested library remains `Unknown`; that
setup failure did not occur in these runs.

## Independent-branch verification

The two commits were subsequently rebased directly onto upstream `master` at
`40c6372d29`.  The Coin library was built in both Debug and Release modes.  The
Debug library used `-fsanitize=undefined -fno-sanitize=vptr`; each contract
caller retained
`-fsanitize=undefined,vptr -fno-sanitize-recover=undefined`.  This isolates the
`SoNodeKitPath` dynamic-type contract from the separate historical
`SoFullPath` downcast still present on `master` and tracked by coin3d/coin#714.

All three aggregate gates passed in both configurations:

- `contract all`;
- `factory all`;
- `frompath`.

The complete configured CTest suite also passed in both configurations: 8/8
tests, including the full `CoinTests` binary.  A full library and test build
completed before each execution.
