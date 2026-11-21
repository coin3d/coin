# Triage of `BUGS.txt` for issues and discussions

This catalog was prepared locally on November 21, 2026. It organizes the 45
entries in [`docs/BUGS.txt`](../BUGS.txt) and preserves the historical reports.
The local audit of October 5, 2026 supplied the technical evidence for the
statuses below, with later GitHub outcomes noted where verified.
**This triage did not rerun all 45 cases.** An entry in the historical file
does not establish that the defect persists, and a single unsuccessful
attempt to reproduce it does not establish that it was fixed.

The catalog of 45 entries was published in [coin3d Discussion #935](https://github.com/orgs/coin3d/discussions/935).
The discussion links to lines in `BUGS.txt` on `coin3d/coin:master` and
distinguishes cases with an existing issue or PR from candidates for new reports.

## How to read the triage

- **Confirmed**: the audit reproduced the symptom or identified a limitation
  that persists. There are 12 entries.
- **Fixed upstream**: the matching PR was merged with validation of the historical
  symptom. There is 1 entry.
- **Partial**: the audit examined related code, mitigation, or behavior.
  The full report still needs a verdict. There are 12 entries.
- **Not reproduced**: the test did not show the symptom, and its coverage
  was insufficient to close the entry. There are 2 entries.
- **Pending**: reproduction or a decision about the expected behavior is
  needed. There are 18 entries.

In the **Destination** column, `Issue` means a candidate technical report
after checking for duplicates. `Discussion` means a decision about API,
policy, or scope. `Verify` means gathering evidence before publishing.
`Existing` means continuing in the existing issue and PR. `Archive` means the
resolution is recorded and the entry requires no new report. The numbers from
`BUGS.txt` are **historical identifiers**, distinct from GitHub issue numbers.

## Full inventory

| ID | Subject | Status | Destination | Next evidence or decision |
| --- | --- | --- | --- | --- |
| [005](../BUGS.txt#L33) | Connected `SoSFEnum`: possible read after free | Pending | Verify | Run a reproducer with ASan or Valgrind. |
| [031](../BUGS.txt#L44) | Alpha transparency in offscreen rendering | Partial | Verify | Compare pixels and context selection on current backends and drivers. See 102. |
| [045](../BUGS.txt#L93) | `SoCenterballManip` rotates off center | Pending | Verify | Run the scene and repeat the gesture from the report. |
| [047](../BUGS.txt#L132) | `SoAntiSquish` and matrix factorization in draggers | Pending | Verify | Reproduce rotation and scaling in the original example. |
| [051](../BUGS.txt#L169) | `SoImage` and `viewAll` before showing the viewer | Pending | Verify | Test SoQt integration with the initial viewport. |
| [054](../BUGS.txt#L229) | `SoComputeBoundingBox` with a changing viewport | Partial | Verify | Reproduce the engine update with the external camera and viewer. |
| [060](../BUGS.txt#L252) | `SoToVRML2Action` loses VRML97 geometry | Not reproduced | Verify | Extend coverage to mixed graphs and check the fields. |
| [066](../BUGS.txt#L303) | `viewAll` with a narrow viewport and large coordinates | Pending | Verify | Test the five-segment scene, a 2:1 window, and rotations. |
| [072](../BUGS.txt#L374) | Extension nodekit cannot be read back after a binary write | Confirmed | Issue | Use the audit's `SoTestKit` reproducer. Reading the buffer back fails. [Closed issue #93](https://github.com/coin3d/coin/issues/93) covers a separate `writeref` case. |
| [073](../BUGS.txt#L466) | Using one `SoTexture2` at different qualities | Partial | Verify | Compare filtering and pixels for two uses of the same texture. |
| [074](../BUGS.txt#L543) | `SoComplexity` after `SoTexture2` | Pending | Discussion | Decide how node order should affect the result, then test the exact scene. |
| [075](../BUGS.txt#L576) | `SoHandleBoxManip` jumps when Shift is released | Pending | Verify | Reproduce the gesture and movement. |
| [076](../BUGS.txt#L596) | `SoHandleBoxManip` jumps during Z translation | Pending | Verify | Test the projection and camera from the report. |
| [090](../BUGS.txt#L625) | `copy(TRUE)` omits output connections | Confirmed | Discussion | The difference was reproduced in Coin. Confirm the SGI contract before proposing a change. |
| [092](../BUGS.txt#L694) | `SoToVRMLAction` ignores Inventor/Coin nodes | Confirmed | Discussion | The limitation was reproduced and documented. Decide whether to extend the conversion. |
| [095](../BUGS.txt#L704) | `SoIndexedTriangleStripSet` is slow with `PER_FACE` normals | Pending | Verify | Run a current benchmark with equivalent bindings. |
| [099](../BUGS.txt#L1114) | Frustum culling depends on bounding boxes | Partial | Discussion | `COIN_GLBBOX` is available. Decide on the desired default or documentation. |
| [102](../BUGS.txt#L1165) | Missing API to configure the offscreen context | Confirmed | Discussion | Define public requirements and backends. The stencil workaround covers only part of the requested functionality. |
| [108](../BUGS.txt#L1206) | Asynchronous textures are absent from the first frame | Pending | Verify | The main symptom was cataloged in [closed issue #182](https://github.com/coin3d/coin/issues/182), which was closed with an option to disable delayed loading. Test whether it still occurs without that setting and examine the fractured texture observation separately. |
| [114](../BUGS.txt#L1278) | Import changes a valid VRML identifier | Confirmed | Issue | The minimal case `DEF @foo Group {}` produces `_@foo`. |
| [121](../BUGS.txt#L1298) | Tiles fail when the subviewport changes | Confirmed | Discussion | The architecture is still undecided. Reproduce the overlay and define the viewport contract. See 191. |
| [129](../BUGS.txt#L1331) | Group of scaling, picking, and dragger bugs | Pending | Verify | Separate the individual scenarios and test each gesture. Some protections are already in place. |
| [133](../BUGS.txt#L1460) | `SoExtSelection` lasso appears in multiple views | Pending | Verify | Test a node shared across two views. |
| [135](../BUGS.txt#L1492) | `doAction()` elements in new actions | Partial | Discussion | Run a custom action and define the extension contract. |
| [140](../BUGS.txt#L1532) | Possible leak when copying `SoVRMLParent` | Partial | Verify | There is a legitimate explanation for the reference count in isolation. Test destruction under ASan. |
| [141](../BUGS.txt#L1589) | Stacking offscreen contexts | Partial | Verify | Test nested and mixed GLX contexts. [PR #924](https://github.com/coin3d/coin/pull/924) covers EGL state restoration, one part of the report. |
| [143](../BUGS.txt#L1667) | Filtering warnings from a defective scene | Partial | Discussion | Define warning categories and the desired API. |
| [157](../BUGS.txt#L1694) | Font styles on Windows | Partial | Verify | Test `:Bold` and current formats on Windows. [Closed issue #78](https://github.com/coin3d/coin/issues/78) fixed a distinct bug in font name selection by converting between UTF-8 and UTF-16. |
| [162](../BUGS.txt#L1722) | Search misses VRML `SFNode`/`MFNode` fields | Confirmed | Issue | `SoSearchAction` does not find `FontStyle`. Combine with 196. |
| [168](../BUGS.txt#L1742) | Possible use-after-free in VRML nodes | Pending | Verify | The report stopped reproducing in 2005. The original scene or an equivalent is still needed for an ASan run. |
| [191](../BUGS.txt#L1852) | Orthographic camera with `CROP` and tiles | Confirmed | Issue | The code explicitly documents a limitation. Compare tiled and untiled images. See 121. |
| [194](../BUGS.txt#L1929) | Assertion in tile mode with a VRML97 graph | Pending | Verify | Run the case in Debug. The callback path has changed. |
| [196](../BUGS.txt#L1954) | Search inside a VRML97 `PROTO` | Confirmed | Issue | The example fails to find `FontStyle`. Combine with 162. |
| [203](../BUGS.txt#L2072) | OpenAL DLL calling convention | Pending | Verify | Requires the affected DLL and a Windows test. |
| [205](../BUGS.txt#L2096) | Hardcopy with viewport changes | Pending | Verify | Compare the PostScript output from the original scene with four views. |
| [208](../BUGS.txt#L2112) | Excessive default tessellation for `SoText3` | Pending | Discussion | Measure cost and visual quality. [PR #920](https://github.com/coin3d/coin/pull/920) bounds out-of-range complexity values. The default tessellation question remains open. |
| [209](../BUGS.txt#L2126) | `SoTranslate1Dragger` translates its own geometry | Pending | Verify | Reproduce the scene connected to `RotationXYZ` and compare with SGI. |
| [210](../BUGS.txt#L2168) | `SoText2` size in pixels or points | Partial | Discussion | The current policy uses pixels. Decide on compatibility and the expected DPI behavior. |
| [211](../BUGS.txt#L2184) | Primitives ignore texture coordinate functions | Partial | Verify | Compare pixels for Sphere/Cube/Cone/Cylinder. [Closed issue #75](https://github.com/coin3d/coin/issues/75) fixed a separate case involving successive `TextureCoordinatePlane` callbacks. [PR #873](https://github.com/coin3d/coin/pull/873) and [#906–908](https://github.com/coin3d/coin/pull/906) cover related behavior. The rendering symptom still needs verification. |
| [213](../BUGS.txt#L2196) | `SbTesselator` fails on a 15-vertex polygon | Not reproduced | Verify | The area matched. Topology, overlap, and edges still need verification. |
| [214](../BUGS.txt#L2251) | `VRMLScript` and current SpiderMonkey | Confirmed | Discussion | Legacy JavaScript support remains unavailable. Decide on a backend and scope. |
| [215](../BUGS.txt#L2273) | `SoText2` under `SoClipPlane` | Partial | Discussion | Reproduce visually and decide the clipping semantics. |
| [217](../BUGS.txt#L2314) | `DrawStyle::POINTS` becomes lines during VRML2 conversion | Confirmed | Issue | `LineSet` and `IndexedLineSet` produce `IndexedLineSet` without a `PointSet`. |
| [219](../BUGS.txt#L2326) | `viewAll` fails to frame `SoText2` | Fixed upstream | Archive | [Issue #417](https://github.com/coin3d/coin/issues/417) is closed. [PR #774](https://github.com/coin3d/coin/pull/774) was merged and documents the original SoXt example rendering after one `viewAll()` call. |
| [221](../BUGS.txt#L2357) | Global field is not evaluated on file read | Confirmed | Issue | In the example, `string` updates only after `touch()`. |

## Cross-reference with closed issues

The review covered the titles and bodies of **259 closed issues** in
`coin3d/coin` and the comments on the candidates below. A match depends on
the symptom and reproducer. The `closed/completed` status records how GitHub
closed the issue. Symptoms in the historical file still require verification.

| Historical source | Closed issue | Relationship |
| --- | --- | --- |
| `BUGS.txt` 108 | [#182](https://github.com/coin3d/coin/issues/182) | **Same main symptom**: a VRML texture is absent from an offscreen render. The closing comment gives `SoVRMLImageTexture::setDelayFetchURL(FALSE)` as a workaround. The full synchronization proposal and the fractured texture observation remain unassessed. |
| `BUGS.txt` 072 | [#93](https://github.com/coin3d/coin/issues/93) | **Related but distinct**: reference counting when writing a graph. #93 covers groups with field connections and was closed after a PR. The audit still reproduced the binary nodekit failure from 072. |
| `BUGS.txt` 157 | [#78](https://github.com/coin3d/coin/issues/78) | **Related but distinct**: Windows font selection was fixed by converting the name between UTF-8 and UTF-16. Entry 157 asks about the `:Bold` style suffix. |
| `BUGS.txt` 211 | [#75](https://github.com/coin3d/coin/issues/75) | **Related but distinct**: successive `TextureCoordinatePlane` updates. Entry 211 addresses whether primitives use the function. |
| `BUGS.txt` 121 | [#74](https://github.com/coin3d/coin/issues/74) | **Related but distinct**: the vertical offset in untiled offscreen rendering was fixed. Entry 121 addresses tiles when the subviewport changes. |
| `BUGS.txt` 219 | [#126](https://github.com/coin3d/coin/issues/126), [#384](https://github.com/coin3d/coin/issues/384) | **Related but distinct**: `SoText2` quality and alpha testing. The direct references for framing with `viewAll` are [closed issue #417](https://github.com/coin3d/coin/issues/417) and [merged PR #774](https://github.com/coin3d/coin/pull/774). |
| `BUGS.txt` 141 | [#505](https://github.com/coin3d/coin/issues/505), [#591](https://github.com/coin3d/coin/issues/591) | **Related but distinct**: GLX context failures on RHEL/SLES and WGL failures on Windows. Entry 141 concerns nested and mixed contexts. |
| `docs/bugs/` 148 | [#81](https://github.com/coin3d/coin/issues/81) | **Same request, closed**: `SoDrawStyle::linePatternScaleFactor`. The current code contains the field and its initialization. The historical request has already been addressed. |

The remaining `BUGS.txt` entries had no direct match among the 259 closed
issues in the initial review. Open issues, discussions, PRs, and records in other repositories
may still cover them. The secondary files under `docs/bugs/` have a
[separate index](bugs-secondary.md).

## Suggested order for preparing reports

1. **Prepare issues with a reproducer:** 072, 114, 162 and 196 together, 217,
   and 221. Item 191 needs a visual comparison to isolate the symptom before
   the report can be finalized.
2. **Open discussions on scope decisions:** 090, 092, 102, 121, and 214.
   Link 121 to 191 and explain the architectural limitation and the concrete case.
3. **Reproduce before publishing:** entries marked `Partial`, `Not reproduced`,
   and `Pending`, especially 211, 073, and reports that depend on Windows or
   specific drivers.

Before opening a new issue, search GitHub for duplicates by symptom and API.
Include a minimal example, observed and expected results, version and
environment, current evidence, and a reference to the historical `BUGS.txt`
ID. Split broad entries like 129 into reports for each reproducible behavior.
Start with a single report for 162 and 196 containing both related cases.
