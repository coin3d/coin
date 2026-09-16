# SoPath assignment auditor contract

Baseline: `40c6372d2984a94c6ac17f059caee7b8d6374654`.

The destination of copy assignment must stop auditing its former route, audit
the assigned route exactly once, and unregister every auditor before it is
destroyed. Self-assignment must preserve the route without duplicating auditor
registrations.

The `copy-construct` case was added after implementation review exposed that
the copy constructor delegated to assignment without first initializing the
destination's auditor policy. It is an implementation-informed regression
test, not part of the original independently ordered A/B contract.

Copy-and-swap is intentionally not used here. Child lists register the address
of the auditor object. Swapping path members with a temporary does not transfer
that external identity, so destruction of the temporary would try to remove
the wrong address and leave the destination registered on its old route.

The observable cases are valid, nonempty paths. Allocation failure, malformed
paths, concurrent scene-graph mutation, and deliberately cyclic graphs are
outside this contract and remain `Unknown`.
