# SoText2 camera fitting regression (#417)

`SoCamera::viewAll()` needs bounds before the camera has been fitted. SoText2
used the current camera's visibility test when computing those bounds, so text
outside the initial view could return an empty box and remain invisible.

The fix includes offscreen text in bounding-box traversal while retaining
visibility checks for drawing and picking. It also checks layout arithmetic,
uses conservative finite bounds where float precision would collapse an extent,
and clips the temporary RGBA buffer to the viewport. For perspective text exactly
on the camera plane, bounding-box traversal uses a near-plane-sized footprint
translated back to the anchor so that camera fitting can proceed.

## Automated tests

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCOIN_BUILD_TESTS=ON
cmake --build build -j2
COIN_GLX_PIXMAP_DIRECT_RENDERING=1 LIBGL_ALWAYS_SOFTWARE=1 \
  xvfb-run -a ctest --test-dir build --output-on-failure
```

The rendering test requires a working GLX display and exits with the CTest skip
code 77 when DISPLAY is absent. Xvfb with Mesa software rendering was used for
validation; no tests were skipped in that run.

- `SoText2BoundsAndPicking`: 38 cases across both camera types, all three valid
  alignments, six anchors, and visible/offscreen picking controls.
- `SoText2ExtendedBounds`: 45 cases covering wide/tall layouts, large spacing,
  camera-plane and behind-camera anchors, large coordinates, invalid inputs,
  cache recovery, empty strings, and zero-sized viewports.
- `SoText2ExtendedRendering`: 21 cases covering large layouts, camera fitting,
  invalid-input rejection, and partial glyph clipping against shifted reference
  images.

At master 674e74267df863dbaf50416c477bc7f918a826d8 the original bounds test
reports 36 failures; the fixed code passes all 38 cases. The full fixed CTest
suite passes 81 tests. Separate builds with assertions and SoText2.cpp
instrumented with UBSan/float-cast-overflow, and with ASan+UBSan, pass the 45
bounds and 21 rendering cases without diagnostics. This was not whole-library
sanitizer coverage, and ASan leak detection was disabled.

## Original SoXt example

`sotext2-issue417.cpp` is the original example from
https://github.com/coin3d/coin/issues/417, unchanged. Build it against a SoXt
installation linked to the Coin build being tested. For example, with both
libraries installed under an isolated prefix:

```sh
c++ sotext2-issue417.cpp -I"$TEST_PREFIX/include" \
  -L"$TEST_PREFIX/lib" -Wl,-rpath,"$TEST_PREFIX/lib" \
  -o sotext2-issue417 -lSoXt -lCoin -lXm -lXt -lX11
./sotext2-issue417
```

The native 400x400 window was captured using both SoXt 1.4.0 and 1.4.2.
For each version the same executable was run with master and fixed Coin:
master had 0 nonblack pixels; the fix displayed "Hello World!" with 683.
This validates those SoXt versions against current Coin, not a reconstruction
of the original Ubuntu 18.04/Coin 4.0.0 environment.

FreeCAD was also used as an external analysis tool: a document ViewProvider
containing the original and a distant SoText2 anchor reproduced the empty
rendering with master and displayed text after the fix. Manual-camera controls,
AnnotationScreen, and a Part box remained functional (44 verification checks).
No FreeCAD source or installed library was changed.

General font-backend metric limits, UTF-8 picking, and removal of FreeCAD's
existing bounding-box workaround are outside this change.

## ABI compatibility

The existing SoText2P getQuad/computeBBox signatures and original member layout
are retained. New 32-bit positions/bounds and size_t buffer capacity live in a
nonvirtual extension allocated and destroyed by SoText2. The old nested
SbList<SbList<SbVec2s>> grow/destructor symbols remain explicitly instantiated.
This matters even though SoText2P is internal: these implementation symbols are
exported by libCoin and the ABI gate checks their continued availability.

A RelWithDebInfo build compared with release v4.0.10 using the workflow's
abidiff/header-directory options reports zero removed functions/symbols and no
incompatible-change flag (exit 4, rather than the original exit 12). The report
still includes additions and four indirect type differences in other areas
already present before this PR. The workflow classifies the repaired result as
allowed additions. No ABI suppression or workflow-policy change was made.

All 81 CTest tests pass with GLX. An executable built before this repair also
passes its 38 bounds/picking cases while loading the repaired library. Targeted
SoText2 assertions/ASan/UBSan/float-cast-overflow builds pass 38 original bounds,
45 extended bounds and 21 rendering cases; leak detection is disabled.

The large-coordinate oracle compares translated centers against a reference
using the same font, allowing float-ULP rounding. It no longer assumes that a
bitmap box contains the pen anchor: positive left bearings are valid and
caused the prior Windows bounds-test failure. Rendering requirements and all
large-coordinate cases remain covered.


## Multi-label viewAll refinement

The initial single-label tests did not cover BUGS.txt item 219. SoCamera now
checks final-camera bounds with a bounded private-camera solver, including
pixel margins and a centering step for wide fixed-pixel labels. Three labels
render completely after one public call. See `sotext2-viewall-contract.md` for
historical failures, the chosen contract, implementation and final validation.
This adds no public API and preserves the repaired SoText2P ABI layout.
