# SoNodeKitPath Coin 4 contract candidate

Status: candidate contract, not sealed.  This document is rooted in baseline
`cb0b61e34cef75781967a7c9c6c747685b2045e7` and deliberately does not select
an implementation.

## Model

For a path `P`, let:

- `F(P)` be its complete ordered sequence of nodes and child indices;
- `V(P)` be the existing `SoPath` visible prefix;
- `K(P)` be `[head(P)]` followed by every later node in `F(P)` derived from
  `SoBaseKit`.

The unconditional head is significant.  A non-empty nodekit projection is
not permitted to lose a non-nodekit head.  An empty path has an empty `F`,
`V`, and `K`.

## Required observations

For `M = |K(P)|`:

- `SoNodeKitPath::getLength()` is `M`.
- `getTail()` is `NULL` when `M == 0`, otherwise `K[M - 1]`.
- `getNode(i)` and `getNodeFromTail(i)` return the corresponding member of
  `K`; negative and out-of-range indices return `NULL` in Debug and Release.
- `containsNode()` observes membership in `K` for nodekit arguments.
- `findFork()` returns the last common position in `K`, and returns `-1`
  when either projection is empty or the heads differ.
- equality compares the complete `K`, including the head; inequality is its
  logical complement.  Nodes and child indices hidden between two projected
  nodes do not independently affect nodekit equality.
- `truncate(n)` is valid for `0 <= n <= M`, leaves the first `n` projected
  nodes, and is a no-op for `n == M`.
- `pop()` on a non-empty projection has the same projected result as
  `truncate(M - 1)`; on an empty projection it is a no-op.

Debug may add diagnostics, but defined return values and post-state are the
same in Debug and Release.

## Construction and object identity

Every non-NULL `SoNodeKitPath *` returned by
`SoBaseKit::createPathToPart()` or `createPathToAnyPart()` points to a real
C++ object whose dynamic type is `SoNodeKitPath` (or a genuine subclass).
Using the returned public type must be clean under UBSan/vptr.  Coin reference
counting rules remain unchanged: a newly returned path has reference count
zero and the caller uses `ref()`/`unref()` when retaining it.

Coin 4 also gains this additive conversion API:

```cpp
static SoNodeKitPath * SoNodeKitPath::fromPath(const SoPath * path);
```

Its contract is:

- `NULL` input returns `NULL`;
- non-NULL input returns a new genuine `SoNodeKitPath`, reference count zero;
- `F(result)` is an independent copy of `F(input)`, including child indices;
- consequently `V(result)` and `K(result)` are projections of the copied
  full route;
- later mutation of either path does not mutate the other.

`fromPath()` is an allocating/copying materialization, not a borrowed view and
not a downcast.  Coin 5 remains free to introduce a view under a different
ABI contract.

Callers that only need the logical tail already exposed by their `SoPath`
must use `path->getTail()` directly.  They must not downcast to
`SoNodeKitPath` merely to query it.

## Mutation

`append(SoBaseKit * childKit)` treats the last member of `K` as the logical
tail.  Before searching, it removes any full-route suffix below that logical
tail.  If `childKit` is reachable as a part of that tail, the first matching
route is appended and `F` remains a valid parent/child path.  In particular,
the operation works when the original full tail is a non-kit below the
logical tail.

`append(const SoNodeKitPath * source)` preserves the source's complete route,
including intermediate nodes and child indices.  A failed join is atomic:
the destination remains byte-for-byte observably unchanged through the path
API.  A successful append never reconstructs a different first-occurrence
route merely because its `K` projection is equal.

## Compatibility boundary

The Coin 4 work preserves the existing public inheritance, object layout,
symbol compatibility, and reference-counting convention.  It may add symbols
and real construction paths.  It does not authorize a Coin 5 view redesign.

## Unknown

The following are outside this candidate contract and must not silently count
as success:

- NULL arguments to existing `append`, `containsNode`, or `findFork` methods;
- `truncate(n)` for `n < 0` or `n > M`;
- structurally invalid `SoPath` routes or NULL nodes inside `F`;
- concurrent or reentrant mutation of one path;
- append of a child that is not reachable from the logical tail, except for
  the required atomic non-mutation property;
- exact diagnostic text and allocation strategy;
- behavior of an arbitrary caller-authored downcast from `SoPath *` to
  `SoNodeKitPath *` (such a downcast is forbidden, not a supported case).
