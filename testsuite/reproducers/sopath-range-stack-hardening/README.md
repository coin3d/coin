# SoPath range and stack regression

The behavior test checks that `truncate()` rejects lengths outside the valid
range without mutation and that `SoPath::pop()` and `SoFullPath::pop()` are
safe on empty paths. Valid zero, current and intermediate truncations, plus
non-empty pops, remain positive controls.

```sh
sh testsuite/reproducers/sopath-range-stack-hardening/run.sh /path/to/build/lib
```

Each case runs in a separate process. The runner requires writable space in
`/dev/shm` and returns nonzero on the first failed behavior check.
