# Shared-coordinate array budget

The default texture-coordinate array path evaluates one texture coordinate for
every coordinate in the active coordinate element. This is beneficial when a
shape draws most of that element, but it is disproportionately expensive when
many small shapes share a large coordinate source.

The budget test covers the ratio boundary, the absolute coordinate limit,
negative inputs, and large reference counts. The rendering investigation used
400 `SoFaceSet` nodes sharing 410,881 points, with each node drawing 1,024
quads. The array path is rejected for that layout and retained when each shape
owns the 4,096 points that it draws.

The policy and the seven affected rendering call sites derive from the
cost-bound proposal by GitHub user One-Shady-Cat in the review of PR #833:
<https://github.com/coin3d/coin/pull/833#issuecomment-6565379111>.

This test intentionally checks the deterministic policy rather than elapsed
time. Performance thresholds are unsuitable for CTest across graphics drivers;
the rendering-path and image comparisons remain part of the reproducer audit.

Build and run the shared-coordinate benchmark with:

```sh
cmake --build /path/to/build --target CoinSharedCoordinateBenchmark
LIBGL_ALWAYS_SOFTWARE=1 LP_NUM_THREADS=4 xvfb-run -a \
  /path/to/build/bin/CoinSharedCoordinateBenchmark \
  shared face explicit uv 2d 400 3 coord
```

It prints JSON lines for scene construction, first render, static renders,
coordinate-source invalidation, and restabilized renders. Each render record
includes elapsed time, resident memory, an RGB image hash, and any missing-array
diagnostics. Use the same executable with different Coin shared libraries when
comparing revisions so that only the library changes.

Set `COIN_BENCHMARK_SHARED_POINTS` to sweep the shared source size while each
shape continues to draw 4,096 vertices. The default is 410,881 points.
Set `COIN_BENCHMARK_VERTICES_PER_SHAPE` to change the submitted vertex count;
the value must be a positive multiple of four and defaults to 4,096.
