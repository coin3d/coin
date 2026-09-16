# Segregated-materialization manifest

- Protocol: Tekt materializacao segregada, contract/oracle phase only.
- Frozen baseline: `cb0b61e34cef75781967a7c9c6c747685b2045e7`.
- Frozen intent: Coin 4 ABI/layout preservation; genuine factory objects;
  `K(P) = [head] + later SoBaseKit nodes`; safe `fromPath()` materialization;
  specified equality, fork, truncation, and append behavior.
- Contract: `CONTRACT.md`.
- Oracle sources: `repro.cpp`, `frompath.cpp`, and `run.sh`.
- Required observables: dynamic C++ type, UBSan/vptr cleanliness, `F/V/K`,
  invalid getters, equality complementarity, head-sensitive fork,
  `truncate(M)`, append below a non-kit full tail, full-route preservation,
  append failure atomicity, and independent `fromPath()` copies.
- Unknown policy: the explicit section in `CONTRACT.md`; `Unknown` never
  defaults to preserved.

## Roles and capabilities

- Intent author: parent task/user; supplied the frozen intent.
- Contract/oracle author: this task; read baseline and historical evidence,
  writes only this reproducer directory, and does not implement the fix.
- Implementer: deliberately absent and must not edit these candidate inputs
  after they are sealed.
- Verifier/adversary: deliberately absent at this phase.

This phase is **executed without attestation of isolation**: contract and
initial oracle share one author.  They do not share a future implementation,
and no implementation or final verdict is produced here.  Hashes are recorded
in `RECEIPT.md` after materialization; there is no seal while discriminating
probes still reject the baseline.
