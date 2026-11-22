/* =====================================================================
 * algoritmos.h - Kruskal e Dijkstra sobre o grafo de listas de adjacencias
 * Legenda: [PDF ...] = trecho vindo dos slides | [ADICIONADO] = novo
 * ===================================================================== */
#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "grafo.h"

/* [ADICIONADO] representa uma aresta {u,v} com peso, usada para ordenar
 * as arestas no Kruskal e para devolver a arvore geradora. */
typedef struct {
   int u, v;
   float peso;
} Aresta;

/* Kruskal [PDF 10, slides 37-47; PDF 9, slides 52-55]
 * Preenche arvore[] (precisa de espaco para nv-1 arestas) e devolve o
 * numero de arestas incluidas; *custo recebe a soma dos pesos.          */
int kruskal(Grafo* g, Aresta* arvore, float* custo);

/* Dijkstra [PDF 11, slides 6-30]
 * dist[i] = menor distancia de origem ate i;
 * ant[i]  = [ADICIONADO] vertice anterior a i no menor caminho (-1 = nenhum) */
void dijkstra(Grafo* g, int origem, float* dist, int* ant);

/* [ADICIONADO] imprime o caminho origem -> ... -> destino usando ant[] */
void mostraCaminho(int* ant, int destino);

#endif
