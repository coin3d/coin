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
