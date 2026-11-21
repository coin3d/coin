# Triagem do `BUGS.txt` para issues e discussions

Preparação local em 21/11/2026. Este documento organiza os 45 itens de
[`docs/BUGS.txt`](../BUGS.txt) sem alterar os relatos históricos. A auditoria local de 05/10/2026 é a base técnica para os estados abaixo; as PRs citadas estavam abertas na
consulta de 21/11/2026. **Não houve nova reprodução dos 45 casos nesta
triagem.** A presença no arquivo antigo não demonstra que um defeito ainda
existe, e a ausência de reprodução isolada não demonstra correção.

O catálogo dos 45 itens foi publicado na [Discussion #935 de
coin3d](https://github.com/orgs/coin3d/discussions/935). Ela usa links para
as linhas do `BUGS.txt` em `coin3d/coin:master` e separa os casos que já têm
issue ou PR dos candidatos a novos relatos.

## Como ler a triagem

- **Confirmado**: sintoma reproduzido ou limitação ainda identificada na
  auditoria. São 13 itens, incluindo 219 em PR aberta.
- **Parcial**: código, mitigação ou comportamento relacionado examinado, mas
  o relato completo não recebeu veredito. São 12 itens.
- **Não reproduzido**: o teste feito não mostrou o sintoma, com cobertura
  insuficiente para encerrar o item. São 2 itens.
- **Pendente**: precisa de reprodução ou decisão de contrato. São 18 itens.

Na coluna **Destino**, `Issue` significa um relato técnico candidato após
conferir duplicatas; `Discussion` significa uma decisão de API, política ou
escopo; `Verificar` significa reunir evidência antes de publicar. `Existente`
significa continuar na issue e PR já abertas. Os números de `BUGS.txt` são
identificadores históricos, **não** números de issue no GitHub.

## Inventário completo

| ID | Assunto | Estado | Destino | Próxima evidência ou decisão |
| --- | --- | --- | --- | --- |
| [005](../BUGS.txt#L33) | `SoSFEnum` conectado: possível leitura após liberação | Pendente | Verificar | Reprodutor sob ASan ou Valgrind. |
| [031](../BUGS.txt#L44) | Transparência alfa em renderização offscreen | Parcial | Verificar | Comparar pixels e escolha de contexto em backends e drivers atuais; relacionar a 102. |
| [045](../BUGS.txt#L93) | `SoCenterballManip` gira fora do centro | Pendente | Verificar | Executar a cena e o gesto do relato. |
| [047](../BUGS.txt#L132) | `SoAntiSquish` e fatoração de matriz em draggers | Pendente | Verificar | Reproduzir rotação e escala no exemplo original. |
| [051](../BUGS.txt#L169) | `SoImage` e `viewAll` antes de exibir viewer | Pendente | Verificar | Testar integração SoQt com viewport inicial. |
| [054](../BUGS.txt#L229) | `SoComputeBoundingBox` com viewport variável | Parcial | Verificar | Reproduzir atualização do engine com câmera e viewer externos. |
| [060](../BUGS.txt#L252) | `SoToVRML2Action` perde geometria VRML97 | Não reproduzido | Verificar | Ampliar para grafos mistos e conferir campos. |
| [066](../BUGS.txt#L303) | `viewAll` com viewport estreito e coordenadas grandes | Pendente | Verificar | Testar cena de cinco segmentos, janela 2:1 e rotações. |
| [072](../BUGS.txt#L374) | Nodekit de extensão não relê após escrita binária | Confirmado | Issue | Usar reprodutor `SoTestKit` da auditoria; reler buffer falha. [Issue fechada #93](https://github.com/coin3d/coin/issues/93) trata outro caso de `writeref`, não este. |
| [073](../BUGS.txt#L466) | Uma `SoTexture2` com qualidades diferentes | Parcial | Verificar | Comparar filtros e pixels em dois usos da mesma textura. |
| [074](../BUGS.txt#L543) | `SoComplexity` depois de `SoTexture2` | Pendente | Discussion | Decidir comportamento esperado da ordem; depois testar cena exata. |
| [075](../BUGS.txt#L576) | `SoHandleBoxManip` salta ao soltar Shift | Pendente | Verificar | Reproduzir gesto e trajetória. |
| [076](../BUGS.txt#L596) | `SoHandleBoxManip` salta na translação Z | Pendente | Verificar | Testar projeção e câmera do relato. |
| [090](../BUGS.txt#L625) | `copy(TRUE)` não replica conexões de saída | Confirmado | Discussion | Diferença reproduzida no Coin; confirmar contrato SGI antes de propor mudança. |
| [092](../BUGS.txt#L694) | `SoToVRMLAction` ignora nós Inventor/Coin | Confirmado | Discussion | Limitação reproduzida e documentada; decidir se ampliar a conversão. |
| [095](../BUGS.txt#L704) | `SoIndexedTriangleStripSet` lento com normais `PER_FACE` | Pendente | Verificar | Benchmark atual com binding equivalente. |
| [099](../BUGS.txt#L1114) | Frustum culling depende de bounding boxes | Parcial | Discussion | Existe `COIN_GLBBOX`; decidir padrão ou documentação desejados. |
| [102](../BUGS.txt#L1165) | Falta API para configurar contexto offscreen | Confirmado | Discussion | Definir requisitos públicos e backends; workaround de stencil não cobre a API pedida. |
| [108](../BUGS.txt#L1206) | Texturas assíncronas ausentes no primeiro frame | Pendente | Verificar | O sintoma principal já foi catalogado na [issue fechada #182](https://github.com/coin3d/coin/issues/182), encerrada com configuração para desativar o carregamento atrasado. Testar se ainda ocorre sem essa configuração e separar a observação sobre texturas fraturadas. |
| [114](../BUGS.txt#L1278) | Importação renomeia identificador VRML válido | Confirmado | Issue | Caso mínimo `DEF @foo Group {}` produz `_@foo`. |
| [121](../BUGS.txt#L1298) | Tiles falham com mudanças de subviewport | Confirmado | Discussion | Arquitetura ainda pendente; reproduzir overlay e definir contrato de viewport. Relacionar a 191. |
| [129](../BUGS.txt#L1331) | Conjunto de bugs de escala, picking e draggers | Pendente | Verificar | Separar os cenários internos e testar cada gesto; há proteções parciais. |
| [133](../BUGS.txt#L1460) | Lasso de `SoExtSelection` aparece em várias views | Pendente | Verificar | Testar nó compartilhado em duas views. |
| [135](../BUGS.txt#L1492) | Elementos de `doAction()` em novas actions | Parcial | Discussion | Executar action customizada e definir contrato de extensão. |
| [140](../BUGS.txt#L1532) | Possível leak na cópia de `SoVRMLParent` | Parcial | Verificar | Refcount isolado tem explicação legítima; testar destruição sob ASan. |
| [141](../BUGS.txt#L1589) | Empilhamento de contextos offscreen | Parcial | Verificar | Testar contextos GLX aninhados e mistos; [PR #924](https://github.com/coin3d/coin/pull/924) cobre restauração EGL, não todo o relato. |
| [143](../BUGS.txt#L1667) | Filtragem de avisos de cena defeituosa | Parcial | Discussion | Definir categorias de avisos e API desejada. |
| [157](../BUGS.txt#L1694) | Estilos de fonte no Windows | Parcial | Verificar | Testar `:Bold` e formatos atuais no Windows. A [issue fechada #78](https://github.com/coin3d/coin/issues/78) corrigiu seleção de nomes de fonte por conversão UTF-8/UTF-16, um defeito distinto. |
| [162](../BUGS.txt#L1722) | Busca não alcança campos VRML `SFNode`/`MFNode` | Confirmado | Issue | `SoSearchAction` não encontra `FontStyle`; consolidar com 196. |
| [168](../BUGS.txt#L1742) | Possível use-after-free em nós VRML | Pendente | Verificar | Relato deixou de reproduzir em 2005; falta cena original ou equivalente sob ASan. |
| [191](../BUGS.txt#L1852) | Câmera ortográfica `CROP` com tiles | Confirmado | Issue | Limitação explícita no código; comparar imagens com e sem tiles. Relacionar a 121. |
| [194](../BUGS.txt#L1929) | Assert em tile mode com grafo VRML97 | Pendente | Verificar | Executar caso em Debug; caminho do callback mudou. |
| [196](../BUGS.txt#L1954) | Busca dentro de `PROTO` VRML97 | Confirmado | Issue | `FontStyle` não encontrado no exemplo; consolidar com 162. |
| [203](../BUGS.txt#L2072) | Convenção de chamada de DLL OpenAL | Pendente | Verificar | Requer DLL problemática e teste Windows. |
| [205](../BUGS.txt#L2096) | Hardcopy com mudanças de viewport | Pendente | Verificar | Comparar PostScript da cena original de quatro views. |
| [208](../BUGS.txt#L2112) | Tesselação padrão de `SoText3` excessiva | Pendente | Discussion | Medir custo e qualidade visual. [PR #920](https://github.com/coin3d/coin/pull/920) limita complexidade fora da faixa, sem encerrar esta pergunta. |
| [209](../BUGS.txt#L2126) | `SoTranslate1Dragger` translada sua geometria | Pendente | Verificar | Reproduzir cena conectada a `RotationXYZ` e comparar com SGI. |
| [210](../BUGS.txt#L2168) | Tamanho de `SoText2`: pixels ou pontos | Parcial | Discussion | Política atual usa pixels; decidir compatibilidade e DPI esperado. |
| [211](../BUGS.txt#L2184) | Primitivas ignoram funções de coordenadas de textura | Parcial | Verificar | Comparar pixels em Sphere/Cube/Cone/Cylinder. A [issue fechada #75](https://github.com/coin3d/coin/issues/75) corrigiu callbacks sucessivos de `TextureCoordinatePlane`, outro caso. [PR #873](https://github.com/coin3d/coin/pull/873) e [#906–908](https://github.com/coin3d/coin/pull/906) cobrem aspectos relacionados, sem comprovar solução do render. |
| [213](../BUGS.txt#L2196) | `SbTesselator` falha em polígono de 15 vértices | Não reproduzido | Verificar | Área fechou; ainda falta validar topologia, sobreposição e bordas. |
| [214](../BUGS.txt#L2251) | `VRMLScript` e SpiderMonkey atual | Confirmado | Discussion | Suporte JS legado segue indisponível; decidir backend e escopo. |
| [215](../BUGS.txt#L2273) | `SoText2` sob `SoClipPlane` | Parcial | Discussion | Reproduzir visualmente e decidir semântica do clipping. |
| [217](../BUGS.txt#L2314) | `DrawStyle::POINTS` vira linha na conversão VRML2 | Confirmado | Issue | `LineSet` e `IndexedLineSet` geram `IndexedLineSet`, sem `PointSet`. |
| [219](../BUGS.txt#L2326) | `viewAll` não enquadra `SoText2` | Confirmado | Existente | Continuar na [issue #417](https://github.com/coin3d/coin/issues/417) e [PR #774](https://github.com/coin3d/coin/pull/774), ainda abertas. |
| [221](../BUGS.txt#L2357) | Campo global não avalia ao ler arquivo | Confirmado | Issue | No exemplo, `string` só atualiza após `touch()`. |

## Cruzamento com issues fechadas

Foram examinados os títulos e corpos das **259 issues fechadas** em
`coin3d/coin`, além dos comentários dos candidatos abaixo. A relação
depende do sintoma e da reprodução, não apenas de mencionar a mesma classe.
O estado `closed/completed` informa como a issue foi encerrada; não comprova,
por si só, que todos os sintomas do arquivo histórico foram corrigidos.

| Arquivo antigo | Issue fechada | Relação encontrada |
| --- | --- | --- |
| `BUGS.txt` 108 | [#182](https://github.com/coin3d/coin/issues/182) | **Mesmo sintoma principal**: textura VRML ausente em render offscreen. O comentário de encerramento indica `SoVRMLImageTexture::setDelayFetchURL(FALSE)` como solução de uso. Não avalia toda a proposta de sincronização nem a observação sobre textura fraturada. |
| `BUGS.txt` 072 | [#93](https://github.com/coin3d/coin/issues/93) | **Relacionado, distinto**: contagem de referências ao escrever grafo. #93 trata grupos com conexões de campo e foi encerrada após uma PR; o nodekit binário de 072 continuou reproduzível na auditoria. |
| `BUGS.txt` 157 | [#78](https://github.com/coin3d/coin/issues/78) | **Relacionado, distinto**: seleção de fonte no Windows corrigida por conversão do nome UTF-8/UTF-16; 157 pergunta pelo sufixo de estilo `:Bold`. |
| `BUGS.txt` 211 | [#75](https://github.com/coin3d/coin/issues/75) | **Relacionado, distinto**: atualização sucessiva de `TextureCoordinatePlane`; 211 trata uso da função por primitivas. |
| `BUGS.txt` 121 | [#74](https://github.com/coin3d/coin/issues/74) | **Relacionado, distinto**: offset vertical em render offscreen sem tiles, corrigido; 121 trata tiles com mudança de subviewport. |
| `BUGS.txt` 219 | [#126](https://github.com/coin3d/coin/issues/126), [#384](https://github.com/coin3d/coin/issues/384) | **Relacionado, distinto**: qualidade e alpha test de `SoText2`, não enquadramento por `viewAll`. O vínculo direto permanece na issue aberta #417 e PR #774. |
| `BUGS.txt` 141 | [#505](https://github.com/coin3d/coin/issues/505), [#591](https://github.com/coin3d/coin/issues/591) | **Relacionado, distinto**: falhas de contexto GLX em RHEL/SLES e WGL no Windows, não empilhamento aninhado/misto. |
| `docs/bugs/` 148 | [#81](https://github.com/coin3d/coin/issues/81) | **Mesmo pedido, encerrado**: `SoDrawStyle::linePatternScaleFactor`. O campo e sua inicialização estão presentes no código atual; não republicar o pedido histórico. |

Os outros itens do `BUGS.txt` não tiveram correspondência direta identificada
nas 259 issues fechadas consultadas. Isso não exclui discussões, issues ainda
abertas, PRs ou registros de outros repositórios. Os arquivos secundários de
`docs/bugs/` estão listados em [seu índice](bugs-secondary.md).

## Ordem sugerida para preparar publicações

1. **Continuar o caso existente:** 219 na issue #417 e PR #774.
2. **Preparar issues com reprodução:** 072, 114, 162+196 em conjunto, 217 e 221. O item 191 requer comparação visual para delimitar o sintoma antes da redação final.
3. **Abrir decisões de escopo em discussions:** 090, 092, 102, 121 e 214. Relacionar 121 a 191 sem confundir causa arquitetural e caso concreto.
4. **Reproduzir antes de publicar:** os itens `Parcial`, `Não reproduzido` e `Pendente`, especialmente 211, 073 e os relatos que dependem de Windows ou de drivers.

Antes de abrir qualquer issue nova, buscar duplicatas por sintoma e API no
GitHub. Cada publicação deve trazer o exemplo mínimo, resultado observado,
resultado esperado, versão/ambiente, evidência atual e referência ao ID
histórico do `BUGS.txt`. Um item amplo como 129 deve ser dividido por
comportamento reproduzível; 162 e 196 devem começar como um único relato
com dois casos relacionados.
