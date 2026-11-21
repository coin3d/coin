# Coin XML resource limit study

This document originated on the `codex/work/cc-xml-limits` study branch, based on `codex/pr/cc-xml-parser-transactional`. The candidate implementation on this branch is based on `codex/pr/cc-xml-parse-length-guard`; it adds per-document configurable quotas, but it deliberately **does not choose global values**. The `size_t` to `int` guard covers only the size of one `XML_Parse()` call, not the total size of all chunks or of the resulting DOM.

## Resource surfaces and cost

| Resource | Required input/counting point | Risk if measured too late |
| --- | --- | --- |
| Cumulative XML input bytes | `cc_xml_doc_read_buffer_x`, both partial parsing functions, and every read performed by `cc_xml_doc_read_file_x` | Many individually small chunks can exceed any per-call guard. |
| External DTD resource bytes | The resolver callback on the experimental libxml2 branch; count the per-document total, including repeated resources | A resource chain can exceed the budget even if every individual resource fits. |
| Expanded text and attributes | Expat character-data and element-start callbacks; on the DTD path, also before libxml2-to-Expat serialization | Entities and default attributes can multiply the input. Source XML bytes do not bound the DOM size. |
| Elements and attributes | Before `cc_xml_elt_new*` and `cc_xml_attr_new_from_data` in the callbacks | Counting after allocation does not prevent the memory peak. CDATA also creates a node; coalescing changes the count, so the contract must specify whether it counts events or retained nodes. |
| Depth | Before pushing onto `parsestack` and before recursion in consumers and serializers | The parser stack and recursion in `cc_xml_elt_calculate_size()` or `cc_xml_elt_write_to_buffer()` can fail before a byte or node limit is reached. |
| Serialized output | `cc_xml_doc_calculate_size`, `cc_xml_elt_calculate_size`, and buffer/file output | The calculated size can overflow `size_t`; `*bytes + 1`, recursive indentation, and the complete output buffer can allocate excessive memory. |

On the experimental DTD path, libxml2 retains its own tree, then materializes another UTF-8 buffer, and only then does Expat create the Coin DOM. A limit applied only to the Coin DOM does not cover those peaks. External resources are supplied exclusively through an application resolver callback; parsing does not authorize automatic file or network access. Validation and the second parse must either share one budget or check limits before each materialization, accounting for peak memory use.

## Semantics to settle before choosing values

1. Define configurable per-document limits, with `0` meaning disabled, or use an explicit enable flag. Decide separately whether the legacy API remains unlimited for compatibility and whether an opt-in safe profile provides defaults. Do not change defaults in this study.
2. Define the unit of every counter: received bytes, expanded UTF-8 bytes, persistent nodes or events, attributes per element and/or in total, root depth equal to `1`, and output bytes with or without the terminator. Check budgets with subtraction before addition to avoid overflow.
3. Ensure that a limit failure stops the parser, returns `FALSE`, and preserves the previous root and filename through transactional rollback. A new read or partial parsing session starts with reset counters. Specify how callers distinguish limit, syntax, and I/O failures without depending on debug messages.
4. Apply the same contract to files, buffers, and partial chunks. For DTDs, include external bytes and the expanded representation; ensure that the libxml2-to-Expat pass neither relaxes nor incorrectly duplicates the budget.
5. Decide whether serialization needs its own budget for DOMs built by the application. It cannot rely on counters collected while parsing.

## Contract implemented by the current candidate

`cc_xml_limits` is configured while a document is idle; zero disables the corresponding quota. The legacy default remains unlimited. Applications must select values appropriate for their trust model and expected document size; the four SCXML examples discussed below are not enough to justify a universal preset.

The counters measure cumulative XML input bytes for the complete parse session; bytes reported by Expat for element and attribute names, attribute values, and character data; total element start events; total attributes; and element depth, with the root at depth `1`. Closing tags are not included in expanded bytes. Elements and attributes are counted before filtering, so discarding an element does not refund the budget. The element quota does not count CDATA nodes, although their contents are indirectly bounded when the expanded-byte quota is active.

Checks run before allocating each Coin DOM element, attribute, or CDATA node. Exceeding a quota stops Expat, returns `FALSE`, exposes the first reached quota through `cc_xml_doc_get_limit_hit()`, and preserves the previous tree through rollback. File input accumulates across 8 KiB reads, and both partial parsing paths accumulate across chunks. Starting a new parse session resets the counters and result. Changing quotas during a partial parsing session is rejected. Arithmetic is checked before addition even when a quota is disabled.

There is not yet a serialization quota for an application-built DOM, nor a quota covering the libxml2 stage of the experimental DTD branch. The DTD branch cannot be presented as a safe parser for untrusted XML based only on the Expat quotas: libxml2 expansion and allocation occur before those checks. Both areas need separate implementation and validation before the DTD work can be proposed.

## Initial evidence and measurements still needed

The four versioned SCXML files under `data/scxml/navigation/` are 480, 4,123, 13,377, and 16,638 bytes (34,618 bytes in total). They are small examples, **not** a sufficient sample from which to derive a limit. Measurements should also cover real consumer documents, large but valid cases, peak memory and processing time for every resource surface, differences between embedded and external Expat, and the opt-in libxml2 path. The four examples must not be extrapolated into global values.

The minimum regression matrix for the implementation includes: exactly-at-limit and limit-plus-one cases; overflow in the second or final chunk; attributes of different lengths on the same element; exactly permitted depth; fragmented and coalesced CDATA; repeated internal and external DTD expansion; a failed parse followed by a valid parse and preservation of the previous DOM; filters that discard nodes; and application-built DOM output near the size and indentation budget. Repeat the matrix in normal and ASan/UBSan builds and on the fixed integration lab and target master before proposing the DTD work.
