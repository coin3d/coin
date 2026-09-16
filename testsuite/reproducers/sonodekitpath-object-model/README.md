# SoNodeKitPath object-model contract probe

This directory contains a candidate Coin 4 contract and implementation-blind
probes.  It does not contain the fix and is intentionally not wired into the
normal CTest target while the frozen baseline is expected to violate several
requirements.

The probes separate:

- genuine C++ `SoNodeKitPath` subclasses (`contract` mode);
- paths returned by the public `SoBaseKit` factory (`factory` mode);
- the required additive copying API (`frompath` mode).

Configure and build a shared Coin library, then run individual gates:

```sh
sh testsuite/reproducers/sonodekitpath-object-model/run.sh /path/to/build/lib contract views
sh testsuite/reproducers/sonodekitpath-object-model/run.sh /path/to/build/lib contract equality
sh testsuite/reproducers/sonodekitpath-object-model/run.sh /path/to/build/lib contract truncate
sh testsuite/reproducers/sonodekitpath-object-model/run.sh /path/to/build/lib contract truncate-pop
sh testsuite/reproducers/sonodekitpath-object-model/run.sh /path/to/build/lib contract contains
sh testsuite/reproducers/sonodekitpath-object-model/run.sh /path/to/build/lib contract append-child
sh testsuite/reproducers/sonodekitpath-object-model/run.sh /path/to/build/lib contract append-route
sh testsuite/reproducers/sonodekitpath-object-model/run.sh /path/to/build/lib contract append-atomic
sh testsuite/reproducers/sonodekitpath-object-model/run.sh /path/to/build/lib factory factory-part
sh testsuite/reproducers/sonodekitpath-object-model/run.sh /path/to/build/lib factory factory-any
sh testsuite/reproducers/sonodekitpath-object-model/run.sh /path/to/build/lib frompath
```

The script compiles the caller with UBSan/vptr and stops on the first sanitizer
diagnostic.  A sanitizer abort, compile failure, failed assertion, or nonzero
exit is `Violated`, not a successful detection test.  Sanitizer silence alone
does not pass: each executable also checks semantic observations.

A missing required member such as `fromPath` in an otherwise configured
baseline is `Violated`.  Failure to locate the compiler, generated headers,
or requested library directory is an external setup result and must be
reported as `Unknown`, not attributed to the implementation.

Expected discriminating baseline observations include:

- historical filtering loses a non-kit head from `K`;
- inherited `operator!=` is not the complement of nodekit `operator==`;
- a factory result is not a genuine derived object and UBSan/vptr rejects its
  ordinary public use;
- append below a non-kit full tail does not reach the requested kit;
- append of a path reconstructs the first route instead of preserving the
  source child indices;
- a failed multi-kit append can leave a valid later kit partially appended;
- `fromPath` is absent until the additive API is materialized.

`truncate(M)` is a positive boundary control: its full route must remain
unchanged.  Cases classified `Unknown` in `CONTRACT.md` are not invoked by
these probes.
