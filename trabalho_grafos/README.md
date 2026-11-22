# Trabalho de Grafos — INF1010 (Estruturas de Dados Avançadas)

1. Representação do grafo do enunciado como **matriz de adjacências** e como **lista de adjacências**
2. **Kruskal**: árvore geradora de custo mínimo
3. **Dijkstra**: menor distância entre o nó **8** e os demais nós

O relatório está em [`RELATORIO.md`](RELATORIO.md) e a saída completa do programa em [`saida.txt`](saida.txt).

## Arquivos

| Arquivo | Conteúdo | Origem |
|---|---|---|
| `grafo.h` / `grafo.c` | struct do grafo (listas de adjacências), matriz de adjacências | struct copiada do PDF 10, slide 26 |
| `bitvet.h` / `bitvet.c` | conjunto como vetor de bits (visitados do Dijkstra) | copiado do PDF EDA_07_Conjuntos, slides 15–17 |
| `particao.h` / `particao.c` | Union-Find (MakeSet / Find / Union) | regras do PDF 9 (slides 5, 14–15, 18, 23–30, 35) |
| `algoritmos.h` / `algoritmos.c` | Kruskal e Dijkstra | passos do PDF 10 (slide 38) e PDF 11 (slides 19–20) |
| `main.c` | monta o grafo do enunciado e executa tudo | adicionado |

Nos códigos, os comentários `[PDF ...]` indicam trechos vindos dos slides
(arquivo + número do slide) e `[ADICIONADO]` indica o que foi escrito para
atingir o objetivo do enunciado.

## Execução

```
make            # ou: gcc -Wall -o grafos main.c grafo.c particao.c bitvet.c algoritmos.c
./grafos
```
