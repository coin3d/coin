# SoPath child-append validation contract test

This standalone test is the independent A/B oracle described in
`TEKT_CONTRACT.md`. It tests only public API and intentionally lives outside
the production implementation.

Build Coin in the desired configuration, then run:

```sh
sh testsuite/reproducers/sopath-child-append-validation/run.sh /path/to/build/lib
```

Run it against both a Debug and a Release build. Each named case is a separate
process so crashes are attributed and do not suppress the remaining cases.
Compilation or setup failure is `Unknown`, not a pass. On an unfixed baseline,
one or more negative cases are expected to fail or crash; on a conforming
candidate, every case passes.
