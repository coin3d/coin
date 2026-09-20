# SoPath legacy accessor bounds regression

The CTest cases exercise negative, exact-limit, above-limit, extreme and empty
indices for the legacy node and child-index accessors. Valid indices and the
visible/full path distinction remain positive controls. Each unsafe access is
isolated in its own process.

```sh
ctest --test-dir <build-dir> --output-on-failure \
  -L sopath-legacy-accessor
```
