# SoPath assignment auditor contract

Baseline: `40c6372d2984a94c6ac17f059caee7b8d6374654`.

The destination of copy assignment must stop auditing its former route, audit
the assigned route exactly once, and unregister every auditor before it is
destroyed. Self-assignment must preserve the route without duplicating auditor
registrations.

The observable cases are valid, nonempty paths. Allocation failure, malformed
paths, concurrent scene-graph mutation, and deliberately cyclic graphs are
outside this contract and remain `Unknown`.
