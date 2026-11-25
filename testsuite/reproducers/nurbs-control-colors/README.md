# NURBS control-point color render regression (#413)

The render test compares framebuffer samples with an independent recursive
Cox-de Boor evaluator. For Coordinate4, the reference uses the same weights
and rational normalization for the position, RGB and alpha fields. Indexed
fixtures reverse the backing arrays and supply nonidentity coordinate indices.

Enable the optional GLX tests in a build with X11 and OpenGL:

```sh
cmake -S . -B build -DCOIN_BUILD_TESTS=ON -DCOIN_TEST_NURBS_COLOR_GLX=ON
cmake --build build
xvfb-run -a ctest --test-dir build -R NurbsControlColor --output-on-failure
```

Cases cover bilinear and curved cubic surfaces, unequal positive weights,
indexed control points, both per-vertex material bindings, nonuniform and
repeated knots, shifted parameter domains, alpha blending, overall material,
explicit texture coordinates, trimming, unclamped knot domains,
independent SoMaterial transparency arrays, and object/screen complexity
settings. Cases check 25 pixel samples (9 for the smaller unclamped patch).
Tolerances account for finite tessellation and 8-bit framebuffer rounding;
they are tighter at higher complexity.

To export a fixture's framebuffer as a PPM image:

```sh
xvfb-run -a build/bin/NurbsControlColorRenderTest rational-cubic output.ppm
```

The rational color path needs GLU 1.3 tessellation callbacks. The tests need
an RGB GLX framebuffer with at least 8 bits per channel. These tests establish
finite regression coverage; they do not exhaust all possible NURBS inputs,
GLU implementations or graphics drivers.
