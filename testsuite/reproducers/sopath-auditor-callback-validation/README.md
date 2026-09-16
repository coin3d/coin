# SoPath auditor callback contract

Baseline: `40c6372d2984a94c6ac17f059caee7b8d6374654`.

The public `insertIndex()`, `removeIndex()`, and `replaceIndex()` callbacks must
leave the path unchanged when it is empty, when the supplied parent is not a
non-tail node of the route, or when an index is negative. A null replacement
must likewise be rejected without materializing a null route node.

Valid callbacks emitted by `SoChildList` retain their existing behavior.
Concurrent mutation, deliberately cyclic graphs, null parent pointers, and
positive indices that could not have come from the associated child list are
outside this contract and remain `Unknown`.
