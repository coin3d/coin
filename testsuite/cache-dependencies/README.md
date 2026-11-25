# Dependencies of open ancestor caches

`CacheAncestorDependencies` exercises the public cache API through a CPU
callback action. A dependency is recorded before opening two ancestor caches.
The dependency must reach both ancestors, across zero, one or multiple empty
cache levels. A level without a cache does not close an ancestor's cache.

The controls cover an empty stack, a valid cache before the state change, and
an unchanged element value. Changing the element value must invalidate the
dependency and both ancestors. This test runs with the legacy renderer off
and requires no display.

```sh
env -u DISPLAY ctest --test-dir /path/to/build \
  -R '^CacheAncestorDependencies$' --output-on-failure
```

`DefaultTextureCacheAncestorDependencies` covers the default mapping through
offscreen GL rendering. A shape's bounds depend on inherited complexity. Its
bounds are warmed before opening a parent cache under an empty cache level.
The test consumes the mapping, changes the inherited value, and verifies that
the parent becomes invalid while the new mapping produces the expected UVs.
It also checks that setting the same value leaves the cache valid. Two GL
contexts are used. The diagnostic reports whether the warmed bounds were reused.

```sh
ctest --test-dir /path/to/build \
  -R '^DefaultTextureCacheAncestorDependencies$' --output-on-failure
```

The GL test uses Xvfb when available, with the same pixmap settings as the
other texture cache tests. It fails if rendering fails; there is no skip code
that would turn a missing context into a successful dependency regression.
