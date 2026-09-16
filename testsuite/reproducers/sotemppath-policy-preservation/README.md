# `SoTempPath` policy-preservation contract

## Frozen inputs and authority

- Baseline: `40c6372d2984a94c6ac17f059caee7b8d6374654`.
- Coin 4 obligation: appending a valid `SoPath` to an empty `SoTempPath`
  retains Coin's route-copy semantics while preserving the temporary path's
  `auditPath(FALSE)` and `nodes.addReferences(FALSE)` invariants. Existing
  behavior for valid non-empty destinations is preserved.
- Contract/test author may read the obligation and baseline and write only
  this oracle. It must not read or edit an implementation candidate.
- A later implementer must treat this directory and its CMake registration as
  protected input.

This is the contract/oracle phase of the Tekt full protocol. It is segregated
from the future implementation, but is not a final verification or a claim of
general `SoTempPath` correctness.

## Observable contract

`empty-destination` requires:

1. The full node/index route is copied exactly from a valid ordinary `SoPath`.
2. Appending the route does not increment any route-node reference count.
3. A later child-list mutation updates the ordinary source path but does not
   update or truncate the `SoTempPath`.

The child is held by an explicit test reference, so observing the deliberately
stale temporary route does not dereference a destroyed object.

`nonempty-destination` is the positive preservation control. It joins a source
whose head is already the temporary path's tail, then checks the same route and
non-ownership properties. The frozen baseline is expected to preserve this
case and violate `empty-destination`, because its empty branch delegates to
`SoPath::operator=()` and imports the source's ownership/auditing flags.

Valid behavior must be the same in Debug and Release. Extra diagnostics are
permitted but are not part of the oracle.

## `Unknown` policy

The oracle makes no claim about:

- a null `frompath` pointer;
- paths that cannot be joined according to `SoPath::append()`'s documented
  route relation;
- dangling nodes or mutations followed by dereferencing destroyed nodes;
- exact notification ordering, diagnostic text, allocation capacity or ABI;
- direct, explicitly qualified calls to protected/private policy mechanisms;
- unrelated `SoPath` and `SoFullPath` boundary behavior.

These inputs are `Unknown`, never implicit successes. An unrecognized runner
case exits `2`.

## Commands

With tests enabled in a configured build:

```sh
cmake --build build --target SoTempPathPolicyContract
ctest --test-dir build -R '^SoTempPathPolicy_' --output-on-failure
```

Run against an existing shared-library build:

```sh
sh testsuite/reproducers/sotemppath-policy-preservation/run.sh \
  /absolute/path/to/build/lib nonempty-destination
sh testsuite/reproducers/sotemppath-policy-preservation/run.sh \
  /absolute/path/to/build/lib empty-destination
```

Repeat both cases for Debug and Release; sanitizer flags may be supplied via
`CXXFLAGS` and `LDFLAGS`.
