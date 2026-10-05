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
