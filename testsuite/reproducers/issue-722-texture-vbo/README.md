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
**576 quadros**: seis fontes/cenários, com e sem display lists pais,
três caminhos de renderização, oito fases e dois contextos. A referência
usa UVs explícitas calculadas independentemente do cache. Os cenários
incluem `SoVertexProperty`, coordenadas herdadas, coordenadas homogêneas,
um shape compartilhado entre estados e um bounding box que depende de
um elemento herdado, além de bounds vazios fornecidos por uma extensão.
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
