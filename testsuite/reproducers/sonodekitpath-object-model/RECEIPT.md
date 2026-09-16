# Candidate-contract execution receipt

This receipt records characterization of frozen baseline
`cb0b61e34cef75781967a7c9c6c747685b2045e7`.  It is not a verdict on a future
implementation and is not a protocol seal by itself.

## Artifact hashes

```text
40f3d422f315f59bb2d918d5d3be22f0d56679aa2d4770669c8414f4739cdba1  CONTRACT.md
a25b0c252235f76dfe012a9ea19487c0b74eebe1cd62f185f242b84d26e0edfb  MANIFEST.md
92688eb5b303bed65fe060c9cd0965865a279e999734d8f2f116770363ec288b  README.md
a0893190412a7fc062172b134eb68755e88c31d9e0908190e9588d807153c097  repro.cpp
3d6eeda0340c8d8fc35592c776b3517a2a110a48f1d4ffc0565f5e5f27f4bd58  frompath.cpp
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
