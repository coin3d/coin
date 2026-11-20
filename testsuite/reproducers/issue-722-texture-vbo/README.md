# Coordenadas de textura e vertex arrays

A matriz cobre `SoIndexedFaceSet`, `SoFaceSet`, `SoPointSet`,
`SoIndexedPointSet`, `SoIndexedLineSet` e `SoVRMLIndexedFaceSet`.
Para cada shape, verifica a imagem sem textura, com UV constante no shader,
com mapeamento padrão, com coordenadas explícitas, com função dependente da
normal, com duas texturas padrão, com GL texgen e com normais geradas. Cada
caso renderiza dois quadros e exige ausência do diagnóstico de array de textura ausente.

São **144 casos / 288 quadros**: oito configurações de textura, seis shapes e
três modos de renderização (imediato, arrays CPU e VBO). `SoFaceSet` também
exercita `startIndex` diferente de zero e coordenadas não utilizadas no início.

No CMake, o alvo `CoinTextureArrayTest` e o teste
`TextureCoordinateArrayRendering` são registrados para o renderizador GL
legado em Linux/Unix com GLX e suporte VRML. O CTest usa `xvfb-run` quando
disponível; sem ele, precisa de DISPLAY e retorna 77 se não houver display.

```sh
cmake --build /path/to/build --target CoinTextureArrayTest
ctest --test-dir /path/to/build -R '^TextureCoordinateArrayRendering$' --output-on-failure
```

Para executar também o reprodutor original da issue 722:

```sh
xvfb-run -a ./run.sh /path/to/build/lib
```

Para comprovar a seleção do caminho em uma biblioteca Coin com símbolos de
depuração, depois de executar `run.sh`:

```sh
xvfb-run -a ./verify-arrays.sh /path/to/build/lib
```

O depurador verifica **72 casos** (seis shapes, seis configurações e dois
modos de arrays), contando duas chamadas de arrays por processo e o valor
VBO recebido por `finishVertexArray()`. Mapeamento padrão, explícito e GL
texgen devem usar arrays; função dependente da normal e multitextura padrão
devem usar o fallback. Faces com normal cache também usam fallback quando
o array não cobre o startIndex ou os bindings não permitem arrays. A matriz
visual verifica também o resultado desses fallbacks.

Resultados locais em 2026-10-12: compilação aprovada; reprodutor original
3/3; matriz visual 144/144, 288 quadros corretos; depurador 72/72. Os resultados
foram obtidos em Xvfb/Mesa com `COIN_GLX_PIXMAP_DIRECT_RENDERING=1`. Outros
drivers e plataformas ainda dependem de validação própria.

Após atualizar a branch sobre `origin/master` (`ca247c1321`), foram removidos
10 testes gerados obsoletos do build reutilizado. `CoinTests` compilou e passou
**419 testes / 307.739 verificações**. A regressão de textura foi repetida
nessa base. Os demais executáveis independentes registrados no CTest não
foram todos compilados ou executados nesta etapa.


## Cache de mapeamento padrão

Preparação local de 2026-11-19: o cache é aplicado sobre master + o head
atual da #833. As chamadas a isAffordable() foram preservadas, incluindo o
limite em bytes derivado de sizeof(SbVec4f) e a proporção por vértices usados.
Uma entrada de UVs SbVec2f é retida por shape elegível. Notificações invalidam
o mapeamento; draws ativos mantêm referências e a liberação ocorre fora do
lock global. Uma entrada sem draws ativos reaproveita seu vetor na próxima
geração de mesmo tamanho. Ao mudar o tamanho, o vetor é substituído para não
reter a capacidade de uma fonte maior. Não há um orçamento global para todos
os shapes.

A estrutura de cache fica em um registro externo, removido quando o shape é
destruído; o layout de SoShapeP permanece preservado. O guard
temporário conserva seu layout e suas entradas antigas. O caminho de cache
usa o tipo privado separado SoCachedVertexArrayTexCoordGuard.

O consumidor `legacy-guard-client.cpp` deve ser compilado com os headers da
biblioteca baseline, incluindo `src/shapenodes`, e ligado contra ela. Executar
o mesmo binário com cada libCoin verifica as entradas dinâmicas antigas, o
espaço reservado pelo cliente, o array gerado e a restauração de estado. As
chamadas usam os símbolos Itanium diretamente para não cair em corpos inline.

A chave inclui também os flags do bundle, pois #901 usa flags para distinguir
representações de extents. Essa mudança não incorpora os ajustes numéricos
de #901 ou #902. Para validar uma composição que contenha essas correções,
`COIN_CACHE_SOURCES=wide-bounds` seleciona em `cache.sh` uma caixa extrema
cuja representação muda por estado herdado, preservando os valores codificados
de origem e tamanho. A referência calcula as UVs independentemente em double.
Esse caso opt-in depende da correção numérica da #901; os casos padrão não.

O controle adicional usa o benchmark já existente da #833:

```sh
cmake --build /path/to/build --target CoinSharedCoordinateBenchmark
COIN_GLXGLUE_NO_PBUFFERS=1 xvfb-run -a sh ./verify-cache-budget.sh \
  /path/to/build/bin/CoinSharedCoordinateBenchmark
```

Ele exige zero gerações de UV na fonte compartilhada de 410881 pontos para
draws de 4096 vértices. Nas oito fontes próprias, exige 16 gerações, oito
entradas alocadas e oito atualizações GL por subdata: a edição reaproveita as
entradas e os buffers. Construtores e gerações têm contadores separados.
As medições de desempenho apresentadas abaixo são históricas (2026-10-12);
não representam uma nova medição de desempenho desta preparação.

A otimização guarda o bounding box com as dependências lidas por
`computeBBox()`, o array de UVs e um VBO opcional. O shape conserva uma
entrada atual; cada draw mantém sua própria referência ao armazenamento.
O cache é refeito quando os dados ou os parâmetros de mapeamento mudam.
O array 2D preserva S/T do callback existente, com R=0 e Q=1.

`CoinTextureCacheTest` e o CTest `DefaultTextureCoordinateCache` comparam
**768 quadros**: oito fontes/cenários, com e sem display lists pais,
três caminhos de renderização, oito fases e dois contextos. A referência
usa UVs explícitas calculadas independentemente do cache. Os cenários
incluem `SoVertexProperty`, coordenadas herdadas, coordenadas homogêneas,
um shape compartilhado entre estados e um bounding box que depende de
um elemento herdado, além de bounds vazios fornecidos por uma extensão.
Dois cenários adicionais percorrem os seis pares ordenados de eixos S/T,
usando fontes 3D e homogêneas e mudanças de bounds por estado herdado.
As fases alteram posições, índices, tamanho do array,
limites degenerados e estado. O display list pai também é criado a partir
de um resultado já em cache, exercitando a propagação das dependências.

```sh
cmake --build /path/to/build --target CoinTextureCacheTest
ctest --test-dir /path/to/build -R '^DefaultTextureCoordinateCache$' --output-on-failure
xvfb-run -a ./verify-cache.sh /path/to/build/bin/CoinTextureCacheTest
```

Em Debug, `verify-cache.sh` verifica sete gerações de UV e de bounding box
em dezesseis quadros por processo: o quadro inalterado reaproveita ambos.
Para VBO, são quatorze uploads de UV (um por geração em cada contexto),
incluindo atualizações por subdata; o teste exige que haja reaproveitamento.
Para arrays CPU, são zero uploads. São seis execuções / 96 quadros.

Quando uma entrada exclusiva muda sem alterar o tamanho, o VBO do contexto
atual é atualizado por `glBufferSubData`. Os outros contextos são invalidados
e recebem um novo upload no próximo uso. Mudanças de tamanho usam a alocação
normal. `SoVBO::updateBufferData()` preserva o binding e acrescenta somente
um método não virtual, sem novos campos ou mudança de layout em `SoVBO`.

## Comparação de desempenho entre bibliotecas

O benchmark é separado dos testes e não impõe um limite de tempo no CI.
Para reproduzir a comparação, conservar as bibliotecas anterior e posterior
em diretórios distintos, compilar o mesmo executável e executá-las em ordem
alternada dentro de uma sessão Xvfb:

```sh
c++ -O2 benchmark.cpp -o /tmp/coin-texture-benchmark \
  -I/path/to/build/include -L/path/to/build/lib -lCoin -lGL
xvfb-run -a python3 benchmark.py /tmp/coin-texture-benchmark \
  /path/to/before/lib /path/to/after/lib --output /tmp/coin-texture-benchmark.json
```

Resultados locais: biblioteca do PR 833 (`8a531fef49`) versus cache de
bounding box, UVs e VBO. Mesma cena, mesmo executável, biblioteca Debug,
Mesa/llvmpipe, 128×128. Cada célula é a mediana de três execuções de trinta
quadros, após cinco quadros de aquecimento. O cronômetro inclui `glFinish()`;
construção da cena e leitura final da imagem ficam fora. Todos os casos
validaram a imagem. Não é uma previsão de desempenho de Release em GPU física.

| Faces | PR 833 (ms/quadro) | Com cache (ms/quadro) | Redução adicional |
| ---: | ---: | ---: | ---: |
| 10000 | 5.55 | 2.65 | 52.2% |
| 40000 | 23.26 | 12.43 | 46.6% |
| 160000 | 66.36 | 23.32 | 64.9% |

O cache conserva memória CPU e recursos GL entre quadros, em vez de
recriar o array temporário. A entrada anterior é liberada quando deixa de
ser usada; SoVBO administra os recursos de cada contexto. Concorrência no
VBO de uma mesma entrada é serializada durante o draw; os testes acima
usam dois contextos em sequência, sem um teste de renderização simultânea.

## Shutdown e validação Release — 2026-11-19

O registro externo pertence ao ciclo de init/cleanup de SoShape. Não tem
destrutor estático C++: cenas liberadas por callbacks atexit registrados antes
do primeiro render ainda podem retirar suas entradas. A limpeza do Coin
destrói o registro e zera o ponteiro, permitindo novos ciclos init/finish.
Os testes DefaultTextureCacheLifecycle cobrem cleanup tardio e três ciclos,
com arrays CPU e VBO. O controle opcional lifecycle.gdb deve indicar três
registros construídos e três destruídos ao executar `property cycles`.

Validação local Linux/GCC13: 130/130 CTests em Debug e 130/130 em Release;
118/118 com renderer legado desligado em Release; composição com #901/#902:
224/224. Todos passaram sem skips. O mesmo teste de cleanup tardio aborta
na biblioteca anterior à correção e passa na baseline e na corrigida.
ABI Debug sem suppressions: zero funções removidas/alteradas. Em Release,
nenhum símbolo dinâmico da baseline foi removido.

Benchmark atualizado: mesmas cenas e executáveis, alternando somente libCoin
Release da base master+#833 (44025052aea) e do cache. Mesa/llvmpipe, viewport
128x128, quatro threads do driver e três repetições alternadas; cronômetro
em render() com glFinish no grafo, leitura/hash fora do cronômetro. Há 30
quadros por fase estável nas malhas e 10 nas cenas de 400 shapes. Todos os
hashes coincidem entre bibliotecas. RSS é do processo inteiro; inclui Mesa e
o alocador e não representa isoladamente memória viva do cache ou VRAM.

| Cena | Estado estável antes/depois (ms) | RSS estável antes/depois (MiB) |
| --- | ---: | ---: |
| 10000 faces, uma malha | 5.035 / 4.732 | 120.941 / 120.789 |
| 40000 faces, uma malha | 13.142 / 13.064 | 133.316 / 133.145 |
| 160000 faces, uma malha | 24.696 / 24.249 | 184.586 / 184.594 |
| 400 shapes, fonte compartilhada | 93.850 / 85.311 | 203.414 / 203.836 |
| 400 shapes, fontes próprias | 85.601 / 72.181 | 232.141 / 251.879 |

Na cena com fontes próprias, o quadro após invalidação passou de 95.537 para
107.895 ms; a fase reestabilizada passou de 82.095 para 63.657 ms, com RSS
251.527 / 291.398 MiB. A retenção e a reconstrução dos buffers têm custo.
O controle explícito de 40000 faces variou de 12.499 para 13.580 ms e o
controle sem textura de 12.268 para 12.171 ms. Assim, esta rodada não sustenta
uma melhora geral nas malhas únicas: seus ganhos são pequenos perante a
variação dos controles. As medições históricas Debug acima não são uma
previsão dos resultados Release nem de GPU física.

Para repetir com primeiro render, invalidação, RSS/pico e liberação da cena:

```sh
cmake --build /path/to/release --target CoinDefaultTextureCacheBenchmark CoinSharedCoordinateBenchmark
LP_NUM_THREADS=4 LIBGL_ALWAYS_SOFTWARE=1 COIN_GLXGLUE_NO_PBUFFERS=1 xvfb-run -a \
  python3 benchmark.py /path/to/release/bin/CoinDefaultTextureCacheBenchmark \
    /path/to/baseline/lib /path/to/candidate/lib \
    --shared-binary /path/to/release/bin/CoinSharedCoordinateBenchmark \
    --output /tmp/cache-release-benchmark.json
```

## Reaproveitamento e custo de invalidação — 2026-11-20

Comparação local entre a baseline `44025052aea`, o cache anterior
`af4847c6f6b` e o reaproveitamento `c12131a4a97`. Mesmas bibliotecas Release,
executável, cenas e configuração Mesa/llvmpipe descritos acima. A rodada
completa anterior/final mantém três repetições alternadas e os controles
explícito e sem textura. Todos os hashes coincidiram. A carga externa do PC
subiu de 23,6 para 35,3; houve grandes oscilações de tempo decorrido também
nos controles. Essa rodada não permite concluir melhora geral de latência.

Uma segunda comparação de três bibliotecas registra também o tempo de CPU do
processo, somado sobre seus threads, com ordem rotativa por repetição. São
três repetições por biblioteca/layout e dez quadros por fase estável, com
`glFinish()` antes de encerrar cada amostra. Leitura e hash ficam fora dos
dois relógios. A carga permaneceu próxima de 32; CPU reduz a interferência
de desescalonamento, mas ainda sofre com frequência e contenção de memória.

400 shapes com fontes próprias, medianas das três repetições:

| Fase | CPU baseline (ms) | CPU cache anterior (ms) | CPU reaproveitamento (ms) | RSS anterior / reaproveitamento (MiB) |
| --- | ---: | ---: | ---: | ---: |
| Primeiro render | 403.073 | 428.501 | 445.653 | 251.160 / 251.012 |
| Estável | 167.544 | 148.832 | 148.782 | 258.074 / 257.691 |
| Após invalidação | 186.688 | 212.116 | 193.900 | 290.750 / 280.332 |
| Reestabilizado | 168.734 | 137.534 | 141.270 | 291.801 / 282.949 |
| Cena/contextos liberados | — | — | — | 280.273 / 263.070 |

O reaproveitamento reduz 8,6% de CPU e 10,4 MiB de RSS no quadro invalidado.
O ganho estável de CPU frente à baseline continua em 11,2%; após a edição,
em 16,3%. A invalidação ainda custa 3,9% mais CPU que a baseline. O primeiro
render não melhorou e a fase reestabilizada variou +2,7% frente ao cache
anterior. A memória estável permanece semelhante; o cache conserva seu custo
de retenção (257,691 MiB contra 233,285 MiB da baseline nesta cena).
RSS e pico são do processo inteiro, incluindo driver e alocador.

Na fonte compartilhada, sem cache de UV pelo orçamento, CPU estável anterior
/ final foi 205,991 / 208,363 ms e reestabilizada 211,598 / 213,126 ms. Esse
controle também varia, apesar de não gerar arrays de UV. A alternativa
experimental sem VBO de UV reduziu memória neste PC, mas não foi adotada:
o resultado final mantém os VBOs e atualiza sua alocação quando possível.
Não há previsão de ganho idêntico em GPU física.

Validação: 130 testes existentes e a nova regressão direta de buffers passaram
em Debug e Release; 118 com renderer desligado e sem DISPLAY; 225 na
composição local com #901/#902, além dos 96 quadros do caso `wide-bounds`.
Sem skips. `DefaultTextureBufferReuse` confere bytes, binding, identificador
GL reaproveitado, crescimento/redução e invalidação em dois contextos, para
buffers de vértices e índices. Os controles GDB exigem as mesmas gerações e
uploads do mapeamento e comprovam o reaproveitamento de oito entradas/VBOs.

ABI contra a baseline master+#833: `abidiff --no-default-suppression
--no-show-locs` aponta zero funções removidas/alteradas; os símbolos Release
da baseline e o consumidor antigo são preservados. Não há suppressions nem
mudança de layout em SoShapeP, SoVBO ou no guard antigo. A comparação bruta
adicional com o cache anterior mostra a saída de uma instância weak privada
de `std::vector<SbVec2f>::_M_default_append`, emitida por `resize()`; esse
símbolo foi acrescentado pelo cache não publicado e não existe na baseline.

Dados e comandos locais: `/tmp/coin-cache-memory-20261120/RESULTS.txt`,
`benchmark-cpu.json`, `benchmark-previous-final.json`, `final-threeway.json`,
logs de validação e `release-symbols.json`. Nenhum PR, push ou comentário
remoto foi feito.

## Regeneração em lote — 2026-11-20

A geração percorre diretamente o array 3D ou homogêneo no bundle. O mesmo
normalizador 2D atende `get()`, o callback e a geração em lote; não se copia
a fórmula para SoShape. O caminho homogêneo mantém `getReal()`. Não há
recíproco aproximado, novas flags de ponto flutuante ou novo teto de memória.
Um método privado não virtual foi acrescentado ao bundle, sem campos novos.
O caminho escalar continua calculando diretamente, sem chamada extra ao
callback. O compilador vetorizou o laço 3D (`divps`) nas flags Release normais.

Sondas temporárias de CPU da thread no código anterior mediram, na edição de
400 shapes: 7,738 ms em geração de UV, 7,302 ms em reconstrução de bounds e
1,781 ms em `updateBufferData`. Isso aponta uma oportunidade no despacho por
vértice (1.638.400 coordenadas). As sondas foram removidas antes dos benchmarks;
elas não contabilizam todo o trabalho paralelo do driver nem o upload inicial.

Nova comparação no Ryzen 7 5800H/16 CPUs, llvmpipe com quatro threads, bibliotecas
Release e mesmo executável: baseline `44025052aea`, versão anterior `0453097c069`
e lote `d4954e73d27`. Builds em RAM; sem nossos builds/testes concorrentes.
Carga 2,09 / 3,37 no início/fim. Seis repetições na cena própria com VBO, com as
seis ordens possíveis das três bibliotecas. Dez quadros estáveis por execução;
CPU agrega os threads do processo, wall inclui esperas, ambos com `glFinish()`.

| Fase, 400 shapes próprios/VBO | CPU baseline (ms) | CPU anterior (ms) | CPU lote (ms) |
| --- | ---: | ---: | ---: |
| Primeiro render | 220.106 | 227.880 | 221.770 |
| Estável | 93.765 | 83.566 | 83.900 |
| Após invalidação | 105.453 | 109.284 | 102.747 |
| Reestabilizado | 92.990 | 78.298 | 78.278 |

O custo residual reproduziu +3,6% de CPU na versão anterior (a medição anterior
indicava +3,9%). O lote ficou **2,6% abaixo da baseline** e 6,0% abaixo da versão
anterior na invalidação. Todas as seis amostras de CPU do lote ficaram abaixo
das seis amostras anteriores. Em wall, invalidação foi 83,089 / 85,468 / 79,211 ms
(baseline / anterior / lote). Os ganhos estável e reestabilizado frente à
baseline permanecem em 10,5% e 15,8% de CPU. A diferença estável entre anterior
e lote é +0,4% de CPU; o custo reestabilizado ficou equivalente.

RSS anterior/lote: estável 251,867 / 251,903 MiB, invalidado 280,228 / 280,158 MiB,
reestabilizado 280,919 / 280,928 MiB; pico 282,140 / 282,067 MiB. A economia da
etapa anterior foi preservada. Após liberar cena/contextos, RSS variou de
262,760 para 267,339 MiB. RSS inclui driver/alocador e não mede cache vivo ou
VRAM; não houve trimming manual. A memória estável ainda excede a baseline.

Controles com três repetições: sem VBO, CPU invalidada baseline/anterior/lote
foi 91,958 / 92,818 / 86,832 ms. A melhora também aparece sem atualização GL.
Na fonte compartilhada, sem cache de UV, CPU estável anterior/lote foi
110,806 / 110,728 ms. A segunda rodada completa anterior/lote confirmou a
queda na invalidação própria: 110,523 / 102,783 ms de CPU. Nas malhas únicas,
tempo estável anterior/lote foi 2,239 / 2,374 ms (10k), 11,247 / 11,269 (40k),
22,651 / 22,120 (160k); os controles explícito e sem textura também variaram
(12,018 / 11,278 e 11,337 / 11,757 ms). Não se conclui melhora geral de malhas
únicas, nem comportamento idêntico em GPU física.

Validação final: 131/131 Debug, 131/131 Release, 118/118 renderer OFF sem DISPLAY,
225/225 na composição #901/#902; sem skips. A composição mantém `coin_bundle_ratio`
da #901 no normalizador compartilhado e passou também os 96 quadros extremos.
Os arrays S/T reais das oito cenas coincidiram bit a bit entre baseline,
anterior e lote; na composição, nove cenas, incluindo `wide-bounds`, coincidiram
entre anterior e lote. R/Q da baseline 4D foram conferidos como 0/1.
GDB confirma as mesmas gerações, orçamento, uploads e reaproveitamento.

ABI Debug contra master+#833, sem suppressions: zero funções removidas/alteradas.
Release: zero símbolos removidos tanto da baseline como da versão anterior;
somente o método novo foi acrescentado frente à versão anterior. Consumidor
dos headers antigos passou em Debug e Release. SoShapeP, SoVBO, bundle e guard
antigo mantêm seus campos/layouts. Tudo local em
`/tmp/coin-cache-invalidation-20261120/`: `benchmark.json`, `benchmark-full.json`,
`values*.py`, dumps, logs de validação, saída ABI integral e `RESULTS.txt`.

Para repetir a comparação de bits, com bibliotecas Debug e seus símbolos de
parâmetros, usando o mesmo executável e cenas em ambos os lados:

```sh
COIN_GLXGLUE_NO_PBUFFERS=1 xvfb-run -a sh ./verify-cache-values.sh \
  /path/to/debug/bin/CoinTextureCacheTest /path/to/before/lib /path/to/after/lib
```

O verificador exige 16 draws por cena, 32 na cena compartilhada, compara os
bytes S/T completos e rejeita R/Q diferentes de 0/1 na representação 4D da
baseline. Para a composição, `COIN_CACHE_SOURCES=wide-bounds` seleciona o caso
extremo com as duas bibliotecas compostas, anterior e posterior ao lote.

## Dependências de caches ancestrais — 2026-11-20

`SoCacheElement::addCacheDependency()` agora percorre toda a pilha, propagando
as dependências a cada cache aberto mesmo com níveis vazios entre eles.
A interrupção no primeiro nível vazio já existia na baseline master+#833;
o reaproveitamento dos bounds pelo cache tornava essa falha uma regressão do
mapeamento, pois deixava o cache ancestral válido após mudar o estado herdado.

As novas regressões `CacheAncestorDependencies` (CPU, inclusive renderer OFF)
e `DefaultTextureCacheAncestorDependencies` (GL, dois contextos) falham com o
cache anterior `f33965ff0ea` e passam com `d8173489e12`. A regressão GL também
passa na baseline `44025052aea`, que recalcula os bounds. Os controles exigem
validade antes da alteração e ao reaplicar o mesmo valor; mudar a complexidade
invalida ambos os ancestrais e produz as novas UVs esperadas. O diagnóstico
confirma que os bounds continuam sendo reutilizados na versão corrigida.
Veja também [cache-dependencies](../../cache-dependencies/README.md).

Validação: **133/133 Debug, 133/133 Release, 119/119 renderer OFF sem DISPLAY
e 227/227 na composição #901/#902**, sem skips. A regressão GL passou também
na Radeon física. Permanecem os controles GDB de gerações, uploads, orçamento
e reaproveitamento; a composição passou novamente os 96 quadros extremos.
Todos os novos builds e temporários de compilação ficaram em RAM.

ABI Debug, sem suppressions: nenhuma diferença entre o cache anterior e o
corrigido (`abidiff --no-default-suppression --no-show-locs`, saída vazia,
status 0). Contra master+#833: zero funções removidas/alteradas. Os símbolos
dinâmicos Release são idênticos aos do cache anterior, e o consumidor dos
headers antigos passou em Debug e Release. Nenhum campo ou layout foi alterado.

Benchmark Release atualizado, mesmo executável, 400 shapes próprios com VBO,
4096 pontos por shape, dez quadros estáveis e seis repetições nas seis ordens
das três bibliotecas. Sem builds/testes concorrentes; carga inicial/final
1,01 / 1,43. Medianas de CPU do processo, em ms:

| Driver / fase | Baseline | Cache anterior | Corrigido |
| --- | ---: | ---: | ---: |
| Radeon / estável | 21,847 | 2,557 | 2,623 |
| Radeon / após invalidação | 26,109 | 15,147 | 14,900 |
| Radeon / reestabilizado | 21,722 | 2,606 | 2,713 |
| llvmpipe / estável | 91,784 | 83,127 | 83,000 |
| llvmpipe / após invalidação | 102,643 | 101,632 | 100,886 |
| llvmpipe / reestabilizado | 91,193 | 77,048 | 76,086 |

Wall após invalidação baseline/anterior/corrigido: Radeon 27,813 / 16,195 /
16,855 ms; llvmpipe 80,778 / 75,873 / 76,208 ms. A diferença de wall frente ao
cache anterior varia entre rodadas; não se atribui ganho de tempo a esta
correção. RSS estável anterior/corrigido: Radeon 130,674 / 130,736 MiB,
llvmpipe 252,036 / 252,004 MiB. Os ganhos do cache frente à baseline permanecem,
assim como seu consumo adicional de memória. Os hashes das imagens coincidem
entre as três bibliotecas em cada driver, antes e depois da edição.

Evidências locais: `/tmp/coin-cache-dependency-fix-20261120/RESULTS.txt`,
`benchmark-threeway.json`, scripts e logs completos. Nenhuma publicação foi feita.
