# Mapa da documentação Doxygen do Coin

Levantamento em 21/11/2026 para a organização de `docs/`. O Doxygen é a
documentação de API ativa do projeto. A [versão publicada](https://www.coin3d.org/coin/)
mostra Coin 4.0.10 e foi gerada com Doxygen 1.10.0 em novembro de 2026.
Corrigir conteúdo dessa documentação significa editar suas fontes e gerar
novamente o site; o HTML gerado não é a fonte editorial.

## Onde está cada parte

| Parte | Fonte | Papel |
| --- | --- | --- |
| Página inicial | [`src/doc/Coin_main.dox`](../../src/doc/Coin_main.dox) | Introdução e navegação inicial. |
| Grupos de classes | [`src/doc/Coin_modules.dox`](../../src/doc/Coin_modules.dox) | Organiza ações, nós, campos, sensores, VRML, XML e outras APIs. |
| Páginas temáticas | [`src/doc/Coin_books.dox`](../../src/doc/Coin_books.dox), [`Coin_macros.dox`](../../src/doc/Coin_macros.dox), [`Coin_environment.dox`](../../src/doc/Coin_environment.dox) | Referências, macros e variáveis de ambiente. |
| Páginas de versões | [`docs/releases.dox`](../releases.dox) | Histórico de recursos por versão na documentação gerada. |
| API de classes e funções | Comentários em `src/` e declarações em `include/` | Material principal das páginas de referência. |
| Configuração clássica | [`docs/coin.doxygen.cmake.in`](../coin.doxygen.cmake.in) | Gera `html/index.html` pelo alvo CMake `documentation`. |
| Configuração moderna | [`docs/coin.doxygen.awesome.cmake.in`](../coin.doxygen.awesome.cmake.in) | Gera `html_awesome/index.html` pelo alvo `documentation_awesome`. |
| Aparência e imagens | [`docs/doxygen/`](../doxygen/), submódulo `docs/doxygen-awesome/` | Cabeçalho, CSS, imagens e tema moderno. |
| Build, checagem e publicação | [`src/doc/CMakeLists.txt`](../../src/doc/CMakeLists.txt), [checagem de PR](../../.github/workflows/documentation-pr-check.yml), [publicação](../../.github/workflows/documentation-workflow.yml) | Configura as entradas, valida a geração e os links em PRs; publica `html_awesome` após push em `master` ou disparo manual. |
| Auxiliares antigos | [`docs/coin_doxygenfiles.inc`](../coin_doxygenfiles.inc), [`docs/doxypp.sed`](../doxypp.sed) | Não são referenciados pelo fluxo CMake atual examinado; preservar até revisão específica. |

O CMake deixa `COIN_BUILD_DOCUMENTATION` e
`COIN_BUILD_AWESOME_DOCUMENTATION` desligados por padrão. O workflow de
publicação liga a segunda opção. Os dois caminhos desabilitam a lista
automática de `\todo` e a lista de itens depreciados; elas não substituem
os catálogos históricos [`todo.txt`](../todo.txt) e [`BUGS.txt`](../BUGS.txt).

## Item 119: `SbBool`

O [relato 119 do catálogo secundário](../bugs/documentation/20040625--sbool-is-not-in-coin-docs.txt)
descreve a ausência de `SbBool` no Doxygen. A declaração fica no cabeçalho
gerado a partir de [`basic.h.cmake.in`](../../include/Inventor/C/basic.h.cmake.in),
enquanto a descrição anterior estava inativa em
[`SoDB.cpp`](../../src/misc/SoDB.cpp). A descrição agora acompanha o `typedef`
na fonte do cabeçalho gerado. Uma geração local do alvo clássico
`documentation` com Doxygen 1.9.8 incluiu `SbBool` no grupo `coin_base`, com
a descrição esperada. O alvo moderno também foi gerado localmente com 1.9.8.
A publicação moderna, que usa Doxygen 1.10.0 no
[workflow](../../.github/workflows/documentation-workflow.yml), ainda precisa
ser conferida depois do merge.

## O que o fluxo de documentação ensina à triagem

- **Fonte e saída são evidências diferentes.** O comentário pode existir no
  código sem aparecer no HTML. Para encerrar um relato de documentação,
  registrar a fonte alterada e verificar a página ou entrada gerada.
- **A referência da API deve acompanhar o código.** Páginas temáticas em
  `src/doc/` explicam conceitos; comentários junto a declarações e métodos
  explicam seu contrato. O catálogo histórico registra a origem da mudança,
  sem ser ingerido automaticamente pelo Doxygen.
- **A validação precisa ocorrer antes da publicação.** A
  [checagem de PR](../../.github/workflows/documentation-pr-check.yml)
  constrói o alvo moderno sem publicar e falha se houver referências
  não resolvidas ou imagens ausentes. Os demais avisos permanecem registrados
  para triagem gradual: `WARN_IF_UNDOCUMENTED=YES` ainda produz milhares de
  avisos, portanto `WARN_AS_ERROR=NO` continua necessário.
- **As descrições precisam de alvos navegáveis.** As 142 variáveis de ambiente
  descritas em [`Coin_environment.dox`](../../src/doc/Coin_environment.dox)
  agora têm declarações somente para documentação em
  [`environment.dox.h`](../../src/doc/environment.dox.h). Os links usam
  âncoras próprias para evitar colisões com símbolos internos de mesmo nome.
- **A página inicial merece revisão editorial.**
  [`Coin_main.dox`](../../src/doc/Coin_main.dox) contém referências antigas a
  integrações gráficas e um rascunho de whitepaper desativado. Revisar
  esses caminhos e oferecer exemplos atuais é trabalho editorial separado
  da correção pontual de `SbBool`.

## Encaminhamento no GitHub

- Manter o Doxygen como documentação gerada a partir do repositório.
- Relatar falhas reproduzidas na documentação publicada em issues focadas,
  com URL da página, versão gerada, trecho fonte e resultado esperado.
- Usar discussions para decisões de estrutura, tutoriais e escopo editorial.
- Não converter cada aviso de build ou cada `\todo` do código em issue sem
  triagem; a fonte, a configuração e o site publicado precisam ser conferidos
  juntos.

Os alvos clássico e moderno foram reconstruídos localmente com Doxygen 1.9.8.
Ambos terminaram sem referências não resolvidas nem imagens ausentes.
O workflow de PR e a publicação usam Doxygen 1.10.0; conferir o resultado
da execução no GitHub antes de considerar essa versão validada.
