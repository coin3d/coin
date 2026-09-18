# `SoPath::copy` range-hardening oracle

Build Coin as a shared library, then run both clients in both orders:

```sh
sh testsuite/reproducers/sopath-copy-range-hardening/run.sh \
  /path/to/build/lib forward
sh testsuite/reproducers/sopath-copy-range-hardening/run.sh \
  /path/to/build/lib reverse
sh testsuite/reproducers/sopath-copy-range-hardening/run-old-header.sh \
  /path/to/build/lib forward
sh testsuite/reproducers/sopath-copy-range-hardening/run-old-header.sh \
  /path/to/build/lib reverse
```

Run the four commands against Debug and Release builds. Each named case runs
in a separate process with a ten-second timeout and a 512 MiB address-space
limit. Operational nonzero exits, signals, and timeouts are `Violated`;
compile/link/setup failures are `Unknown` and prevent a verdict. The aggregate
runner returns nonzero while any violation exists, which is expected for the
frozen baseline.

The old-header runner archives all public `include/Inventor` headers from the
immutable baseline before compiling. It exercises the unchanged out-of-line
ABI call path for representative empty, clamp/overflow, head-index, and
auditing cases.
