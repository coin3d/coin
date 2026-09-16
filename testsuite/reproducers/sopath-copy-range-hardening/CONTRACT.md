# Tekt contract: `SoPath::copy` range hardening

## Authority and scope

This is an implementation-blind observable contract derived from
`L0_PROMPT.md`. The immutable baseline is
`2adeaa8a039067a76d098e8e2be52b431f97d18e`. The contract/oracle author may
read the frozen intent, public headers, baseline implementation, and baseline
build/test conventions, and may write only this reproducer directory on the
isolated contract branch. Any later candidate implementation or implementer
conclusion is a prohibited input.

The regime is the full segregated-materialization protocol, limited here to
the pre-implementation contract/oracle phase. This artifact prescribes only
publicly observable behavior, not a validation expression, loop form,
diagnostic, allocation strategy, or candidate patch.

## Observable contract

Let `full_length` be the node count exposed by the `SoFullPath` view of the
source. Let `start` be `startfromnodeindex`. After a valid start,
`remaining = full_length - start`.

| Inputs | Required result |
| --- | --- |
| `start < 0` | return `NULL` normally; source unchanged |
| `start >= full_length` | return `NULL` normally; source unchanged |
| `numnodes < 0` | return `NULL` normally; source unchanged |
| `numnodes == 0` | copy exactly `remaining` nodes |
| `0 < numnodes <= remaining` | copy exactly `numnodes` nodes |
| `numnodes > remaining` | copy exactly `remaining` nodes |

Consequently, default `copy()` on an empty path returns `NULL`; `start ==
full_length` is invalid rather than an empty-slice request; and `INT_MAX` is a
valid excessive count that clamps rather than overflowing, allocating from
the untrusted count, throwing, aborting, or reading outside the source.

For every non-`NULL` result of length `result_length`:

- result node `i` is source full-path node `start + i`;
- `result->getIndex(0) == 0`, establishing the Coin head convention;
- for every `i > 0`, the result index is the source full-path relationship
  index at `start + i`;
- the result has initial reference count zero, as documented, and remains a
  regular auditing `SoPath` even if the source is a non-auditing
  `SoTempPath`;
- the source's full node/index sequence is unchanged by the copy operation.

All lengths and source observations above are full-path observations. A node
hidden by the ordinary `SoPath` view does not terminate or bound the copy.
Valid complete copies, explicit or default, preserve the complete sequence.

"Returns normally" excludes an escaping exception, signal, abort, crash,
resource-exhaustion termination, timeout, or diagnostic-dependent behavior.
Diagnostic text and count are deliberately not oracles.

## Oracle design and expected discrimination

`repro.cpp` runs one named case per process, snapshots the source through
`SoFullPath`, invokes `copy`, and checks result length, node identity, head
index, later relationship indices, and source immutability. A bounded process
prevents the `INT_MAX` attack from exhausting the host. The hidden-path case
proves that the range is based on full length. The `SoTempPath` case mutates a
parent child list after copying: the returned path must adjust its relationship
index while the deliberately non-auditing source retains its stored index.

`old-header-client.cpp` is compiled against public headers archived from the
frozen baseline and linked to the selected library. Since `copy` is
out-of-line, these cases are an ABI-call-path control, not a claim that a
header-only defect exists.

The suite discriminates at least these semantic mutants:

- validation compiled only in Debug;
- omitted lower or upper start bound, including empty default copy;
- rejection of every non-positive count, incorrectly rejecting zero;
- rejection rather than clamp of excessive counts;
- evaluation of `start + numnodes` before overflow-safe normalization;
- allocation from the untrusted excessive count;
- copying the old parent relationship index into the new head;
- zeroing every copied index instead of only the new head;
- using visible `SoPath::getLength()` instead of full length;
- copying `SoTempPath`'s disabled-auditing policy into the result;
- mutation of the source during rejection or successful copying;
- unconditional `NULL`, empty, prefix-only, or full-copy behavior.

## Unknown policy and limits

`Unknown` is explicit and never passes. Compile, link, archive, runner setup,
or unavailable timeout support is `Unknown`. Once a named operation starts,
an unexpected return, exception, signal, abort, memory-limit termination,
timeout, `NULL`/non-`NULL` mismatch, state mismatch, or audit mismatch is
`Violated`. Only exit zero after all named observations is `Preserved`.

The contract does not assert diagnostic wording/count, performance beyond the
bounded safety check, thread safety, dynamic type of the returned `SoPath`,
nodekit-specific APIs, or equivalence outside this `copy` fragment. It does
not issue a mutation score, seal, implementation verdict, or final Tekt
certificate; those belong to independent later roles.

## Calibration gates

Calibration and later verification run current-header and frozen-header
clients against Debug and Release shared libraries, each in forward and
reverse order. Classification must be order-invariant. Positive baseline
controls are expected `Preserved`; observations exposing the frozen defect
are expected `Violated`. No `Unknown` result is acceptable for a verdict.
