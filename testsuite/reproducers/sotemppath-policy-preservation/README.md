# SoTempPath policy-preservation regression

Appending a valid `SoPath` to an empty or non-empty `SoTempPath` must copy the
route without acquiring node references or enabling child-list auditing. A
later child-list mutation updates the ordinary source path but not the
temporary path.

```sh
ctest --test-dir <build-dir> -R '^SoTempPathPolicy_' --output-on-failure
```

The standalone runner accepts the build library directory and either
`nonempty-destination` or `empty-destination`. It requires writable space in
`/dev/shm`.
