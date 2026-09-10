# Identity transform cache checks

The shared scenario in `testsuite/IdentityTransformCacheTest.h` runs in
`CoinTests` as `SoScale_TestSuite/identity_transform_cache_transitions`. No rendering
context or optional dependency is needed. The standalone driver runs the same
45 assertions and calls `SoDB::finish()` after destroying its scene and actions.

From the repository root, with a configured and built Coin tree:

```sh
sh testsuite/reproducers/identity-transform-node-cache-check/run.sh /path/to/build/lib
```

For a Clang ASan/UBSan build, instrument both Coin and the driver:

```sh
CXX=clang++ \
CXXFLAGS='-O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer' \
LDFLAGS='-fsanitize=address,undefined' \
ASAN_OPTIONS=detect_leaks=1 \
sh testsuite/reproducers/identity-transform-node-cache-check/run.sh /path/to/sanitized-build/lib
```

The script uses Unix library paths; the portable scenario itself also runs
through the regular CoinTests target on other platforms.

For scale and both rotation nodes, the scenario checks identity, a non-identity
value, and identity again. Bounding-box caching is enabled once. A traversal
counter verifies one rebuild after each change and reuse on an unchanged repeat.
A 2x4x6 box changes extents under the tested quarter-turn about Z, so an old box
cannot pass the rotation checks. Expected matrices and extents are specified
independently of the transform accessors.

These checks exercise matrix and bounding-box actions. They do not execute
GL rendering, validate render caches, reproduce issue #534's visual symptom,
or measure the number of matrix-element updates avoided by the optimization.
