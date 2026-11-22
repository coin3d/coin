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
