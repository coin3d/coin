# Historical documents in `docs/`

This index was prepared on 21 November 2026. It covers the historical text
files at the root of `docs/` and keeps the original wording available in
the source files. The same numbers can occur in both `todo.txt` and
`BUGS.txt`. These are historical identifiers. This index assigns ordinals to
entries that had no original number.

## Sources and follow-up

| File | Contents | Follow-up |
| --- | --- | --- |
| [`BUGS.txt`](../BUGS.txt) | 45 reports | See [Discussion #935](https://github.com/orgs/coin3d/discussions/935) and the [technical triage](bugs.md). |
| [`bugs/`](bugs-secondary.md) | 42 report files | Check for duplicates and reproduce the behavior before opening issues. |
| [`todo.txt`](../todo.txt) | 44 numbered entries | Review current status and duplicates before publishing individual items. |
| [`api-wish-list.txt`](../api-wish-list.txt) | 45 ideas | Discuss API scope first. Open issues once the specification is agreed. |
| [`projects.txt`](../projects.txt) | 12 proposals | Reassess proposals, especially those tied to older platforms, before starting discussions. |
| [`plan.txt`](../plan.txt) | 1999 milestones | Keep as historical context. The file identifies many items as completed or obsolete. |
| [`optimization.txt`](../optimization.txt) | Guidelines and tasks | Separate recommendations from measurable work. See item 118 in the secondary bug catalog. |
| [`oiv_compatibility.txt`](../oiv_compatibility.txt) | Open Inventor compatibility | Historical reference. The numbers refer to Bugzilla. |
| [Doxygen](doxygen.md) | Generated API documentation | The [documentation map](doxygen.md) covers `src/doc/` sources, code comments, `docs/` configuration, and GitHub Pages publishing. |
| [`c-porting.txt`](../c-porting.txt), [`quat2euler.txt`](../quat2euler.txt) | Technical notes | Keep as documentation and validate any examples before republishing. |
| [`foreign-language-bindings.txt`](../foreign-language-bindings.txt), [`misc-resources.txt`](../misc-resources.txt) | Links and references | Check links and relevance before updating the documentation. |
| [`oiki-launch.txt`](../oiki-launch.txt) | Launch announcement | Keep as a historical record. |
| `announcement-*.txt` | 19 release announcements | Keep as release history. |

## `todo.txt`: 44 entries

The notes in the original file record its status at the time. Check each entry
against current behavior, issues, and PRs before publishing it.

| Historical ID | Topic | Triage note |
| --- | --- | --- |
| TODO-000 | [Improve error reporting on texture loading](../todo.txt#L16) | The file marks the error-message subtask as fixed. The broader request remains open for review. |
| TODO-001 | [A "stopwatch" for built-in profiling support](../todo.txt#L51) | Needs verification |
| TODO-002 | [Heed the SoBoundingBoxCache::hasLinesOrPoints() settings](../todo.txt#L102) | Needs verification |
| TODO-003 | [Clean up the file versioning mess](../todo.txt#L117) | Needs verification |
| TODO-004 | [Re-design the public API for handling textures](../todo.txt#L252) | Needs verification |
| TODO-005 | [Support compressed textures](../todo.txt#L295) | Basic support exists. [SoTexture2](../../src/nodes/SoTexture2.cpp#L553) forwards the compression preference, and the [OpenGL glue](../../src/glue/gl.cpp#L5040) selects compressed formats. Format and driver coverage require separate verification. |
| TODO-006 | [Font specification](../todo.txt#L318) | Needs verification |
| TODO-010 | [Make Coin Usable from ROM](../todo.txt#L332) | Needs verification |
| TODO-011 | [Rethink handling of camera properties and display configuration.](../todo.txt#L340) | Needs verification |
| TODO-012 | [The SoSphere should be tessellated better.](../todo.txt#L357) | Needs verification |
| TODO-013 | [Implement an alternative to using macros for specifying nodekit parts.](../todo.txt#L365) | Needs verification |
| TODO-014 | [Make rendering (and other operations) robust for inconsistent scenegraphs.](../todo.txt#L376) | Needs verification |
| TODO-015 | [100% correct transparency mode.](../todo.txt#L389) | Needs verification |
| TODO-016 | [Detect multiple instances of the Coin library within the same run-time application space.](../todo.txt#L410) | An older update records this as implemented. The code contains `CoinStaticObjectInDLL`. Test detection on the current build. |
| TODO-017 | [Move action-style code developed in other projects into proper actions.](../todo.txt#L432) | The code contains `SoToVRMLAction`, `SoToVRML2Action`, and `SoReorganizeAction`. Check what work is still needed. |
| TODO-018 | [Check out and build simage as part of the Coin build process.](../todo.txt#L452) | Needs verification |
| TODO-019 | [Dynamically configurable viewer interaction (SoQt++)](../todo.txt#L516) | Needs verification |
| TODO-020 | [SoSub*.h improvements.](../todo.txt#L531) | Needs verification |
| TODO-021 | [Call-stack backtraces on errors.](../todo.txt#L544) | Needs verification |
| TODO-022 | [VRML97 VRMLScript scripting node.](../todo.txt#L557) | Same topic as BUGS-214 (VRMLScript/SpiderMonkey). |
| TODO-023 | [SoCollisionManager & So@Gui@CollisionViewer.](../todo.txt#L570) | Needs verification |
| TODO-024 | [Optimizing the performance of the SoIntersectionDetectionAction.](../todo.txt#L579) | Needs verification |
| TODO-025 | [Integrate QGL into SoQt.](../todo.txt#L605) | Needs verification |
| TODO-026 | [WWW-interface on Coin / SoWin / SoQt builds.](../todo.txt#L706) | Needs verification |
| TODO-027 | [Build Coin / SoQt / SoWin on MSWin without relying on MSVC++.](../todo.txt#L736) | Needs verification |
| TODO-028 | [Support for multiple viewports in render areas](../todo.txt#L807) | Needs verification |
| TODO-029 | [Better font support.](../todo.txt#L840) | Needs verification |
| TODO-030 | [Offscreen context support functions.](../todo.txt#L915) | The [context helpers](../../include/Inventor/C/glue/gl.h#L624) provide creation and activation, consistent with the original FIXED note. Track the separate request for context configuration under [BUGS-102](bugs.md), linking to the [original report](../BUGS.txt#L1165). |
| TODO-031 | [Handle POSIX paths under MSWin.](../todo.txt#L920) | Needs verification |
| TODO-032 | [Port SGI's ''ivperf'' tool.](../todo.txt#L935) | Needs verification |
| TODO-033 | [Port TGS's ''ivtovrml'' tool.](../todo.txt#L943) | Needs verification |
| TODO-034 | [Image loading under MSWin through DirectX.](../todo.txt#L968) | Needs verification |
| TODO-035 | [Clean up SoBase write code.](../todo.txt#L989) | Related to the scene graph writing in BUGS-072, with broader scope. |
| TODO-036 | [Write an "interior-geometry reducer" action.](../todo.txt#L1004) | Needs verification |
| TODO-037 | [Take advantage of NVidia's occlusion culling extension.](../todo.txt#L1027) | Needs verification |
| TODO-038 | [Duplicate API for TGS's SoBigImageRenderer extension.](../todo.txt#L1039) | Needs verification |
| TODO-039 | [Move tristripifying code into SoReorganizeAction.](../todo.txt#L1048) | Needs verification |
| TODO-040 | [Add built-in version numbers to win32 DLLs.](../todo.txt#L1055) | Implemented in the Windows shared library build. [CMake](../../src/CMakeLists.txt#L243) adds the generated [version resource](../../src/Coin.rc.cmake.in#L3) with major, minor and patch values. Verified in source. |
| TODO-041 | [Generic ADT in C for dynamic arrays.](../todo.txt#L1119) | Needs verification |
| TODO-042 | [Coin internationalization.](../todo.txt#L1131) | Needs verification |
| TODO-043 | [The Coin library should have an option to set up a fatal error handler.](../todo.txt#L1169) | Needs verification |
| TODO-044 | [Use GL_TEXTURE_RECTANGLE_EXT extension.](../todo.txt#L1186) | Partly covered. [SoGLImage](../../src/rendering/SoGLImage.cpp#L1749) uses the ARB rectangle texture target. The original request also covers pbuffer creation and binding, which this code does not establish. |
| TODO-045 | [Coin / SoQt / Qt port or reimplementation of the SGI ''gview'' program.](../todo.txt#L1202) | Needs verification |
| TODO-046 | [FILEFORMAT/DEFAULTS sections for the nodes.](../todo.txt#L1221) | Needs verification |

## `api-wish-list.txt`: 45 ideas

The file ends with a section titled “IMPLEMENTED, RESIDING IN SmallChange.” Verify separately whether the current Coin provides each feature.

| Catalog ID | Idea | Triage note |
| --- | --- | --- |
| API-01 | [missing base class fields](../api-wish-list.txt#L19) | Needs verification |
| API-02 | [SbVec2i](../api-wish-list.txt#L31) | Needs verification |
| API-03 | [SoGLPrimitiveSet node.](../api-wish-list.txt#L49) | Needs verification |
| API-04 | [More nodes and elements to control GL state.](../api-wish-list.txt#L115) | Needs verification |
| API-05 | [Add SoMFMatrix input fields in SoCalculator](../api-wish-list.txt#L124) | Needs verification |
| API-06 | [A better hash/dict class than SbDict.](../api-wish-list.txt#L135) | Needs verification |
| API-07 | [SoGestureEvent](../api-wish-list.txt#L165) | Needs verification |
| API-08 | [SoText2::verticalAlignment / verticalJustification](../api-wish-list.txt#L175) | Needs verification |
| API-09 | [3D GUI Components](../api-wish-list.txt#L185) | Needs verification |
| API-10 | [Less environment pollution](../api-wish-list.txt#L223) | Needs verification |
| API-11 | [SbBool SbMatrix::inversePossible(void) const](../api-wish-list.txt#L276) | Needs verification |
| API-12 | [SbList::setEqTester()](../api-wish-list.txt#L289) | Needs verification |
| API-13 | [SoNode::setOverride() should be made persistent](../api-wish-list.txt#L311) | Needs verification |
| API-14 | [SoDB::registerImporter()](../api-wish-list.txt#L330) | Needs verification |
| API-15 | [New node TextureAttribute](../api-wish-list.txt#L347) | Needs verification |
| API-16 | [void SbRotation::getAsEuler(SbVec3f &), void SbRotation::setAsEuler(const SbVec3f &)](../api-wish-list.txt#L375) | Needs verification |
| API-17 | [SoFocusEvent](../api-wish-list.txt#L383) | Needs verification |
| API-18 | [SoMarkerSet scaling](../api-wish-list.txt#L392) | Needs verification |
| API-19 | [SbReal typedef](../api-wish-list.txt#L400) | Needs verification |
| API-20 | [The SbBool typedef](../api-wish-list.txt#L417) | Requires an ABI decision. The [public C header template](../../include/Inventor/C/basic.h.cmake.in#L55) defines `SbBool` as `int`. The wish proposes changing the type to `bool`; the historical [Doxygen report](doxygen.md#item-119-sbbool) concerns documenting the existing type. |
| API-21 | [Make APIs for engine classes more consistent.](../api-wish-list.txt#L434) | Needs verification |
| API-22 | [SoInput::setStrings(char **)](../api-wish-list.txt#L445) | Needs verification |
| API-23 | [SoInput/SoOutput alternative](../api-wish-list.txt#L459) | Needs verification |
| API-24 | [SoAction::apply(SoAction * beingApplied)](../api-wish-list.txt#L476) | Needs verification |
| API-25 | [SbFile](../api-wish-list.txt#L492) | Needs verification |
| API-26 | [SoField::setValidator(SoValidator *)](../api-wish-list.txt#L535) | Needs verification |
| API-27 | [SoKeyboardEvent::Key enums](../api-wish-list.txt#L545) | Needs verification |
| API-28 | [SoSwitch::whichChild as an SoMFBool](../api-wish-list.txt#L635) | Needs verification |
| API-29 | [SoMouseButtonEvent::isDoubleClick()](../api-wish-list.txt#L661) | Needs verification |
| API-30 | [SbBigNum type](../api-wish-list.txt#L680) | Needs verification |
| API-31 | [SoOffscreenRenderer::render(SoPathList &)](../api-wish-list.txt#L687) | Needs verification |
| API-32 | [Support for field value expressions in Inventor ascii files](../api-wish-list.txt#L699) | Needs verification |
| API-33 | [SoGLTeaPot](../api-wish-list.txt#L713) | Needs verification |
| API-34 | [SoSearchPathAction (which is part of the TGS Inventor API)](../api-wish-list.txt#L725) | Needs verification |
| API-35 | [Rewrite non-member/friend operator methods for the base classes.](../api-wish-list.txt#L733) | Needs verification |
| API-36 | [Detect and support libtar if present, and update SoInput accordingly.](../api-wish-list.txt#L762) | Needs verification |
| API-37 | [Prettier SoWriteAction .iv-formatting](../api-wish-list.txt#L794) | Needs verification |
| API-38 | [Output .iv-files with CR/LF on Win32 systems](../api-wish-list.txt#L817) | Needs verification |
| API-39 | [SoViewportEvent](../api-wish-list.txt#L829) | Needs verification |
| API-40 | [Extending SoInput to support callbacks for file operations](../api-wish-list.txt#L841) | Needs verification |
| API-41 | [SoDepthBuffer, SoGLDepthBufferElement](../api-wish-list.txt#L885) | Implemented in Coin. Both [SoDepthBuffer](../../include/Inventor/nodes/SoDepthBuffer.h#L42) and [SoGLDepthBufferElement](../../include/Inventor/elements/SoGLDepthBufferElement.h#L38) are present. See [PR #856](https://github.com/coin3d/coin/pull/856) and [PR #886](https://github.com/coin3d/coin/pull/886) for specific behavior changes. |
| API-42 | [SoViewportRegion](../api-wish-list.txt#L897) | The file records an implementation in SmallChange. This Coin checkout has no SoViewportRegion. |
| API-43 | [Coinboard node](../api-wish-list.txt#L911) | The file notes a partial implementation in SmallChange. |
| API-44 | [SoEnvironment::fogStart field.](../api-wish-list.txt#L936) | Partly covered. [SoEnvironmentElement](../../include/Inventor/elements/SoEnvironmentElement.h#L63) accepts `fogStart`, but [SoEnvironment](../../include/Inventor/nodes/SoEnvironment.h#L65) does not expose that field. The remaining proposal is the public field and its scene-file behavior. |
| API-45 | [SoSkyDome node.](../api-wish-list.txt#L946) | The file notes a partial implementation in SmallChange. |

## `projects.txt`: 12 proposals

These entries proposed work for students or interns. Some depend on older platforms and tools.

| Catalog ID | Proposal | Triage note |
| --- | --- | --- |
| PROJECT-01 | [A QNX port (QNX for x86 platforms is supposedly available for free](../projects.txt#L4) | Reassess scope and relevance |
| PROJECT-02 | [A "gl-attack" application stressing many different aspects of](../projects.txt#L7) | Reassess scope and relevance |
| PROJECT-03 | [Work with the documentation to improve it (add example code](../projects.txt#L10) | Reassess scope and relevance |
| PROJECT-04 | [Write many more examples, covering the functionality offered by](../projects.txt#L13) | Reassess scope and relevance |
| PROJECT-05 | [Write a tutorial.](../projects.txt#L16) | Reassess scope and relevance |
| PROJECT-06 | [Write a NURBS-library covering all the necessary functionality we](../projects.txt#L18) | Reassess scope and relevance |
| PROJECT-07 | [An IVF (MFC-based) GUI-glue library.](../projects.txt#L24) | Reassess scope and relevance |
| PROJECT-08 | [Make simple routines for packaging up Coin for RedHat Linux (.rpm),](../projects.txt#L26) | Reassess scope and relevance |
| PROJECT-09 | [Do the work needed for being compatible with 64-bit systems. These](../projects.txt#L31) | Reassess scope and relevance |
| PROJECT-10 | [Profiling and optimalization of rendering and other action](../projects.txt#L44) | Reassess scope and relevance |
| PROJECT-11 | [A regression test system for the classes under Coin/src/base.](../projects.txt#L47) | Implemented as test infrastructure. [CoinTests](../../testsuite/CMakeLists.txt#L114) is registered with [CTest](../../testsuite/CMakeLists.txt#L163), and base classes such as [SbImage](../../src/base/SbImage.cpp#L727) have regression tests. Track specific coverage gaps separately. |
| PROJECT-12 | [Profiling and optimalization of the base classes. Should depend on a](../projects.txt#L49) | Reassess scope and relevance |

## Related entries

- TODO-022 and BUGS-214 concern VRMLScript. Link both in any follow-up.
- TODO-035 and BUGS-072 concern scene graph writing. The TODO also covers broader thread safety.
- API-06 proposes replacing or improving `SbDict`. Compare with work already done on `SbHash` and `SbDict` before opening a new proposal.
- The shared-texture note in `optimization.txt` points to historical item 118 in [`bugs/performance_enhancements/`](../bugs/performance_enhancements/20040624--medium-pri-issue--multiple-implicit-refs-to-same-texture-file.txt). Use that source when referring to the item.

## Next steps

1. Check entries explicitly marked “fixed” or “implemented” against the current code.
2. Match entries in `todo.txt`, `api-wish-list.txt`, and `projects.txt` to open and closed issues and PRs by symptom. Use historical identifiers for the catalog entries.
3. Start a discussion for each list or broad topic after triage. Open focused issues when there is a current reproduction or specification.
