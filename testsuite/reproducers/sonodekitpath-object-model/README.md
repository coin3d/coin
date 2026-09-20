# SoNodeKitPath object-model regression

These public-API tests cover genuine `SoNodeKitPath` objects, paths returned by
`SoBaseKit` factories and the `fromPath` copying API. They verify full-route
views, equality, truncation, containment, child/path append semantics and
failure atomicity. Callers are compiled with UBSan/vptr in addition to checking
the semantic results.

```sh
sh testsuite/reproducers/sonodekitpath-object-model/run.sh \
  /path/to/build/lib behavior all
sh testsuite/reproducers/sonodekitpath-object-model/run.sh \
  /path/to/build/lib factory factory-any
sh testsuite/reproducers/sonodekitpath-object-model/run.sh \
  /path/to/build/lib frompath
```

The runner requires writable space in `/dev/shm`.
