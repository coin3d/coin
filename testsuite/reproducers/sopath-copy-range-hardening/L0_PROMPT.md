# Frozen L0 intent: `SoPath::copy` range hardening

Baseline: `2adeaa8a039067a76d098e8e2be52b431f97d18e` on
`study/sopath-range-stack-hardening`.

`SoPath::copy(startfromnodeindex, numnodes)` must have one build-independent
contract in Debug and Release:

1. `startfromnodeindex < 0` or `startfromnodeindex >= fullLength` returns
   `NULL` normally and does not mutate the source. This includes default
   `copy()` on an empty path.
2. `numnodes < 0` returns `NULL` normally and does not mutate the source.
3. `numnodes == 0` copies every remaining full-path node.
4. `numnodes > remaining` clamps to `remaining`, recovering the Open Inventor
   intent.
5. Validation must not depend on `startfromnodeindex + numnodes` being
   representable; `INT_MAX` is an observable boundary.
6. A partial copy makes its first copied node a new Coin path head, so
   `getIndex(0) == 0`, while later node identities and relationship indices
   are preserved.
7. Valid full copies keep working, and range calculations use full length,
   including nodes hidden through the ordinary `SoPath` view.
8. The returned path audits normally even when the source is an
   `SoTempPath`; the historical policy of not copying the source's disabled
   auditing state is preserved.

The contract and oracles are authored without reading or writing any future
candidate implementation. Calibration is against the immutable baseline in
Debug and Release, in forward and reverse case order. This role must not edit
production code, integrate a correction, or issue the final implementation
verdict.
