# GLX framebuffer count regression

From the repository root, with a configured and built GLX-enabled shared
Coin library:

```sh
sh testsuite/reproducers/glx-pbuffer-fbccnt-uninitialized/run.sh /absolute/path/to/build/lib
```

The script requires a GCC or Clang supporting `-ftrivial-auto-var-init=pattern`,
GLX/X11 development files, and the generated build headers. It writes its
executable to a temporary directory and removes it on exit. An unavailable
build or unsupported compiler is an error, never a passing test.

The test includes the actual `src/glue/gl_glx.cpp` and calls its private
`glxglue_context_create_pbuffer()` with mocked GLX entry points. This avoids
exporting a library symbol just for testing. It is a standalone test because
it needs private GLX state and platform headers; linking its replacement GLX
implementation into the public-API `CoinTests` executable would interfere
with other tests. No display, GPU, or `SoDB::init()` is needed.

The four cases verify exactly one chooser call each: NULL with an unwritten
count, NULL with zero count, NULL with positive count, and a valid single
configuration. Failure cases must return FALSE without attempting pbuffer or
context creation. The success case verifies those calls and the stored state.
These are mocked integration checks, not evidence of real-driver rendering.

`-ftrivial-auto-var-init=pattern` together with enabled assertions makes a
missing count initializer fail deterministically with the tested GCC/Clang
versions. To check the regression, temporarily replace `int fbc_cnt = 0;`
with `int fbc_cnt;` in `gl_glx.cpp` and rerun: the count assertion must fail.
Restore the initializer afterwards. No library rebuild is needed for this
check because the test compiles the implementation directly.

ASan does not detect uninitialized local-variable reads. The regression
above deliberately controls their contents; it is not an ASan finding.
For additional ASan/UBSan checks, use a library built with matching flags:

```sh
CXX=clang++ CXXFLAGS='-fsanitize=address,undefined -fno-omit-frame-pointer' \
  LDFLAGS='-fsanitize=address,undefined' \
  sh testsuite/reproducers/glx-pbuffer-fbccnt-uninitialized/run.sh /absolute/path/to/asan-build/lib
```

The affected implementation and driver are both instrumented by this command.
Dependencies are instrumented only if their own builds enable the sanitizers.
The mocked NULL return models an unwritten output count; this test does not
establish a driver contract violation. The fix supplies a defined initial
value. When the returned pointer is NULL, the existing short-circuit
check prevents `fbc[0]` access even in Release; the demonstrated defect is the
uninitialized count read, including the Debug assertion.
