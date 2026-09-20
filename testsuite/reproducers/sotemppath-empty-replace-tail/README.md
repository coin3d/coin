# SoTempPath empty `replaceTail()` regression

The tests verify three public behaviors:

- replacing the tail of an empty path is a no-op and leaves the object usable;
- the established `NULL/-1` sentinel remains valid on a non-empty path;
- a valid non-empty replacement still changes the requested tail and index.

```sh
ctest --test-dir <build-dir> -R '^SoTempPathReplaceTail_' --output-on-failure
```

The standalone runner accepts the build library directory and one of
`valid-sentinel`, `valid-replacement` or `empty-noop`. It requires writable
space in `/dev/shm`.
