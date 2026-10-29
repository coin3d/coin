# Profiler initialization ordering

The startup regression is registered with CTest as seven `ProfilerInit_*`
cases. Each runs a fresh process so its environment is established before
`SoDB::init()`. It cannot be an inline `CoinTests` case because that runner
initializes SoDB before running its tests.

```sh
cmake -S . -B /dev/shm/coin-build -DCMAKE_BUILD_TYPE=Debug
cmake --build /dev/shm/coin-build --parallel
ctest --test-dir /dev/shm/coin-build --output-on-failure
ctest --test-dir /dev/shm/coin-build -R '^ProfilerInit_' --output-on-failure
```

`unset`, `off`, and `0` must leave profiling disabled. `on`, `1`, and `syncgl`
must enable it. An invalid token following `on` must emit exactly one warning
and leave profiling enabled. The wrapper removes any inherited overlay setting.
The executable also calls `SoDB::init()` twice to check its existing no-op
behavior on repeated initialization.

A bounding-box traversal of a separator, two cubes and an inspection callback
must invoke the callback once and retain the expected cube bounds. When enabled,
the profiler element must be present and contain four entries; when disabled,
it must be absent. This tests collected data and normal traversal results,
not merely `SoProfiler::isEnabled()`. No graphics context or display is needed.

The standalone script compiles the same executable and invokes the same CMake
wrapper for every mode. It requires a shared Coin build and generated headers,
resolves relative library paths before changing directories and cleans up its
temporary executable:

```sh
sh testsuite/reproducers/soprofiler-initclass-ordering/run.sh /path/to/build/lib
```

For sanitizer checks, instrument Coin itself and build/run its CTest targets.
For standalone execution against that instrumented library:

```sh
CXX=clang++ CXXFLAGS='-fsanitize=address,undefined -fno-omit-frame-pointer' \
  LDFLAGS='-fsanitize=address,undefined' \
  sh testsuite/reproducers/soprofiler-initclass-ordering/run.sh /path/to/asan-build/lib
```

Reading `COIN_PROFILER` only after `SoAction::initClass()` must fail the enabled
cases. Reading it both before and after that call must fail the invalid-token
warning-count case. These are behavioral assertions; sanitizer silence is not
a substitute for either check. Rendering, overlay configuration and a complete
shutdown/reinitialization cycle are outside this test's scope.
