/* =====================================================================
 * main.c - Trabalho de Grafos (INF1010 - Estruturas de Dados Avancadas)
 *
 *  1) Representa o grafo do enunciado como matriz e listas de adjacencias
 *  2) Kruskal: arvore geradora de custo minimo
 *  3) Dijkstra: menor distancia entre o no 8 e os demais nos
 *
 * Legenda: [PDF ...] = trecho vindo dos slides | [ADICIONADO] = novo
 * ===================================================================== */
#include <stdio.h>
#include <float.h>
#include "grafo.h"
#include "algoritmos.h"

#define NV 9        /* [ADICIONADO] vertices 0..8 do grafo do enunciado */
#define ORIGEM 8    /* [ADICIONADO] origem pedida para o Dijkstra       */

/* [ADICIONADO] arestas do grafo da imagem do enunciado: {u, v, peso} */
static const int ARESTAS[][3] = {
   {0, 1, 4},  {0, 7, 8},
   {1, 2, 8},  {1, 7, 11},
   {2, 3, 7},  {2, 5, 4},  {2, 8, 2},
   {3, 4, 9},  {3, 5, 14},
   {4, 5, 10},
   {5, 6, 2},
   {6, 7, 1},  {6, 8, 6},
   {7, 8, 7}
};

int main(void) {
   int i, n;
   int na = sizeof(ARESTAS) / sizeof(ARESTAS[0]);
   Grafo* g;
   Aresta arvore[NV - 1];
   float custo;
   float dist[NV];
   int ant[NV];

   /* ---------------- 1) Representacao do grafo ---------------- */
   g = grafoCria(NV);
   for (i = 0; i < na; i++)
      grafoInsereAresta(g, ARESTAS[i][0], ARESTAS[i][1], (float)ARESTAS[i][2]);

   printf("==============================================================\n");
   printf(" 1) REPRESENTACAO DO GRAFO  (%d vertices, %d arestas)\n", g->nv, g->na);
   printf("==============================================================\n");
   printf("\nMatriz de adjacencias (1 = ha aresta, 0 = nao ha) [PDF 10, slide 20]:\n");
   grafoMostraMatriz(g);
   printf("\nMatriz de adjacencias com pesos (0 = nao ha aresta):\n");
   grafoMostraMatrizPesos(g);
   printf("\nListas de adjacencias [PDF 10, slide 26] - (vizinho, peso):\n");
   grafoMostraListas(g);

   /* ---------------- 2) Kruskal ---------------- */
   printf("\n==============================================================\n");
   printf(" 2) KRUSKAL - ARVORE GERADORA DE CUSTO MINIMO\n");
   printf("==============================================================\n");
   n = kruskal(g, arvore, &custo);
   printf("\n  Arvore geradora minima (%d arestas):\n", n);
   for (i = 0; i < n; i++)
      printf("    %d - %d  (peso %.0f)\n", arvore[i].u, arvore[i].v, arvore[i].peso);
   printf("  Custo total = %.0f\n", custo);
   if (n != g->nv - 1)
      printf("  ATENCAO: grafo desconexo, foi gerada uma floresta.\n");

   /* ---------------- 3) Dijkstra ---------------- */
   printf("\n==============================================================\n");
   printf(" 3) DIJKSTRA - MENOR DISTANCIA DO NO %d AOS DEMAIS\n", ORIGEM);
   printf("==============================================================\n");
   dijkstra(g, ORIGEM, dist, ant);
   printf("\n  Resultado final:\n");
   for (i = 0; i < NV; i++) {
      printf("    %d -> %d : ", ORIGEM, i);
      if (dist[i] == FLT_MAX) { printf("inalcancavel\n"); continue; }
      printf("distancia %2.0f   caminho: ", dist[i]);
      mostraCaminho(ant, i);
      printf("\n");
   }

   grafoLibera(g);
   return 0;
}
