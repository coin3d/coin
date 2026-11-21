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
| Build e publicação | [`src/doc/CMakeLists.txt`](../../src/doc/CMakeLists.txt), [workflow](../../.github/workflows/documentation-workflow.yml) | Configura as entradas, constrói o alvo moderno e publica `html_awesome` no GitHub Pages após push em `master` ou disparo manual. |
| Auxiliares antigos | [`docs/coin_doxygenfiles.inc`](../coin_doxygenfiles.inc), [`docs/doxypp.sed`](../doxypp.sed) | Não são referenciados pelo fluxo CMake atual examinado; preservar até revisão específica. |

O CMake deixa `COIN_BUILD_DOCUMENTATION` e
`COIN_BUILD_AWESOME_DOCUMENTATION` desligados por padrão. O workflow de
publicação liga a segunda opção. Os dois caminhos desabilitam a lista
automática de `\todo` e a lista de itens depreciados; elas não substituem
os catálogos históricos [`todo.txt`](../todo.txt) e [`BUGS.txt`](../BUGS.txt).

## Questão histórica a verificar

O [relato 119 do catálogo secundário](../bugs/documentation/20040625--sbool-is-not-in-coin-docs.txt)
diz que `SbBool` não aparece como definição própria no Doxygen. O texto em
[`src/misc/SoDB.cpp`](../../src/misc/SoDB.cpp) usa `//` para as linhas com
`\typedef int SbBool`, fora de um bloco Doxygen convencional. A documentação
atual mostra usos de `SbBool` nas APIs, mas isso não comprova uma página ou
entrada própria para o tipo. Conferir o índice de tipos do HTML gerado antes
de classificar o item 119 como resolvido ou abrir uma issue nova.

## Encaminhamento no GitHub

- Manter o Doxygen como documentação gerada a partir do repositório.
- Relatar falhas reproduzidas na documentação publicada em issues focadas,
  com URL da página, versão gerada, trecho fonte e resultado esperado.
- Usar discussions para decisões de estrutura, tutoriais e escopo editorial.
- Não converter cada aviso de build ou cada `\todo` do código em issue sem
  triagem; a fonte, a configuração e o site publicado precisam ser conferidos
  juntos.

O submódulo `docs/doxygen-awesome` está não inicializado neste checkout,
portanto o alvo moderno não foi reconstruído nesta rodada. A estrutura do
workflow e o site publicado foram inspecionados.
