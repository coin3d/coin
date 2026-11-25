# SoLevelOfDetail bounding-box traversal (issue #374)

Issue: https://github.com/coin3d/coin/issues/374

`SoLevelOfDetail::getBoundingBox()` contained a `return` immediately after
`break` in the shared `OFF_PATH` / `IN_PATH` case. The issue discussion asked
for a test to clarify whether these cases should return early. The regression
checks are in [`SoLevelOfDetailBoundingBoxTest.cpp`](../../SoLevelOfDetailBoundingBoxTest.cpp)
and run through CTest as `LevelOfDetailBoundingBox`.

## Cases and expected behavior

- `NO_PATH`: calculate the union of all children's geometry, cache it, reuse
  it on repetition, and rebuild it after a cube's dimensions change.
- `BELOW_PATH`: when a path ends at the LOD, include all children and reuse
  the complete cache.
- `IN_PATH`: when a path continues to one geometry branch, compute that
  branch's box, even with a valid complete cache. Repeated partial traversal
  must execute again; it must not create or overwrite the complete cache.
- `OFF_PATH`: visit the state-affecting callback in the earlier sibling LOD,
  without including that LOD's geometry in the target sibling's bounding box.
  Check with and without an existing complete cache, and verify that the
  complete cache remains usable afterward.
- Camera-space traversal: bypass an existing cache, recompute on every
  traversal, and preserve the complete cache for normal traversal afterward.

The scene contains two translated cubes with independently known extents:
`[-6,-2,-3]..[-4,2,3]` and `[7,-1,-1]..[13,1,1]`. Their complete union is
`[-6,-2,-3]..[13,2,3]`; the reported center is `[2.5,0,0]`. A callback counts
actual child traversal. Child separator caches are disabled so they cannot
mask LOD cache behavior. No GL context or rendering backend is required.

## Run

```sh
cmake -S . -B build-issue374 -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug -DCOIN_BUILD_TESTS=ON
cmake --build build-issue374 --target CoinLevelOfDetailBoundingBoxTest
ctest --test-dir build-issue374 -R '^LevelOfDetailBoundingBox$' -V
```

## Recorded validation

Validated on Linux with GCC 13.3.0 in Debug configuration:

| Source variant | Result |
| --- | --- |
| Original code with `return` after `break` | 5 tests / 172 checks passed |
| Corrected code with unreachable `return` removed | 5 tests / 172 checks passed |
| Deliberate mutation: reachable `return` before `break` | 2 of 5 tests failed (`IN_PATH` and `OFF_PATH`) |
| Corrected code restored after mutation | 5 tests / 172 checks passed |

The mutation causes the `IN_PATH` bounding box and center checks to fail and
skips the observable callback traversal in `OFF_PATH`. It confirms that the
tests can distinguish continuing traversal from returning early. The original
and corrected code produce the expected results, supporting removal of the
dead statement without changing the intended flow. The mutation is not part
of the committed source.
