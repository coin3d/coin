# Index of historical reports in `docs/bugs/`

This index preserves the 42 reports in the secondary catalog and their historical identifiers. The original reports are unchanged. The file [`docs/BUGS.txt`](../BUGS.txt) has a [separate triage](bugs.md).

The November 21, 2026 review of 259 closed `coin3d/coin` issues found one direct match in this catalog: entry 148 and issue #81. The remaining entries need individual technical triage. A dash in the confirmed link column leaves the status open for investigation, including possible matches in other records.

| Category | ID | Original report | Confirmed link |
| --- | --- | --- | --- |
| Configuration and build | 044 | [Release builds on Cygwin/MSVC++ doesn't heed all configure options.](../bugs/configuration_and_build/20021118--cygwin-and-msvc-builds-does-not-heed-all-configure-options.txt) | — |
| Configuration and build | 048 | [Building SGI InventorXt on top of Coin reveals compile-time incompabilities.](../bugs/configuration_and_build/20021218--sgi-inventorxt-does-not-build-with-Coin.txt) | — |
| Configuration and build | 052 | [MSWin dependency walker gives surprising results.](../bugs/configuration_and_build/20030122--msvcrt-multiple-dependencies.txt) | — |
| Configuration and build | 067 | [Possible mis-compilation with Intel Compiler.](../bugs/configuration_and_build/20030514--possible-miscompilation-with-intel-compiler.txt) | — |
| Configuration and build | 082 | [Configure/build problem: we need to demand X11 when using GLX.](../bugs/configuration_and_build/20031028--demand-X11-when-using-GLX.txt) | — |
| Configuration and build | 120 | [@exec_prefix@ expansion in Coin/coin.cfg.in makes installed package not movable.](../bugs/configuration_and_build/20040705--exec_prefix-expansion-in-coin.cfg-makes-pkg-not-movable.txt) | [Open PR #930](https://github.com/coin3d/coin/pull/930) revisits this topic for the current configuration. |
| Configuration and build | 123 | [coin-config --setup uses hardcoded LD_LIBRARY_PATH variable name and is incompatible with e.g. tcsh](../bugs/configuration_and_build/20040707--coin-config-uses-LD_LIBRARY_PATH-which-fails-with-tcsh.txt) | — |
| Configuration and build | 177 | [Libtool is buggy on 64-bit Linux.](../bugs/configuration_and_build/20041208--libtool-buggy-on-64-bit-linux.txt) | — |
| Configuration and build | 193 | [superglu should be sync'ed against SGI GLU.](../bugs/configuration_and_build/20050602--sync-with-sgi-glu.txt) | — |
| Configuration and build | 198 | [MS Windows DLL versioning.](../bugs/configuration_and_build/20050624--dll-versioning.txt) | — |
| Documentation | 119 | [Documentation bug: SbBool is not part of the Doxygen-generated doc.](../bugs/documentation/20040625--sbool-is-not-in-coin-docs.txt) | [Fixed in the source and verified locally](doxygen.md#item-119-sbbool). `SbBool` appears in the `coin_base` group of the classic HTML generated with Doxygen 1.9.8. Check the published documentation after merge. |
| Other features | 103 | [Need more 'ivinfo' information](../bugs/features/others/20040218--more-information-from-ivinfo-hack-desirable.txt) | — |
| Other features | 124 | [Manipulators and Spacemouse support](../bugs/features/others/20040722--spacemouse-support-for-draggers-and-manips.txt) | — |
| TGS Inventor compatibility | 146 | [Compatibility issue: SoShapeHints::windingType missing in Coin.](../bugs/features/tgs-inventor/20041010--SoShapeHints_windingType-missing-in-Coin.txt) | — |
| TGS Inventor compatibility | 148 | [Compatibility issue: SoDrawStyle::linePatternScaleFactor missing in Coin.](../bugs/features/tgs-inventor/20041010--SoDrawStyle_linePatternScaleFactor.txt) | [Closed issue #81](https://github.com/coin3d/coin/issues/81). The requested field is present in the code. |
| TGS Inventor compatibility | 149 | [Compatibility issue: SoVRMLSound fields dopplerFactor and dopplerVelocity missing in Coin.](../bugs/features/tgs-inventor/20041010--SoVRMLSound_dopplerFactor-and-dopplerVelocity.txt) | — |
| TGS Inventor compatibility | 150 | [Compatibility issue: SoVRMLMovieTexture fields alpha, model, blendColor and scaleTexCoord missing in Coin.](../bugs/features/tgs-inventor/20041010--missing-fields-in-SoVRMLMovieTexture.txt) | — |
| TGS Inventor compatibility | 169 | [SoDataSensor extension](../bugs/features/tgs-inventor/20041116--SoDataSensor-extension.txt) | — |
| TGS Inventor compatibility | 170 | [SoIndexedFaceSet defines](../bugs/features/tgs-inventor/20041116--SoIndexedFaceSet-defines.txt) | — |
| TGS Inventor compatibility | 172 | [SoFieldContainer and private fields](../bugs/features/tgs-inventor/20041116--SoFieldContainer-and-private-fields.txt) | — |
| TGS Inventor compatibility | 174 | [SoTrackerEvent missing](../bugs/features/tgs-inventor/20041116--SoTrackerEvent-missing.txt) | — |
| TGS Inventor compatibility | 178 | [SbMatrixd and SbRotationd missing from Coin.](../bugs/features/tgs-inventor/20041210--sbmatrixd-and-sbrotationd-missing.txt) | — |
| TGS Inventor compatibility | 197 | [Compatibility issue: SoDB::threadInit() not implemented in Coin.](../bugs/features/tgs-inventor/20050620--sodb_threadinit-missing-in-Coin.txt) | — |
| Low priority: other | 036 | [This program will hit an assert if we interact with the SoTrackballDragger at the same time as it is remove…](../bugs/low_priority/insignificant/20021011--crash_when_deleting_active_dragger.txt) | — |
| Low priority: other | 041 | [Circular field connections not always handled.](../bugs/low_priority/insignificant/20021016--circular-field-connections.txt) | — |
| Low priority: input and output | 025 | [Import hangs on invalid scenegraph. This reproduces the problem:](../bugs/low_priority/io/20020911--unhelpful_error_message_on_invalid_file.txt) | — |
| Low priority: input and output | 104 | [Unhelpful read error message.](../bugs/low_priority/io/20040220--unhelpful-read-error-message.txt) | — |
| Low priority: input and output | 165 | [Faulty error reporting upon reading bad file.](../bugs/low_priority/io/20041105--incorrect_error_message_on_invalid_file.txt) | — |
| Low priority: older systems | 200 | ["Fake" NVidia stereo reported to not work with Coin / SoWin/SoQt.](../bugs/low_priority/obscure_systems/20050627--nividia-stereo-hack-not-working.txt) | — |
| Low priority: older systems | 201 | [Coin 2.4.1 + SoQt 1.2.0 + Qt 3.3.4 hangs on Win98.](../bugs/low_priority/obscure_systems/20050627--coin-plus-soqt-hangs-on-win98.txt) | — |
| Low priority: robustness | 026 | [Bounding box calculation: the bbox for the Barcelona.iv model is _very_ sub-optimal. (SGI Inventor seems to…](../bugs/low_priority/robustness/20020915--suboptimal_bbox_on_buggy_model.txt) | — |
| Low priority: robustness | 176 | [SbTime::formatDate() will crash on bad (?) input.](../bugs/low_priority/robustness/20041207--sbtime_formatdate-not-robust-on-bad-input.txt) | — |
| Low priority: robustness | 184 | [SbOctTree::findItems() functions not multi-CRT safe on Windows.](../bugs/low_priority/robustness/20041221--multi-crt-robustness.txt) | — |
| Obsolete | 003 | [There are seams in texturemaps on SGI Octane with "IMPACTSR" graphics hardware.](../bugs/obsolete/20020205--textures_on_sgi_octane.txt) | — |
| Obsolete | 033 | [Some of the default geometry files for dragger geometries uses an SoDrawStyle::lineWidth > 1.0. This can be…](../bugs/obsolete/20020930--dragger_geometry_files_may_have_too_large_lineWidth.txt) | — |
| Performance | 115 | [SoBaseKit::setPart() is inefficient.](../bugs/performance_enhancements/20040607--SoBaseKit_setPart-is-inefficient.txt) | — |
| Performance | 118 | [Weakness in internal handling of multiple implicit references to the same texture.](../bugs/performance_enhancements/20040624--medium-pri-issue--multiple-implicit-refs-to-same-texture-file.txt) | — |
| Performance | 139 | [SoText2 nodes never culled?](../bugs/performance_enhancements/20040903--SoText2-nodes-never-culled.txt) | — |
| Performance | 173 | [SbIntList implementation](../bugs/performance_enhancements/20041116--SbIntList-inefficient.txt) | — |
| Performance | 202 | [SoLocateHighlight implementation is inefficient.](../bugs/performance_enhancements/20050630--SoLocateHighlight-implementation-is-inefficient.txt) | — |
| Performance | — | [libCgGL loading triggered at SoDB::init().](../bugs/performance_enhancements/20060208--libCgGL-not-lazy-loaded.txt) | — |
| Resource leaks | 001 | [Memory leak with SoSelection.](../bugs/resource_leaks/BUGS.txt) | The October 5, 2026 local audit reproduced the retention. |

## Follow-up

- **148:** Issue #81 already records and closes this request. Avoid opening a duplicate.
- **120:** Review the outcome of PR #930 before deciding whether to annotate the historical relocation report.
- **001:** The audit reproduced retention of `SoSelection`. Prepare a current report after checking for open duplicates and narrowing down the viewer integration.
- **Remaining files:** Keep the historical report as the source, check the current environment and reproduction, then link an existing issue or prepare a dedicated report.
