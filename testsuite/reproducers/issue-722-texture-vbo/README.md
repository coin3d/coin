# Texture coordinates and vertex arrays

The matrix covers `SoIndexedFaceSet`, `SoFaceSet`, `SoPointSet`,
`SoIndexedPointSet`, `SoIndexedLineSet`, and `SoVRMLIndexedFaceSet`.
For each shape, it checks the image without a texture, with constant UVs in
the shader, with default mapping, with explicit coordinates, with a
normal-dependent function, with two default textures, with GL texgen,
and with generated normals. Each case renders two frames and checks that
the missing texture array diagnostic does not appear.

The **144 cases / 288 frames** cover eight texture configurations, six
shapes, and three rendering modes (immediate mode, CPU arrays, and VBOs).
`SoFaceSet` also exercises a nonzero `startIndex` with unused leading coordinates.

CMake registers the `CoinTextureArrayTest` target and the
`TextureCoordinateArrayRendering` test for the legacy GL renderer on
Linux/Unix with GLX and VRML support. CTest uses `xvfb-run` when available.
Otherwise, the test requires `DISPLAY` and returns 77 if no display is available.

```sh
cmake --build /path/to/build --target CoinTextureArrayTest
ctest --test-dir /path/to/build -R '^TextureCoordinateArrayRendering$' --output-on-failure
```

To run the original reproducer for issue #722 as well:

```sh
xvfb-run -a ./run.sh /path/to/build/lib
```

To verify rendering path selection with a Coin library built with debug
symbols, run the following after `run.sh`:

```sh
xvfb-run -a ./verify-arrays.sh /path/to/build/lib
```

The debugger checks **72 cases** (six shapes, six configurations, and two
array modes). It checks for two array draw calls per process and checks the
VBO value passed to `finishVertexArray()`. Default mapping, explicit
mapping, and GL texgen should use arrays. The normal-dependent function and
default multitexturing should use the fallback. Faces using the normal cache
also fall back when the array does not cover `startIndex` or the bindings
prevent array rendering. The visual matrix also checks the fallback results.

Local results on 2026-10-12: the build succeeded, the original reproducer
passed 3/3, the visual matrix passed 144/144 with all 288 frames correct,
and the debugger checks passed 72/72. These runs used Xvfb/Mesa with
`COIN_GLX_PIXMAP_DIRECT_RENDERING=1`. Other drivers and platforms still
require separate validation.

After updating the branch on top of `origin/master` (`ca247c1321`), ten
obsolete generated tests were removed from the reused build directory.
`CoinTests` built and passed **419 tests / 307,739 checks**. The texture
regression ran again on this base. This validation covered only a subset
of the other standalone executables registered with CTest.


## Default mapping cache

In the local setup on 2026-11-19, the cache builds on master plus the
current head of #833. The `isAffordable()` calls retain the byte limit
derived from `sizeof(SbVec4f)` and the ratio based on vertices used.
Each eligible shape retains one entry of `SbVec2f` UVs. Notifications
invalidate the mapping. Active draws retain references and release them
outside the global lock. An entry with no active draws reuses its vector
the next time it generates UVs of the same size. When the size changes,
the vector is replaced so it releases the capacity of a larger source.
There is no budget shared across all shapes.

An external registry stores the cache and removes its entry when the shape
is destroyed. `SoShapeP` keeps its existing layout. The temporary guard
retains its original layout and entry points. The cache path uses the
separate private type `SoCachedVertexArrayTexCoordGuard`.

Build the `legacy-guard-client.cpp` consumer with the baseline library's
headers, including `src/shapenodes`, and link it against that library.
Run the same executable with each libCoin to verify the original dynamic
entry points, client-allocated storage, generated array, and restored state.
The calls use Itanium symbols directly to avoid inline implementations.

The key includes bundle flags because #901 uses them to distinguish
representations of the extents. The numerical corrections from #901 and
#902 are separate. To validate a build that incorporates those corrections,
`COIN_CACHE_SOURCES=wide-bounds` selects an extreme box in `cache.sh` whose
representation changes with inherited state while the encoded origin and
size remain the same. The reference calculates UVs independently in
double precision. This optional case requires the numerical correction
from #901. The standard cases run without that correction.

The additional control uses the existing benchmark from #833:

```sh
cmake --build /path/to/build --target CoinSharedCoordinateBenchmark
COIN_GLXGLUE_NO_PBUFFERS=1 xvfb-run -a sh ./verify-cache-budget.sh \
  /path/to/build/bin/CoinSharedCoordinateBenchmark
```

It requires zero UV generations for the shared source of 410,881 points
with draws of 4,096 vertices. For the eight individual sources, it
requires 16 generations, eight allocated entries, and eight GL subdata
updates. The edit reuses the entries and buffers. Constructors and
generations have separate counters. The performance measurements below
come from the 2026-10-12 run.

The optimization caches the bounding box with the dependencies read by
`computeBBox()`, the UV array, and an optional VBO. The shape retains one
current entry. Each draw keeps its own reference to the storage.
The cache rebuilds when the data or mapping parameters change. The 2D
array preserves S/T from the existing callback, with R=0 and Q=1.

`CoinTextureCacheTest` and the CTest `DefaultTextureCoordinateCache`
compare **768 frames** across eight sources/scenarios, with and without
parent display lists, three rendering paths, eight phases, and two
contexts. The reference uses explicit UVs calculated independently of
the cache. The scenarios include `SoVertexProperty`, inherited coordinates,
homogeneous coordinates, a shape shared between states, a bounding box
that depends on an inherited element, and empty bounds supplied by an
extension. Two additional scenarios exercise the six ordered S/T axis
pairs using 3D and homogeneous sources and bounds changes caused by
inherited state. The phases change positions, indices, array size,
degenerate bounds, and state. The test also builds the parent display list
from an already cached result to exercise dependency propagation.

```sh
cmake --build /path/to/build --target CoinTextureCacheTest
ctest --test-dir /path/to/build -R '^DefaultTextureCoordinateCache$' --output-on-failure
xvfb-run -a ./verify-cache.sh /path/to/build/bin/CoinTextureCacheTest
```

In Debug, `verify-cache.sh` verifies seven UV and bounding-box generations
in sixteen frames per process. The unchanged frame reuses both. The VBO
path performs fourteen UV uploads (one per generation in each context),
including subdata updates. The test requires reuse. The CPU array path
performs zero uploads. This covers six runs and 96 frames.

When an exclusive entry changes without changing size, `glBufferSubData`
updates the VBO for the current context. Other contexts are invalidated
and upload on next use. Size changes use the normal allocation path.
`SoVBO::updateBufferData()` preserves the binding and adds one nonvirtual
method. The fields and layout of `SoVBO` stay the same.

## Library performance comparison

The benchmark runs separately from the tests and imposes no time limit
in CI. To reproduce the comparison, keep the before and after libraries
in separate directories, build the same executable, and run it with each
library in alternating order in one Xvfb session:

```sh
c++ -O2 benchmark.cpp -o /tmp/coin-texture-benchmark \
  -I/path/to/build/include -L/path/to/build/lib -lCoin -lGL
xvfb-run -a python3 benchmark.py /tmp/coin-texture-benchmark \
  /path/to/before/lib /path/to/after/lib --output /tmp/coin-texture-benchmark.json
```

Local results compare PR #833 (`8a531fef49`) with the bounding-box, UV,
and VBO cache. Both runs used the same scene and executable, Debug
libraries, Mesa/llvmpipe, and a 128×128 viewport. Each cell is the median
of three runs of 30 frames after five warm-up frames. Timing includes
`glFinish()` and excludes scene construction and final image readback.
The image matched in every case. These measurements apply to the Debug
software rendering setup.

| Faces | PR #833 (ms/frame) | With cache (ms/frame) | Additional reduction |
| ---: | ---: | ---: | ---: |
| 10000 | 5.55 | 2.65 | 52.2% |
| 40000 | 23.26 | 12.43 | 46.6% |
| 160000 | 66.36 | 23.32 | 64.9% |

The cache retains CPU memory and GL resources between frames to avoid
recreating the temporary array. The previous entry is released when it
is no longer in use. `SoVBO` manages the resources for each context.
Draws that use the same entry serialize access to its VBO. The above
tests use two contexts in sequence and cover sequential rendering.

## Shutdown and Release validation (2026-11-19)

The external registry belongs to the `SoShape` initialization and cleanup
lifecycle. It has no static C++ destructor, so scenes released by
`atexit` callbacks registered before the first render can still remove
their entries. Coin cleanup destroys the registry and clears its pointer
to permit further init/finish cycles. The `DefaultTextureCacheLifecycle`
tests cover late cleanup and three cycles with CPU arrays and VBOs.
The optional `lifecycle.gdb` control should report three registry
constructions and three destructions when running `property cycles`.

Local Linux/GCC 13 validation passed 130/130 CTests in Debug, 130/130 in
Release, 118/118 in Release with the legacy renderer disabled, and
224/224 in the combined build with #901/#902. There were zero skips.
The same late cleanup test aborts in the library before the fix and
passes on the baseline and the corrected library. The Debug ABI check
without suppressions found zero removed or changed functions. In Release,
zero baseline dynamic symbols were removed.

The updated benchmark uses the same scenes and executables, alternating
only the Release libCoin from master plus #833 (`44025052aea`) and the
cache build. It uses Mesa/llvmpipe, a 128x128 viewport, four driver threads,
and three alternating runs. Timing covers `render()` with `glFinish()`
in the scene graph and excludes readback and hashing. Each steady-state
phase has 30 frames for meshes and ten for scenes with 400 shapes.
Image hashes match across libraries. RSS covers the entire process,
including Mesa and the allocator. It does not measure live cache memory
or VRAM by itself.

| Scene | Steady state before/after (ms) | Steady state RSS before/after (MiB) |
| --- | ---: | ---: |
| 10000 faces, one mesh | 5.035 / 4.732 | 120.941 / 120.789 |
| 40000 faces, one mesh | 13.142 / 13.064 | 133.316 / 133.145 |
| 160000 faces, one mesh | 24.696 / 24.249 | 184.586 / 184.594 |
| 400 shapes, shared source | 93.850 / 85.311 | 203.414 / 203.836 |
| 400 shapes, individual sources | 85.601 / 72.181 | 232.141 / 251.879 |

For the scene with individual sources, the first frame after invalidation
went from 95.537 to 107.895 ms. The new steady state went from 82.095 to
63.657 ms with RSS of 251.527 / 291.398 MiB. Retaining and rebuilding
the buffers has a cost. The explicit-coordinate control with 40000
faces went from 12.499 to 13.580 ms, and the untextured control went
from 12.268 to 12.171 ms. The changes for single meshes are small
relative to these control variations, leaving the overall effect on
single meshes uncertain. The earlier Debug results apply to their
original software rendering setup.

To repeat with the first render, invalidation, RSS and peak RSS, and scene cleanup:

```sh
cmake --build /path/to/release --target CoinDefaultTextureCacheBenchmark CoinSharedCoordinateBenchmark
LP_NUM_THREADS=4 LIBGL_ALWAYS_SOFTWARE=1 COIN_GLXGLUE_NO_PBUFFERS=1 xvfb-run -a \
  python3 benchmark.py /path/to/release/bin/CoinDefaultTextureCacheBenchmark \
    /path/to/baseline/lib /path/to/candidate/lib \
    --shared-binary /path/to/release/bin/CoinSharedCoordinateBenchmark \
    --output /tmp/cache-release-benchmark.json
```

## Reuse and invalidation cost (2026-11-20)

The local comparison used baseline `44025052aea`, the previous cache
`af4847c6f6b`, and the version with reuse `c12131a4a97`. It used the
Release libraries, executable, scenes, and Mesa/llvmpipe settings above.
The full comparison of the previous and final versions kept three
alternating runs and the explicit-coordinate and untextured controls.
All hashes matched. External machine load rose from 23.6 to 35.3, and
elapsed times varied widely in the controls as well. This run leaves the
overall effect on latency uncertain.

A second comparison of the three libraries also measured process CPU time
summed across threads, rotating the order with each repetition. Each
library/layout ran three times with ten frames in each steady phase and
`glFinish()` before the end of each sample. Readback and hashing were
outside both timers. Load stayed near 32. CPU time reduces the effect of
descheduling and remains sensitive to clock frequency and memory contention.

400 shapes with individual sources, medians of three runs:

| Phase | Baseline CPU (ms) | Previous cache CPU (ms) | CPU with reuse (ms) | RSS previous / with reuse (MiB) |
| --- | ---: | ---: | ---: | ---: |
| First render | 403.073 | 428.501 | 445.653 | 251.160 / 251.012 |
| Steady state | 167.544 | 148.832 | 148.782 | 258.074 / 257.691 |
| After invalidation | 186.688 | 212.116 | 193.900 | 290.750 / 280.332 |
| New steady state | 168.734 | 137.534 | 141.270 | 291.801 / 282.949 |
| After scene and context cleanup | — | — | — | 280.273 / 263.070 |

Reuse reduces CPU time by 8.6% and RSS by 10.4 MiB on the invalidated
frame. CPU time stays 11.2% below baseline in steady state and 16.3% below
baseline after the edit. Invalidation still costs 3.9% more CPU time than
baseline. The first render took longer, and the new steady phase took 2.7%
more CPU time than the previous cache. Steady memory use remains similar
to the previous cache. The cache retains additional memory compared with
baseline (257.691 MiB versus 233.285 MiB in this scene).
RSS and peak RSS cover the whole process, including the driver and allocator.

The shared source exceeds the UV cache budget. Previous/final CPU time
was 205.991 / 208.363 ms in steady state and 211.598 / 213.126 ms in the
new steady state. This control varies even though it generates no UV
arrays. An experiment without UV VBOs reduced memory use on this machine.
The final version retains the VBOs and updates their existing allocations
when possible. These measurements reflect the local software renderer.

Validation passed the 130 existing tests plus the direct buffer regression
in Debug and Release, 118 with the renderer disabled and `DISPLAY`
unset, and 225 in the local build with #901/#902, plus the 96 frames in
the `wide-bounds` case. There were zero skips.
`DefaultTextureBufferReuse` checks bytes, binding, reuse of the GL
identifier, resizing in both directions, and invalidation in two contexts
for vertex and index buffers. The GDB controls require unchanged mapping
generation and upload counts and verify reuse of eight entries and VBOs.

The ABI comparison against master plus #833 used
`abidiff --no-default-suppression --no-show-locs` and found zero removed
or changed functions. The baseline Release symbols and the old consumer
are preserved. The comparison uses no suppressions, and `SoShapeP`,
`SoVBO`, and the original guard retain their layouts. A separate
comparison against the previous cache shows the removal of a private
weak instance of `std::vector<SbVec2f>::_M_default_append` emitted by
`resize()`. The unpublished cache added that symbol, and the baseline
does not contain it.

Local data and commands: `/tmp/coin-cache-memory-20261120/RESULTS.txt`,
`benchmark-cpu.json`, `benchmark-previous-final.json`, `final-threeway.json`,
validation logs, and `release-symbols.json`. This validation was local.

## Batch regeneration (2026-11-20)

Generation iterates directly over the 3D or homogeneous array in the
bundle. The same 2D normalization routine serves `get()`, the callback,
and batch generation. `SoShape` reuses that routine. The homogeneous
path keeps `getReal()`. This change preserves exact division, existing
floating-point flags, and the existing memory limit. It adds a private
nonvirtual method to the bundle without adding fields. The scalar path
still computes coordinates directly. With the standard Release flags,
the compiler vectorized the 3D loop using `divps`.

Temporary per-thread CPU probes in the previous code measured 7.738 ms
for UV generation, 7.302 ms for rebuilding bounds, and 1.781 ms in
`updateBufferData` when editing 400 shapes. These timings point to
per-vertex dispatch overhead across 1,638,400 coordinates. The probes
were removed before benchmarking. They do not cover all parallel driver
work or the initial upload.

The new comparison used a Ryzen 7 5800H with 16 logical CPUs, llvmpipe
with four threads, Release libraries, and the same executable. It
compared baseline `44025052aea`, the previous version `0453097c069`, and
batch regeneration `d4954e73d27`. Builds ran in RAM, with no concurrent
builds or tests from this validation. Load was 2.09 at the start and
3.37 at the end. The scene with individual sources and VBOs ran six
times, one for each order of the three libraries. Each run had ten
steady-state frames. CPU time sums process threads and wall time includes
waiting. Both include `glFinish()`.

| Phase, 400 shapes with individual sources and VBOs | Baseline CPU (ms) | Previous CPU (ms) | Batch CPU (ms) |
| --- | ---: | ---: | ---: |
| First render | 220.106 | 227.880 | 221.770 |
| Steady state | 93.765 | 83.566 | 83.900 |
| After invalidation | 105.453 | 109.284 | 102.747 |
| New steady state | 92.990 | 78.298 | 78.278 |

The previous version again showed a CPU cost above baseline after
invalidation, 3.6% in this run versus 3.9% in the earlier measurement.
Batch regeneration was **2.6% below baseline** and 6.0% below the previous
version after invalidation. All six batch CPU samples were below all six
previous samples. Wall time after invalidation was 83.089 / 85.468 /
79.211 ms for baseline / previous / batch. CPU gains against baseline
remain 10.5% in steady state and 15.8% in the new steady state. Batch
regeneration used 0.4% more steady-state CPU time than the previous
version, with essentially the same CPU time in the new steady state.

Previous/batch RSS was 251.867 / 251.903 MiB in steady state,
280.228 / 280.158 MiB after invalidation, 280.919 / 280.928 MiB in the
new steady state, and 282.140 / 282.067 MiB at peak. The memory savings
from the previous step were preserved. After scene and context cleanup,
RSS went from 262.760 to 267.339 MiB. RSS includes the driver and
allocator, and does not measure live cache memory or VRAM. No manual
trimming was performed. Steady-state memory use still exceeds baseline.

In controls with three runs and no VBOs, CPU time after invalidation
was 91.958 / 92.818 / 86.832 ms for baseline / previous / batch. The
improvement also appears without GL updates. For the shared source
without UV caching, previous/batch CPU time in steady state was
110.806 / 110.728 ms. A second full comparison confirmed lower CPU time
after invalidation with individual sources: 110.523 / 102.783 ms.
Steady-state time for single meshes was 2.239 / 2.374 ms (10k),
11.247 / 11.269 ms (40k), and 22.651 / 22.120 ms (160k) for previous/batch.
The explicit-coordinate and untextured controls also varied
(12.018 / 11.278 and 11.337 / 11.757 ms). These results leave the overall
effect on single meshes and the behavior on physical GPUs uncertain.

Final validation passed 131/131 in Debug, 131/131 in Release, 118/118
with the renderer disabled and `DISPLAY` unset, and 225/225 with
#901/#902. There were zero skips. The combined build keeps
`coin_bundle_ratio` from #901 in the shared normalization routine and
also passed the 96 extreme-case frames. The actual S/T arrays in all
eight scenes matched bit for bit across baseline, previous, and batch.
In the combined build, nine scenes including `wide-bounds` matched
between previous and batch. The 4D baseline's R/Q values were verified
as 0/1. GDB confirmed unchanged generation counts, budgets, uploads,
and reuse.

The unsuppressed Debug ABI comparison against master plus #833 found
zero removed or changed functions. The Release comparison found zero
removed symbols against both baseline and the previous version, with one
new method relative to the previous version. The consumer built with the
old headers passed in Debug and Release. `SoShapeP`, `SoVBO`, the bundle,
and the original guard retain their fields and layouts. Local evidence in
`/tmp/coin-cache-invalidation-20261120/` includes `benchmark.json`,
`benchmark-full.json`, `values*.py`, dumps, validation logs, full ABI
output, and `RESULTS.txt`.

To repeat the bit comparison with Debug libraries and their parameter
symbols, use the same executable and scenes for both libraries:

```sh
COIN_GLXGLUE_NO_PBUFFERS=1 xvfb-run -a sh ./verify-cache-values.sh \
  /path/to/debug/bin/CoinTextureCacheTest /path/to/before/lib /path/to/after/lib
```

The verifier requires 16 draws per scene and 32 for the shared scene.
It compares the complete S/T bytes and rejects R/Q values other than
0/1 in the baseline's 4D representation. For the combined build,
`COIN_CACHE_SOURCES=wide-bounds` selects the extreme case for both
combined libraries, before and after batch regeneration.

## Ancestor cache dependencies (2026-11-20)

`SoCacheElement::addCacheDependency()` now walks the entire stack,
propagating dependencies to every open cache even across empty levels.
Stopping at the first empty level was existing behavior in master plus
#833. Reusing cached bounds exposed it as a mapping regression because
the ancestor cache stayed valid when inherited state changed.

The new `CacheAncestorDependencies` regression (CPU, including
renderer disabled) and `DefaultTextureCacheAncestorDependencies`
regression (GL, two contexts) fail with the previous cache
`f33965ff0ea` and pass with `d8173489e12`. The GL test also passes on
baseline `44025052aea`, which recalculates the bounds. The controls
require valid caches before changes and after setting the same value.
Changing complexity invalidates both ancestors and produces the expected
new UVs. Diagnostics confirm that the corrected version still reuses
the bounds. See [cache-dependencies](../../cache-dependencies/README.md).

Validation passed **133/133 in Debug, 133/133 in Release, 119/119 with
the renderer disabled and `DISPLAY` unset, and 227/227 with #901/#902**,
with zero skips. The GL regression also passed on the physical Radeon
GPU. The GDB controls still verify generation counts, uploads, budget,
and reuse. The combined build passed all 96 extreme-case frames again.
All new builds and temporary compilation files were kept in RAM.

The unsuppressed Debug ABI comparison found no differences between the
previous and corrected cache (`abidiff --no-default-suppression --no-show-locs`,
empty output, status 0). Against master plus #833 it found zero removed
or changed functions. Dynamic Release symbols are identical to those
of the previous cache. The consumer built with the old headers passed
in Debug and Release. The fields and layouts remain unchanged.

The updated Release benchmark used the same executable, 400 shapes with
individual sources and VBOs, 4096 points per shape, ten steady-state
frames, and six runs covering every order of the three libraries.
There were no concurrent builds or tests, and the load was 1.01 at the
start and 1.43 at the end. Median process CPU times in milliseconds:

| Driver / phase | Baseline | Previous cache | Corrected |
| --- | ---: | ---: | ---: |
| Radeon / steady state | 21.847 | 2.557 | 2.623 |
| Radeon / after invalidation | 26.109 | 15.147 | 14.900 |
| Radeon / new steady state | 21.722 | 2.606 | 2.713 |
| llvmpipe / steady state | 91.784 | 83.127 | 83.000 |
| llvmpipe / after invalidation | 102.643 | 101.632 | 100.886 |
| llvmpipe / new steady state | 91.193 | 77.048 | 76.086 |

Wall time after invalidation for baseline / previous / corrected was
27.813 / 16.195 / 16.855 ms on Radeon and 80.778 / 75.873 / 76.208 ms on
llvmpipe. Changes in wall time relative to the previous cache vary across
runs, so the timing effect of this correction remains uncertain.
Previous/corrected steady-state RSS was 130.674 / 130.736 MiB on Radeon
and 252.036 / 252.004 MiB on llvmpipe. The cache retains its gains
against baseline and its additional memory use. Image hashes match
across all three libraries for each driver, before and after the edit.

Local evidence includes `/tmp/coin-cache-dependency-fix-20261120/RESULTS.txt`,
`benchmark-threeway.json`, scripts, and complete logs. This validation
was local.

### GLX image test backend

The CTests `TextureCoordinateArrayRendering` and
`DefaultTextureCoordinateCache` set `COIN_GLXGLUE_NO_PBUFFERS=1` and
`COIN_GLX_PIXMAP_DIRECT_RENDERING=1`. This makes them use GLX pixmaps
under Xvfb, as the dependency, buffer reuse, and lifecycle tests do.
The tests can run regardless of the pbuffer capabilities advertised by
the runner's driver. CTest sets the variables itself. Manual script
commands can use the same variables. If `render()` fails, the test
reports the scene, phase, and context to stderr and records a test
failure. The Xvfb wrappers use `-screen 0 1280x1024x24 -noreset`, keeping
the server available between comparison processes and init/finish cycles
even when the last client disconnects.
