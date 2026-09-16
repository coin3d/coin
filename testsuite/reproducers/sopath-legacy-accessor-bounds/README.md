# `SoPath` legacy accessor bounds contract test

This is the independent A/B oracle for the frozen contract in
`TEKT_CONTRACT.md`. It deliberately exercises one invalid accessor per process
so a baseline crash or undefined access cannot hide the other boundary cases.

CTest registers representative negative, exact-limit, above-limit, extreme,
empty-path, valid-index, and visible/full cases. Run all registered cases in
each separately configured Debug and Release build:

```sh
ctest --test-dir <build-dir> --output-on-failure \
  -L sopath-legacy-accessor-contract
```

A crash or sanitizer report produced while evaluating an invalid index is a
contract violation. Build, discovery, loader, or environmental failures are
`Unknown`, never a pass. See `TEKT_CONTRACT.md` for the complete classification
policy and scope limitations.
