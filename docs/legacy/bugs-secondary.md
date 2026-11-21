# Índice dos relatos históricos em `docs/bugs/`

Este índice preserva os 42 arquivos de relatos do catálogo secundário e seus identificadores históricos. Não altera os textos originais. O arquivo [`docs/BUGS.txt`](../BUGS.txt) tem [triagem própria](bugs.md).

Na consulta de 21/11/2026 às 259 issues fechadas de `coin3d/coin`, foi identificada uma correspondência direta neste catálogo: o item 148 com a issue #81. Os demais itens ainda precisam de triagem técnica individual; um travessão na coluna de vínculo não significa que o defeito esteja aberto, resolvido ou ausente de outros registros.

| Grupo | ID | Relato original | Vínculo confirmado |
| --- | --- | --- | --- |
| Configuração e build | 044 | [Release builds on Cygwin/MSVC++ doesn't heed all configure options.](../bugs/configuration_and_build/20021118--cygwin-and-msvc-builds-does-not-heed-all-configure-options.txt) | — |
| Configuração e build | 048 | [Building SGI InventorXt on top of Coin reveals compile-time incompabilities.](../bugs/configuration_and_build/20021218--sgi-inventorxt-does-not-build-with-Coin.txt) | — |
| Configuração e build | 052 | [MSWin dependency walker gives surprising results.](../bugs/configuration_and_build/20030122--msvcrt-multiple-dependencies.txt) | — |
| Configuração e build | 067 | [Possible mis-compilation with Intel Compiler.](../bugs/configuration_and_build/20030514--possible-miscompilation-with-intel-compiler.txt) | — |
| Configuração e build | 082 | [Configure/build problem: we need to demand X11 when using GLX.](../bugs/configuration_and_build/20031028--demand-X11-when-using-GLX.txt) | — |
| Configuração e build | 120 | [@exec_prefix@ expansion in Coin/coin.cfg.in makes installed package not movable.](../bugs/configuration_and_build/20040705--exec_prefix-expansion-in-coin.cfg-makes-pkg-not-movable.txt) | Tema retomado pela [PR #930 aberta](https://github.com/coin3d/coin/pull/930) na configuração atual. |
| Configuração e build | 123 | [coin-config --setup uses hardcoded LD_LIBRARY_PATH variable name and is incompatible with e.g. tcsh](../bugs/configuration_and_build/20040707--coin-config-uses-LD_LIBRARY_PATH-which-fails-with-tcsh.txt) | — |
| Configuração e build | 177 | [Libtool is buggy on 64-bit Linux.](../bugs/configuration_and_build/20041208--libtool-buggy-on-64-bit-linux.txt) | — |
| Configuração e build | 193 | [superglu should be sync'ed against SGI GLU.](../bugs/configuration_and_build/20050602--sync-with-sgi-glu.txt) | — |
| Configuração e build | 198 | [MS Windows DLL versioning.](../bugs/configuration_and_build/20050624--dll-versioning.txt) | — |
| Documentação | 119 | [Documentation bug: SbBool is not part of the Doxygen-generated doc.](../bugs/documentation/20040625--sbool-is-not-in-coin-docs.txt) | [Corrigido na fonte e validado localmente](doxygen.md#item-119-sbbool): `SbBool` aparece no grupo `coin_base` do HTML clássico gerado com Doxygen 1.9.8; conferir a publicação após merge. |
| Recursos diversos | 103 | [Need more 'ivinfo' information](../bugs/features/others/20040218--more-information-from-ivinfo-hack-desirable.txt) | — |
| Recursos diversos | 124 | [Manipulators and Spacemouse support](../bugs/features/others/20040722--spacemouse-support-for-draggers-and-manips.txt) | — |
| Compatibilidade TGS Inventor | 146 | [Compatibility issue: SoShapeHints::windingType missing in Coin.](../bugs/features/tgs-inventor/20041010--SoShapeHints_windingType-missing-in-Coin.txt) | — |
| Compatibilidade TGS Inventor | 148 | [Compatibility issue: SoDrawStyle::linePatternScaleFactor missing in Coin.](../bugs/features/tgs-inventor/20041010--SoDrawStyle_linePatternScaleFactor.txt) | [Issue #81 fechada](https://github.com/coin3d/coin/issues/81); pedido presente no código. |
| Compatibilidade TGS Inventor | 149 | [Compatibility issue: SoVRMLSound fields dopplerFactor and dopplerVelocity missing in Coin.](../bugs/features/tgs-inventor/20041010--SoVRMLSound_dopplerFactor-and-dopplerVelocity.txt) | — |
| Compatibilidade TGS Inventor | 150 | [Compatibility issue: SoVRMLMovieTexture fields alpha, model, blendColor and scaleTexCoord missing in Coin.](../bugs/features/tgs-inventor/20041010--missing-fields-in-SoVRMLMovieTexture.txt) | — |
| Compatibilidade TGS Inventor | 169 | [SoDataSensor extension](../bugs/features/tgs-inventor/20041116--SoDataSensor-extension.txt) | — |
| Compatibilidade TGS Inventor | 170 | [SoIndexedFaceSet defines](../bugs/features/tgs-inventor/20041116--SoIndexedFaceSet-defines.txt) | — |
| Compatibilidade TGS Inventor | 172 | [SoFieldContainer and private fields](../bugs/features/tgs-inventor/20041116--SoFieldContainer-and-private-fields.txt) | — |
| Compatibilidade TGS Inventor | 174 | [SoTrackerEvent missing](../bugs/features/tgs-inventor/20041116--SoTrackerEvent-missing.txt) | — |
| Compatibilidade TGS Inventor | 178 | [SbMatrixd and SbRotationd missing from Coin.](../bugs/features/tgs-inventor/20041210--sbmatrixd-and-sbrotationd-missing.txt) | — |
| Compatibilidade TGS Inventor | 197 | [Compatibility issue: SoDB::threadInit() not implemented in Coin.](../bugs/features/tgs-inventor/20050620--sodb_threadinit-missing-in-Coin.txt) | — |
| Baixa prioridade: outros | 036 | [This program will hit an assert if we interact with the SoTrackballDragger at the same time as it is remove…](../bugs/low_priority/insignificant/20021011--crash_when_deleting_active_dragger.txt) | — |
| Baixa prioridade: outros | 041 | [Circular field connections not always handled.](../bugs/low_priority/insignificant/20021016--circular-field-connections.txt) | — |
| Baixa prioridade: entrada e saída | 025 | [Import hangs on invalid scenegraph. This reproduces the problem:](../bugs/low_priority/io/20020911--unhelpful_error_message_on_invalid_file.txt) | — |
| Baixa prioridade: entrada e saída | 104 | [Unhelpful read error message.](../bugs/low_priority/io/20040220--unhelpful-read-error-message.txt) | — |
| Baixa prioridade: entrada e saída | 165 | [Faulty error reporting upon reading bad file.](../bugs/low_priority/io/20041105--incorrect_error_message_on_invalid_file.txt) | — |
| Baixa prioridade: sistemas antigos | 200 | ["Fake" NVidia stereo reported to not work with Coin / SoWin/SoQt.](../bugs/low_priority/obscure_systems/20050627--nividia-stereo-hack-not-working.txt) | — |
| Baixa prioridade: sistemas antigos | 201 | [Coin 2.4.1 + SoQt 1.2.0 + Qt 3.3.4 hangs on Win98.](../bugs/low_priority/obscure_systems/20050627--coin-plus-soqt-hangs-on-win98.txt) | — |
| Baixa prioridade: robustez | 026 | [Bounding box calculation: the bbox for the Barcelona.iv model is _very_ sub-optimal. (SGI Inventor seems to…](../bugs/low_priority/robustness/20020915--suboptimal_bbox_on_buggy_model.txt) | — |
| Baixa prioridade: robustez | 176 | [SbTime::formatDate() will crash on bad (?) input.](../bugs/low_priority/robustness/20041207--sbtime_formatdate-not-robust-on-bad-input.txt) | — |
| Baixa prioridade: robustez | 184 | [SbOctTree::findItems() functions not multi-CRT safe on Windows.](../bugs/low_priority/robustness/20041221--multi-crt-robustness.txt) | — |
| Obsoletos | 003 | [There are seams in texturemaps on SGI Octane with "IMPACTSR" graphics hardware.](../bugs/obsolete/20020205--textures_on_sgi_octane.txt) | — |
| Obsoletos | 033 | [Some of the default geometry files for dragger geometries uses an SoDrawStyle::lineWidth > 1.0. This can be…](../bugs/obsolete/20020930--dragger_geometry_files_may_have_too_large_lineWidth.txt) | — |
| Desempenho | 115 | [SoBaseKit::setPart() is inefficient.](../bugs/performance_enhancements/20040607--SoBaseKit_setPart-is-inefficient.txt) | — |
| Desempenho | 118 | [Weakness in internal handling of multiple implicit references to the same texture.](../bugs/performance_enhancements/20040624--medium-pri-issue--multiple-implicit-refs-to-same-texture-file.txt) | — |
| Desempenho | 139 | [SoText2 nodes never culled?](../bugs/performance_enhancements/20040903--SoText2-nodes-never-culled.txt) | — |
| Desempenho | 173 | [SbIntList implementation](../bugs/performance_enhancements/20041116--SbIntList-inefficient.txt) | — |
| Desempenho | 202 | [SoLocateHighlight implementation is inefficient.](../bugs/performance_enhancements/20050630--SoLocateHighlight-implementation-is-inefficient.txt) | — |
| Desempenho | — | [libCgGL loading triggered at SoDB::init().](../bugs/performance_enhancements/20060208--libCgGL-not-lazy-loaded.txt) | — |
| Vazamentos de recursos | 001 | [Memory leak with SoSelection.](../bugs/resource_leaks/BUGS.txt) | Retenção reproduzida na auditoria local de 05/10/2026. |

## Encaminhamento

- **148:** já catalogado e encerrado na issue #81; não abrir uma nova issue com o mesmo pedido.
- **120:** revisar o desfecho da PR #930 antes de decidir se o relato histórico de relocação precisa de anotação adicional.
- **001:** a auditoria reproduziu retenção de `SoSelection`; preparar um relato atual só após conferir duplicatas abertas e delimitar a integração com o viewer.
- **Demais arquivos:** manter o relato histórico como fonte, verificar ambiente e reprodução atuais, depois relacionar à issue existente ou redigir publicação específica.
