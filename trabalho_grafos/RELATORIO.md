# Relatório — Algoritmos sobre Grafos (INF1010)

## 0. O grafo de entrada

Grafo **não dirigido e ponderado** G = (V, E, p) com 9 vértices (0 a 8) e 14 arestas:

| Aresta | Peso | Aresta | Peso |
|---|---|---|---|
| 0–1 | 4 | 3–4 | 9 |
| 0–7 | 8 | 3–5 | 14 |
| 1–2 | 8 | 4–5 | 10 |
| 1–7 | 11 | 5–6 | 2 |
| 2–3 | 7 | 6–7 | 1 |
| 2–5 | 4 | 6–8 | 6 |
| 2–8 | 2 | 7–8 | 7 |

Convenção usada no código: comentários `[PDF ...]` marcam trechos vindos dos slides
(arquivo + slide) e `[ADICIONADO]` marca o que foi escrito por nós.

---

## 1. Representação do grafo

### 1.1 Matriz de adjacências

Definição do PDF 10 (slide 20): `mat[i][j] = 1` se há aresta entre i e j, `0` caso contrário.
Como o grafo é não dirigido, a matriz é **simétrica** (slide 21).

```
       0  1  2  3  4  5  6  7  8
  0 |  0  1  0  0  0  0  0  1  0
  1 |  1  0  1  0  0  0  0  1  0
  2 |  0  1  0  1  0  1  0  0  1
  3 |  0  0  1  0  1  1  0  0  0
  4 |  0  0  0  1  0  1  0  0  0
  5 |  0  0  1  1  1  0  1  0  0
  6 |  0  0  0  0  0  1  0  1  1
  7 |  1  1  0  0  0  0  1  0  1
  8 |  0  0  1  0  0  0  1  1  0
```

### 1.2 Listas de adjacências

Usamos exatamente a estrutura do lab (PDF 10, slide 26): `struct _grafo` com `nv`, `na` e
`Viz** viz`, onde cada `Viz` guarda `noj` (vizinho), `peso` e `prox`. Cada aresta {i, j}
aparece na lista de i e na lista de j.

```
  [0]-> (1, 4) -> (7, 8) -> NULL
  [1]-> (0, 4) -> (2, 8) -> (7,11) -> NULL
  [2]-> (1, 8) -> (3, 7) -> (5, 4) -> (8, 2) -> NULL
  [3]-> (2, 7) -> (4, 9) -> (5,14) -> NULL
  [4]-> (3, 9) -> (5,10) -> NULL
  [5]-> (2, 4) -> (3,14) -> (4,10) -> (6, 2) -> NULL
  [6]-> (5, 2) -> (7, 1) -> (8, 6) -> NULL
  [7]-> (0, 8) -> (1,11) -> (6, 1) -> (8, 7) -> NULL
  [8]-> (2, 2) -> (6, 6) -> (7, 7) -> NULL
```

---

## 2. Algoritmo de Kruskal

### 2.1 Descrição

Encontra a **árvore geradora de custo mínimo**: subgrafo sem ciclos que contém todos os
vértices e cuja soma dos pesos é mínima (PDF 10, slide 37). Passos (PDF 10, slide 38):

1. Considere cada nó em V como uma árvore separada (formando uma floresta).
2. Examine a aresta de menor custo. Se ela unir duas árvores na floresta, inclua-a.
3. Repita o passo 2 até todos os nós estarem conectados.

Implementação:

- As arestas são extraídas das listas de adjacências e **ordenadas por peso** (`qsort`) —
  assim "examinar a aresta de menor custo" é só percorrer o vetor ordenado.
- A floresta é uma **partição dinâmica de conjuntos / Union-Find** (PDF 9, slide 55):
  - representação por vetor, raiz com `-1` (slides 14–15);
  - `Find` com **compressão de trajetória** (slide 35);
  - `Union` "seguro" (faz os Finds internamente, slide 18) com **união por altura/posto** (slides 23–30).
- Uma aresta {u, v} une duas árvores se `Find(u) != Find(v)`; caso contrário, ela formaria um **ciclo** e é rejeitada.
- O algoritmo para quando tem `nv − 1 = 8` arestas.

### 2.2 Saída gerada

```
2) KRUSKAL - ARVORE GERADORA MINIMA
  6 - 7  (peso 1)
  2 - 8  (peso 2)
  5 - 6  (peso 2)
  0 - 1  (peso 4)
  2 - 5  (peso 4)
  2 - 3  (peso 7)
  0 - 7  (peso 8)
  3 - 4  (peso 9)
  Custo total = 37
```

### 2.3 Caminho percorrido, passo a passo

O programa imprime só o resultado; abaixo está a execução do algoritmo detalhada, aresta por aresta.

Floresta inicial: {0} {1} {2} {3} {4} {5} {6} {7} {8}.

| # | Aresta (peso) | Find | Decisão | Floresta depois |
|---|---|---|---|---|
| 1 | 6–7 (1) | 6 ≠ 7 | inclui | {6,7} {0} {1} {2} {3} {4} {5} {8} |
| 2 | 2–8 (2) | 2 ≠ 8 | inclui | {6,7} {2,8} {0} {1} {3} {4} {5} |
| 3 | 5–6 (2) | 5 ≠ 6 | inclui | {5,6,7} {2,8} {0} {1} {3} {4} |
| 4 | 0–1 (4) | 0 ≠ 1 | inclui | {5,6,7} {2,8} {0,1} {3} {4} |
| 5 | 2–5 (4) | 2 ≠ 6 | inclui | {2,5,6,7,8} {0,1} {3} {4} |
| 6 | 6–8 (6) | 2 = 2 | **rejeita** (6 e 8 já ligados por 6–5–2–8) | — |
| 7 | 2–3 (7) | 2 ≠ 3 | inclui | {2,3,5,6,7,8} {0,1} {4} |
| 8 | 7–8 (7) | 2 = 2 | **rejeita** (ciclo 7–6–5–2–8) | — |
| 9 | 0–7 (8) | 0 ≠ 2 | inclui | {0,1,2,3,5,6,7,8} {4} |
| 10 | 1–2 (8) | 2 = 2 | **rejeita** (ciclo 1–0–7–6–5–2) | — |
| 11 | 3–4 (9) | 2 ≠ 4 | inclui | {0,1,2,3,4,5,6,7,8} → **fim** (8 arestas) |

Observações:

- **Empates de peso**: 2–8 e 5–6 (peso 2), 0–1 e 2–5 (peso 4), 2–3 e 7–8 (peso 7), 0–7 e 1–2 (peso 8).
  Ordenamos por (peso, u, v) para a saída ser sempre a mesma. Outra ordem de desempate poderia
  produzir outra árvore (por exemplo, com 1–2 no lugar de 0–7), mas **o custo mínimo é sempre 37**.
- Uma aresta é rejeitada quando `Find(u) == Find(v)`, ou seja, u e v já estão na mesma árvore e incluí-la formaria um ciclo.

Árvore final:

```
 1 —4— 0 —8— 7 —1— 6 —2— 5 —4— 2 —2— 8
                                |
                                7
                                |
                                3 —9— 4
```

(arestas: 1–0, 0–7, 7–6, 6–5, 5–2, 2–8, 2–3, 3–4; o "7" vertical é o peso da aresta 2–3)

---

## 3. Algoritmo de Dijkstra (origem = nó 8)

### 3.1 Descrição

Calcula a **menor distância** do nó de origem a cada nó do grafo (PDF 11, slide 6).
Passos (PDF 11, slides 19–20), que estão como comentários no código:

1. Defina o nó de origem.
2. Atribua distância 0 à origem e infinito a todos os outros nós.
3. Marque todos como não visitados e a origem como nó corrente (A).
4. Para cada vizinho B **não visitado** de A, calcule `dist[A] + peso(A,B)`; se for menor que
   `dist[B]`, sobrescreva `dist[B]`.
5. Marque A como visitado — sua distância é final e mínima.
6. Se todos foram visitados, termine; senão, o próximo corrente é o não visitado de menor distância. Volte ao passo 4.

Implementação:

- Usa as listas de adjacências e um vetor `dist[]`, como nos slides 22–30 do PDF 11.
- O conjunto de visitados é um **vetor de bits** (`BitVector`, copiado do PDF EDA_07_Conjuntos, slides 15–17).
- Foi adicionado o vetor `ant[]` (anterior de cada nó no menor caminho) para conseguir **mostrar o caminho**, não só a distância.
- O nó corrente seguinte é escolhido por busca linear, então a complexidade é O(V²) (slide 21: depende da implementação de Q).
- Empate de distâncias: escolhe o de menor índice.

### 3.2 Saída gerada

Para cada nó: menor distância a partir do nó 8 e, entre parênteses, o caminho correspondente.

```
3) DIJKSTRA - MENOR DISTANCIA DO NO 8 AOS DEMAIS
  8 -> 0 : 14   (8 -> 2 -> 1 -> 0)
  8 -> 1 : 10   (8 -> 2 -> 1)
  8 -> 2 :  2   (8 -> 2)
  8 -> 3 :  9   (8 -> 2 -> 3)
  8 -> 4 : 16   (8 -> 2 -> 5 -> 4)
  8 -> 5 :  6   (8 -> 2 -> 5)
  8 -> 6 :  6   (8 -> 6)
  8 -> 7 :  7   (8 -> 7)
```

### 3.3 Caminho percorrido, passo a passo

O programa imprime só o resultado; abaixo está a execução do algoritmo detalhada, nó por nó.

Início: `dist = [∞ ∞ ∞ ∞ ∞ ∞ ∞ ∞ 0]` (índices 0..8).

| Passo | Corrente | O que acontece com os vizinhos | dist[0..8] depois |
|---|---|---|---|
| 1 | **8** (0) | 2: ∞→2; 6: ∞→6; 7: ∞→7 | ∞ ∞ 2 ∞ ∞ ∞ 6 7 0 |
| 2 | **2** (2) | 1: ∞→10; 3: ∞→9; 5: ∞→6; ignora 8 | ∞ 10 2 9 ∞ 6 6 7 0 |
| 3 | **5** (6) | ignora 2; 3 mantém 9 (6+14=20); 4: ∞→16; 6 mantém 6 (6+2=8) | ∞ 10 2 9 16 6 6 7 0 |
| 4 | **6** (6) | ignora 5; 7 mantém 7 (6+1=7, não é menor); ignora 8 | ∞ 10 2 9 16 6 6 7 0 |
| 5 | **7** (7) | 0: ∞→15; 1 mantém 10 (7+11=18); ignora 6, 8 | 15 10 2 9 16 6 6 7 0 |
| 6 | **3** (9) | ignora 2; 4 mantém 16 (9+9=18); ignora 5 | 15 10 2 9 16 6 6 7 0 |
| 7 | **1** (10) | 0: 15→**14** (10+4); ignora 2, 7 | 14 10 2 9 16 6 6 7 0 |
| 8 | **0** (14) | todos os vizinhos já visitados | 14 10 2 9 16 6 6 7 0 |
| 9 | **4** (16) | todos os vizinhos já visitados | 14 10 2 9 16 6 6 7 0 |

Não há mais vértices não visitados — FIM.

Pontos que merecem destaque:

- **Passo 3**: os nós 5 e 6 tinham a mesma distância (6); foi escolhido o 5 (menor índice). A ordem não muda o resultado.
- **Passo 4**: 8→6→7 também custa 7, igual a 8→7 direto. Como só sobrescrevemos quando a nova distância é **estritamente menor**, o caminho registrado para 7 continua sendo o direto 8→7.
- **Passo 5 → 7**: o nó 0 recebeu primeiro 15 (via 8→7→0) e depois foi corrigido para **14** (via 8→2→1→0). Isso mostra por que a distância só vira definitiva quando o nó é **visitado**, não quando é alcançado pela primeira vez.
- O nó 4 ficou com 16 por 8→2→5→4 (2+4+10); a alternativa 8→2→3→4 custaria 18.

---

## 4. O que funciona e o que não funciona

**Funciona:**

- Montagem do grafo do enunciado com a struct de listas de adjacências do lab, e impressão da matriz e das listas.
- Kruskal com Union-Find (união por altura + compressão de trajetória): gera a árvore de custo **37** com as 8 arestas corretas.
- Dijkstra a partir do nó 8: distâncias corretas para todos os nós e o caminho até cada um.
- Conferido à mão (tabelas acima) e compilado sem avisos com `-Wall -Wextra`; rodado sem erros de memória com AddressSanitizer.

**Limitações / o que não funciona:**

- O grafo é **fixo no código** (`main.c`); não há leitura de arquivo nem entrada pelo teclado.
- O Dijkstra assume **pesos não negativos** (limitação do próprio algoritmo).
- O Dijkstra escolhe o próximo nó por busca linear, O(V²); não usa fila de prioridade (heap). Para este grafo pequeno isso não importa.
- O programa foi feito para o grafo do enunciado, que é conexo. Se o grafo fosse desconexo, o Kruskal produziria uma floresta (menos de nv−1 arestas) e os nós não alcançados pelo Dijkstra ficariam com distância "infinita", sem tratamento especial na saída.
- Em caso de empate de pesos, o Kruskal pode gerar outra árvore com o mesmo custo, dependendo da ordem de desempate.
- O `BitVector` foi copiado como está no slide, que usa `sizeof(int)` (4) como número de bits por `int`. Funciona, pois a mesma conta é usada em todas as funções, mas só aproveita 4 dos 32 bits de cada `int`.
