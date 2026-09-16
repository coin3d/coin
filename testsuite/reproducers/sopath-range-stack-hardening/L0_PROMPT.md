# Frozen L0 intent: SoPath range/stack hardening

Baseline: `40c6372d2984a94c6ac17f059caee7b8d6374654`.

`SoPath::truncate` must reject, without mutation, requested lengths smaller
than zero or greater than the path's full stored length in both Debug and
Release builds. This must make an empty `SoPath::pop` and an empty
`SoFullPath::pop` safe, including a client compiled with the baseline inline
`pop` implementation that calls `truncate(-1)`.

Valid `truncate(0)`, `truncate(current full length)`, an intermediate
reduction, and `pop` on a non-empty path must preserve their existing
behaviour.

The contract and its oracles must be authored without reading or writing a
candidate implementation. Calibration is against the immutable baseline and
must record Debug and Release results. The contract author does not integrate
or implement the correction.
