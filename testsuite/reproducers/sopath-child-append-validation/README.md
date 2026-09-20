# SoPath child-append regression

This public-API test checks that invalid child appends fail without changing
the path and that valid node and path appends retain their route and indices.
Each case runs in a separate process so a memory-safety failure cannot hide
later cases.

With a configured shared-library build:

```sh
sh testsuite/reproducers/sopath-child-append-validation/run.sh /path/to/build/lib
```

The runner requires writable space in `/dev/shm` and returns nonzero if any
behavior check fails.
