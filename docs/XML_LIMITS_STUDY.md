# Estudo de limites de recursos do XML do Coin

Base desta branch: `codex/pr/cc-xml-parser-transactional` (`162dc998af31291fb2d861b0d6b1431acb1c18da`). Esta etapa define o que medir e onde impor limites; **não escolhe valores nem modifica o parser**. A guarda `size_t` → `int` já tem contribuição separada (`codex/pr/cc-xml-parse-length-guard`), mas ela cobre apenas o tamanho de uma chamada a `XML_Parse()`, não o total de chunks nem o DOM.

## Superfícies e custo

| Recurso | Entrada/contagem necessária | Risco se medido tarde |
| --- | --- | --- |
| Bytes XML cumulativos | `cc_xml_doc_read_buffer_x`, as duas funções de parsing parcial e cada leitura de `cc_xml_doc_read_file_x` | Muitos chunks individualmente pequenos ultrapassam qualquer guarda por chamada. |
| Bytes de recursos DTD externos | Callback de resolução da branch experimental libxml2; contar soma por documento, inclusive recursos repetidos | Uma cadeia de recursos pode exceder o orçamento embora cada recurso isolado caiba. |
| Texto expandido/atributos | Callbacks Expat de texto e início de elemento; no caminho DTD, também antes da serialização libxml2 → Expat | Entidades e atributos padrão podem multiplicar a entrada. Os bytes de XML de origem não limitam o DOM. |
| Nós e atributos | Antes de `cc_xml_elt_new*` e `cc_xml_attr_new_from_data` nos callbacks | Contar depois da alocação não evita o pico de memória. CDATA também cria nó; coalescência muda a contagem, portanto o contrato precisa especificar se conta eventos ou nós retidos. |
| Profundidade | Antes do `push` em `parsestack` e antes da recursão dos consumidores/serializadores | Pilha do parser e recursão de `cc_xml_elt_calculate_size()`/`cc_xml_elt_write_to_buffer()` podem falhar antes de um limite apenas de bytes/nós. |
| Saída serializada | `cc_xml_doc_calculate_size`, `cc_xml_elt_calculate_size`, escrita em buffer/arquivo | O tamanho calculado pode transbordar `size_t`; `*bytes + 1`, indentação recursiva e buffer completo podem alocar excessivamente. |

No caminho DTD experimental, a libxml2 mantém sua árvore, depois materializa outro buffer UTF-8 e só então Expat cria o DOM Coin. Um limite aplicado apenas ao DOM Coin não cobre esses picos. O contrato de recursos externos é exclusivamente callback; não há autorização para abrir arquivo/rede automaticamente. A validação e a segunda análise precisam compartilhar um orçamento ou verificar limites antes de cada materialização, com custo de memória de pico medido.

## Semântica a fechar antes de valores

1. Definir limites configuráveis por documento, com valor `0` significando “desabilitado” ou com flag explícita. Decidir separadamente se a API legada mantém comportamento ilimitado por compatibilidade e se um perfil seguro opt-in fornece padrões. Não mudar defaults neste estudo.
2. Estabelecer unidade de cada contador: bytes recebidos, bytes UTF-8 expandidos, nós persistentes ou eventos, atributos por elemento e/ou totais, profundidade da raiz como `1`, bytes de saída incluindo terminador ou não. Usar aritmética de subtração para verificar orçamento antes de somar, evitando overflow.
3. Garantir que um erro de limite interrompa o parser, devolva `FALSE` e preserve raiz/filename anteriores pelo rollback transacional; uma nova leitura ou sessão parcial começa com contadores zerados. Especificar como o cliente distingue limite, sintaxe e I/O sem depender de mensagens de debug.
4. Aplicar o mesmo contrato a arquivo, buffer e chunks parciais. No DTD, incluir bytes externos e a representação expandida; verificar que a passagem libxml2 → Expat não relaxa nem duplica indevidamente o orçamento.
5. Decidir se serialização recebe orçamento próprio para DOMs construídos pelo usuário. Ela não pode confiar nos contadores da leitura.

## Evidência inicial e medições pendentes

Os quatro arquivos SCXML versionados em `data/scxml/navigation/` têm 480, 4.123, 13.377 e 16.638 bytes (total 34.618); são exemplos pequenos, **não** uma amostra suficiente para fixar um teto. Medir também documentos reais dos consumidores, casos grandes porém válidos, memória de pico/tempo por superfície e diferenças Expat embarcado/externo e libxml2 opt-in. Não extrapolar valores a partir dessas quatro amostras.

Matriz mínima de regressão para a implementação: exatamente limite e limite+1; excesso no segundo ou último chunk; atributos de comprimento distinto em um mesmo elemento; profundidade exatamente permitida; CDATA fragmentado/coalescido; expansão DTD interna e externa repetida; falha seguida de nova leitura válida e preservação do DOM anterior; filtro que descarta nós; escrita de DOM manual com tamanho/indentação próximos do teto. Repetir em builds normal e ASan/UBSan e no lab fixo e master de destino antes de criar PR.
