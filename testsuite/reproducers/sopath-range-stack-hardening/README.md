# SoPath range/stack hardening oracle

Build Coin as a shared library, then run both clients in both orders:

```sh
sh testsuite/reproducers/sopath-range-stack-hardening/run.sh /path/to/build/lib forward
sh testsuite/reproducers/sopath-range-stack-hardening/run.sh /path/to/build/lib reverse
sh testsuite/reproducers/sopath-range-stack-hardening/run-old-header.sh /path/to/build/lib forward
sh testsuite/reproducers/sopath-range-stack-hardening/run-old-header.sh /path/to/build/lib reverse
```

Run the four commands against Debug and Release builds. Each named case runs
in its own process. A non-zero operation result is classified `Violated`; a
compile/link/setup failure is `Unknown` and prevents a verdict. The aggregate
script returns non-zero while any violation exists, which is expected on the
frozen, uncorrected baseline.

The old-header runner archives every public `include/Inventor` header from
the immutable baseline before compilation. Its two calls therefore retain the
historical inline `pop()` bodies even when linked to a later shared library.
