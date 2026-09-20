# SoPath copy-range regression

The behavior test covers empty paths, invalid starts and counts, clamped
ranges, partial copies with a new head, hidden full-path nodes and auditing of
copies made from `SoTempPath`. Rejected inputs must return normally without
changing the source path.

```sh
sh testsuite/reproducers/sopath-copy-range-hardening/run.sh /path/to/build/lib
```

Each case runs in a separate process with a timeout and address-space limit.
The runner requires writable space in `/dev/shm` and returns nonzero on the
first failed behavior check.
