# Profiling path lookup regression

The portable regression cases are part of `CoinTests`, extracted from
`src/profiler/SbProfilingData.cpp`. Reconfigure CMake after adding these first
tests to the file, then build and run CTest:

```sh
cmake -S . -B /dev/shm/coin-build -DCMAKE_BUILD_TYPE=Debug
cmake --build /dev/shm/coin-build --parallel
ctest --test-dir /dev/shm/coin-build --output-on-failure
```

The cases cover:

- Lookup through the tail after registering a sibling path, including repeated
  references to the same node at different child indices.
- Timing, footprint and flags from the intended entry, using explicit values
  that differ from the parent's values.
- An empty registry, missing paths, prefix paths, cache changes and the fact
  that read-only queries do not create entries.
- A node shared by two parents at the same child index. A path registered
  under one parent must not satisfy a query under the other. Creating the
  second entry must preserve separate ancestry and metrics.

The tests use real `SoTempPath` instances (a subclass of `SoFullPath`) to
isolate this logic fix from the invalid plain-`SoPath` downcasts addressed
separately by PR #714. They exercise the public profiling API without a GPU,
rendering context, environment-enabled profiler, or optional scene-graph
features. They do not establish that those separate downcasts are safe.

A standalone check for the original sibling-path case is also available:

```sh
sh testsuite/reproducers/sbprofilingdata-getindexnocreate-offbyone/run.sh /path/to/build/lib
```

The script requires a shared Coin build and its generated headers. It resolves
relative library paths before changing directories and removes its temporary
executable on exit. Compilation or execution failure produces a nonzero exit.
For sanitized execution, build Coin itself with the sanitizer flags, then use:

```sh
CXX=clang++ CXXFLAGS='-fsanitize=address,undefined -fno-omit-frame-pointer' \
  LDFLAGS='-fsanitize=address,undefined' \
  sh testsuite/reproducers/sbprofilingdata-getindexnocreate-offbyone/run.sh /path/to/asan-build/lib
```

The assertions on returned values detect the logic defects; sanitizer silence
alone does not. To verify regression sensitivity, reverting the forward-walk
bound from `<=` to `<` must fail the sibling/missing-path case; omitting the
parent-index checks must fail the shared-node case. Both checks apply to the
actual implementation, not a rewritten copy of its algorithm.
