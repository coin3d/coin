# Índice dos documentos históricos de `docs/` para triagem no GitHub

Preparado em 21/11/2026. Este índice cobre as listas e notas `.txt` históricas da raiz de `docs/`. Preserva os textos originais e separa **registros históricos** de propostas atuais. IDs de `todo.txt` se repetem em `BUGS.txt` e não são números de issue do GitHub. As ideias sem número recebem um ordinal apenas neste índice.

## Encaminhamento por arquivo

| Arquivo | Conteúdo | Encaminhamento |
| --- | --- | --- |
| [`BUGS.txt`](../BUGS.txt) | 45 relatos | [Discussion #935](https://github.com/orgs/coin3d/discussions/935) publicada; [triagem técnica](bugs.md). |
| [`bugs/`](bugs-secondary.md) | 42 arquivos de relatos | Índice secundário; conferir duplicatas e reprodução antes de issues. |
| [`todo.txt`](../todo.txt) | 44 entradas numeradas | Backlog histórico; revisar estado e duplicatas antes de qualquer publicação específica. |
| [`api-wish-list.txt`](../api-wish-list.txt) | 45 ideias | Propostas de API; discussions para decisões de escopo, depois issues somente se houver especificação aceita. |
| [`projects.txt`](../projects.txt) | 12 propostas | Ideias de projetos, algumas ligadas a plataformas antigas; discussions somente após reavaliação. |
| [`plan.txt`](../plan.txt) | Marcos de 1999 | Arquivo diz expressamente que muitos itens estão completos ou obsoletos; preservar como plano histórico. |
| [`optimization.txt`](../optimization.txt) | Diretrizes e tarefas misturadas | Separar recomendações de trabalho verificável; cita o item 118 do catálogo secundário de bugs. |
| [`oiv_compatibility.txt`](../oiv_compatibility.txt) | Compatibilidade com Open Inventor | Referência histórica; números Bugzilla citados ali não são issues do GitHub. |
| [Doxygen](doxygen.md) | Documentação de API gerada | [Mapa próprio](doxygen.md): fontes em `src/doc/`, comentários no código, configurações em `docs/` e publicação no GitHub Pages. |
| [`c-porting.txt`](../c-porting.txt), [`quat2euler.txt`](../quat2euler.txt) | Notas técnicas | Preservar como documentação; validar exemplos se forem republicados. |
| [`foreign-language-bindings.txt`](../foreign-language-bindings.txt), [`misc-resources.txt`](../misc-resources.txt) | Links e referências | Revisar links e relevância antes de atualizar documentação. |
| [`oiki-launch.txt`](../oiki-launch.txt) | Anúncio de lançamento | Arquivo histórico; não é backlog. |
| `announcement-*.txt` | 19 anúncios de versões | Histórico de releases; não converter em issues. |

## `todo.txt`: 44 entradas

**Status abaixo:** uma nota escrita no arquivo antigo não é uma verificação do código atual. Cada item ainda precisa ser cruzado com issues, PRs e comportamento atual antes de publicação.

| ID histórico | Assunto | Nota de triagem |
| --- | --- | --- |
| TODO-000 | [Improve error reporting on texture loading](../todo.txt#L16) | Subitem de mensagem marcado como corrigido no próprio texto; pedido maior sem veredito. |
| TODO-001 | [A "stopwatch" for built-in profiling support](../todo.txt#L51) | A verificar |
| TODO-002 | [Heed the SoBoundingBoxCache::hasLinesOrPoints() settings](../todo.txt#L102) | A verificar |
| TODO-003 | [Clean up the file versioning mess](../todo.txt#L117) | A verificar |
| TODO-004 | [Re-design the public API for handling textures](../todo.txt#L252) | A verificar |
| TODO-005 | [Support compressed textures](../todo.txt#L295) | Atualização de 2005 diz implementado; `src/glue/gl.cpp` usa formatos comprimidos. Cobertura e completude ainda precisam ser verificadas. |
| TODO-006 | [Font specification](../todo.txt#L318) | A verificar |
| TODO-010 | [Make Coin Usable from ROM](../todo.txt#L332) | A verificar |
| TODO-011 | [Rethink handling of camera properties and display configuration.](../todo.txt#L340) | A verificar |
| TODO-012 | [The SoSphere should be tessellated better.](../todo.txt#L357) | A verificar |
| TODO-013 | [Implement an alternative to using macros for specifying nodekit parts.](../todo.txt#L365) | A verificar |
| TODO-014 | [Make rendering (and other operations) robust for inconsistent scenegraphs.](../todo.txt#L376) | A verificar |
| TODO-015 | [100% correct transparency mode.](../todo.txt#L389) | A verificar |
| TODO-016 | [Detect multiple instances of the Coin library within the same run-time application space.](../todo.txt#L410) | Atualização antiga diz implementado; `CoinStaticObjectInDLL` existe no código, mas a detecção ainda precisa de teste atual. |
| TODO-017 | [Move action-style code developed in other projects into proper actions.](../todo.txt#L432) | `SoToVRMLAction`, `SoToVRML2Action` e `SoReorganizeAction` existem no código; conferir o escopo ainda desejado. |
| TODO-018 | [Check out and build simage as part of the Coin build process.](../todo.txt#L452) | A verificar |
| TODO-019 | [Dynamically configurable viewer interaction (SoQt++)](../todo.txt#L516) | A verificar |
| TODO-020 | [SoSub*.h improvements.](../todo.txt#L531) | A verificar |
| TODO-021 | [Call-stack backtraces on errors.](../todo.txt#L544) | A verificar |
| TODO-022 | [VRML97 VRMLScript scripting node.](../todo.txt#L557) | Mesmo tema de BUGS-214 (VRMLScript/SpiderMonkey). |
| TODO-023 | [SoCollisionManager & So@Gui@CollisionViewer.](../todo.txt#L570) | A verificar |
| TODO-024 | [Optimizing the performance of the SoIntersectionDetectionAction.](../todo.txt#L579) | A verificar |
| TODO-025 | [Integrate QGL into SoQt.](../todo.txt#L605) | A verificar |
| TODO-026 | [WWW-interface on Coin / SoWin / SoQt builds.](../todo.txt#L706) | A verificar |
| TODO-027 | [Build Coin / SoQt / SoWin on MSWin without relying on MSVC++.](../todo.txt#L736) | A verificar |
| TODO-028 | [Support for multiple viewports in render areas](../todo.txt#L807) | A verificar |
| TODO-029 | [Better font support.](../todo.txt#L840) | A verificar |
| TODO-030 | [Offscreen context support functions.](../todo.txt#L915) | Marcado FIXED no próprio arquivo. |
| TODO-031 | [Handle POSIX paths under MSWin.](../todo.txt#L920) | A verificar |
| TODO-032 | [Port SGI's ''ivperf'' tool.](../todo.txt#L935) | A verificar |
| TODO-033 | [Port TGS's ''ivtovrml'' tool.](../todo.txt#L943) | A verificar |
| TODO-034 | [Image loading under MSWin through DirectX.](../todo.txt#L968) | A verificar |
| TODO-035 | [Clean up SoBase write code.](../todo.txt#L989) | Relaciona-se à escrita de grafos de BUGS-072; escopo mais amplo. |
| TODO-036 | [Write an "interior-geometry reducer" action.](../todo.txt#L1004) | A verificar |
| TODO-037 | [Take advantage of NVidia's occlusion culling extension.](../todo.txt#L1027) | A verificar |
| TODO-038 | [Duplicate API for TGS's SoBigImageRenderer extension.](../todo.txt#L1039) | A verificar |
| TODO-039 | [Move tristripifying code into SoReorganizeAction.](../todo.txt#L1048) | A verificar |
| TODO-040 | [Add built-in version numbers to win32 DLLs.](../todo.txt#L1055) | A verificar |
| TODO-041 | [Generic ADT in C for dynamic arrays.](../todo.txt#L1119) | A verificar |
| TODO-042 | [Coin internationalization.](../todo.txt#L1131) | A verificar |
| TODO-043 | [The Coin library should have an option to set up a fatal error handler.](../todo.txt#L1169) | A verificar |
| TODO-044 | [Use GL_TEXTURE_RECTANGLE_EXT extension.](../todo.txt#L1186) | A verificar |
| TODO-045 | [Coin / SoQt / Qt port or reimplementation of the SGI ''gview'' program.](../todo.txt#L1202) | A verificar |
| TODO-046 | [FILEFORMAT/DEFAULTS sections for the nodes.](../todo.txt#L1221) | A verificar |

## `api-wish-list.txt`: 45 ideias

A seção final do arquivo se chama “IMPLEMENTED, RESIDING IN SmallChange”. Isso registra implementação em outra biblioteca; não prova que cada recurso está disponível no Coin atual.

| ID deste índice | Ideia | Nota de triagem |
| --- | --- | --- |
| API-01 | [missing base class fields](../api-wish-list.txt#L19) | A verificar |
| API-02 | [SbVec2i](../api-wish-list.txt#L31) | A verificar |
| API-03 | [SoGLPrimitiveSet node.](../api-wish-list.txt#L49) | A verificar |
| API-04 | [More nodes and elements to control GL state.](../api-wish-list.txt#L115) | A verificar |
| API-05 | [Add SoMFMatrix input fields in SoCalculator](../api-wish-list.txt#L124) | A verificar |
| API-06 | [A better hash/dict class than SbDict.](../api-wish-list.txt#L135) | A verificar |
| API-07 | [SoGestureEvent](../api-wish-list.txt#L165) | A verificar |
| API-08 | [SoText2::verticalAlignment / verticalJustification](../api-wish-list.txt#L175) | A verificar |
| API-09 | [3D GUI Components](../api-wish-list.txt#L185) | A verificar |
| API-10 | [Less environment pollution](../api-wish-list.txt#L223) | A verificar |
| API-11 | [SbBool SbMatrix::inversePossible(void) const](../api-wish-list.txt#L276) | A verificar |
| API-12 | [SbList::setEqTester()](../api-wish-list.txt#L289) | A verificar |
| API-13 | [SoNode::setOverride() should be made persistent](../api-wish-list.txt#L311) | A verificar |
| API-14 | [SoDB::registerImporter()](../api-wish-list.txt#L330) | A verificar |
| API-15 | [New node TextureAttribute](../api-wish-list.txt#L347) | A verificar |
| API-16 | [void SbRotation::getAsEuler(SbVec3f &), void SbRotation::setAsEuler(const SbVec3f &)](../api-wish-list.txt#L375) | A verificar |
| API-17 | [SoFocusEvent](../api-wish-list.txt#L383) | A verificar |
| API-18 | [SoMarkerSet scaling](../api-wish-list.txt#L392) | A verificar |
| API-19 | [SbReal typedef](../api-wish-list.txt#L400) | A verificar |
| API-20 | [The SbBool typedef](../api-wish-list.txt#L417) | A verificar |
| API-21 | [Make APIs for engine classes more consistent.](../api-wish-list.txt#L434) | A verificar |
| API-22 | [SoInput::setStrings(char **)](../api-wish-list.txt#L445) | A verificar |
| API-23 | [SoInput/SoOutput alternative](../api-wish-list.txt#L459) | A verificar |
| API-24 | [SoAction::apply(SoAction * beingApplied)](../api-wish-list.txt#L476) | A verificar |
| API-25 | [SbFile](../api-wish-list.txt#L492) | A verificar |
| API-26 | [SoField::setValidator(SoValidator *)](../api-wish-list.txt#L535) | A verificar |
| API-27 | [SoKeyboardEvent::Key enums](../api-wish-list.txt#L545) | A verificar |
| API-28 | [SoSwitch::whichChild as an SoMFBool](../api-wish-list.txt#L635) | A verificar |
| API-29 | [SoMouseButtonEvent::isDoubleClick()](../api-wish-list.txt#L661) | A verificar |
| API-30 | [SbBigNum type](../api-wish-list.txt#L680) | A verificar |
| API-31 | [SoOffscreenRenderer::render(SoPathList &)](../api-wish-list.txt#L687) | A verificar |
| API-32 | [Support for field value expressions in Inventor ascii files](../api-wish-list.txt#L699) | A verificar |
| API-33 | [SoGLTeaPot](../api-wish-list.txt#L713) | A verificar |
| API-34 | [SoSearchPathAction (which is part of the TGS Inventor API)](../api-wish-list.txt#L725) | A verificar |
| API-35 | [Rewrite non-member/friend operator methods for the base classes.](../api-wish-list.txt#L733) | A verificar |
| API-36 | [Detect and support libtar if present, and update SoInput accordingly.](../api-wish-list.txt#L762) | A verificar |
| API-37 | [Prettier SoWriteAction .iv-formatting](../api-wish-list.txt#L794) | A verificar |
| API-38 | [Output .iv-files with CR/LF on Win32 systems](../api-wish-list.txt#L817) | A verificar |
| API-39 | [SoViewportEvent](../api-wish-list.txt#L829) | A verificar |
| API-40 | [Extending SoInput to support callbacks for file operations](../api-wish-list.txt#L841) | A verificar |
| API-41 | [SoDepthBuffer, SoGLDepthBufferElement](../api-wish-list.txt#L885) | Registrado como implementado em SmallChange; SoDepthBuffer também existe neste checkout. |
| API-42 | [SoViewportRegion](../api-wish-list.txt#L897) | Registrado como implementado em SmallChange; não há SoViewportRegion no Coin deste checkout. |
| API-43 | [Coinboard node](../api-wish-list.txt#L911) | O texto diz implementação parcial em SmallChange. |
| API-44 | [SoEnvironment::fogStart field.](../api-wish-list.txt#L936) | O texto diz suporte no elemento; conferir o campo público solicitado. |
| API-45 | [SoSkyDome node.](../api-wish-list.txt#L946) | O texto diz implementação parcial em SmallChange. |

## `projects.txt`: 12 propostas

Essas entradas são propostas para estudantes ou estagiários, não issues confirmadas. Algumas dependem de plataformas e ferramentas históricas.

| ID deste índice | Proposta | Nota de triagem |
| --- | --- | --- |
| PROJECT-01 | [A QNX port (QNX for x86 platforms is supposedly available for free](../projects.txt#L4) | Reavaliar escopo e relevância |
| PROJECT-02 | [A "gl-attack" application stressing many different aspects of](../projects.txt#L7) | Reavaliar escopo e relevância |
| PROJECT-03 | [Work with the documentation to improve it (add example code](../projects.txt#L10) | Reavaliar escopo e relevância |
| PROJECT-04 | [Write many more examples, covering the functionality offered by](../projects.txt#L13) | Reavaliar escopo e relevância |
| PROJECT-05 | [Write a tutorial.](../projects.txt#L16) | Reavaliar escopo e relevância |
| PROJECT-06 | [Write a NURBS-library covering all the necessary functionality we](../projects.txt#L18) | Reavaliar escopo e relevância |
| PROJECT-07 | [An IVF (MFC-based) GUI-glue library.](../projects.txt#L24) | Reavaliar escopo e relevância |
| PROJECT-08 | [Make simple routines for packaging up Coin for RedHat Linux (.rpm),](../projects.txt#L26) | Reavaliar escopo e relevância |
| PROJECT-09 | [Do the work needed for being compatible with 64-bit systems. These](../projects.txt#L31) | Reavaliar escopo e relevância |
| PROJECT-10 | [Profiling and optimalization of rendering and other action](../projects.txt#L44) | Reavaliar escopo e relevância |
| PROJECT-11 | [A regression test system for the classes under Coin/src/base.](../projects.txt#L47) | `testsuite/CMakeLists.txt` já define `CoinTests`; conferir lacunas específicas antes de propor outro sistema. |
| PROJECT-12 | [Profiling and optimalization of the base classes. Should depend on a](../projects.txt#L49) | Reavaliar escopo e relevância |

## Relações a preservar

- TODO-022 e BUGS-214 abordam VRMLScript; uma eventual publicação deve apontar ambos.
- TODO-035 e BUGS-072 abordam a escrita de grafos, mas o TODO trata thread safety de forma mais ampla.
- API-06 propõe substituir ou melhorar `SbDict`; comparar com o trabalho já feito em `SbHash` e `SbDict` antes de abrir nova proposta.
- A observação de `optimization.txt` sobre textura compartilhada aponta para o item 118 em [`bugs/performance_enhancements/`](../bugs/performance_enhancements/20040624--medium-pri-issue--multiple-implicit-refs-to-same-texture-file.txt), não para o item 118 do GitHub.

## Próxima etapa de migração

1. Conferir as notas explícitas de “fixed” e “implemented” no código atual.
2. Cruzar `todo.txt`, `api-wish-list.txt` e `projects.txt` com issues e PRs abertas e fechadas por sintoma, evitando interpretar IDs antigos como números do GitHub.
3. Preparar uma Discussion para cada lista ou tema amplo somente depois dessa triagem; criar issues focadas quando houver reprodução ou especificação atual.
