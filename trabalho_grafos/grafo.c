/* =====================================================================
 * grafo.c - Operacoes sobre o grafo (listas e matriz de adjacencias)
 * Legenda: [PDF ...] = trecho vindo dos slides | [ADICIONADO] = novo
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

/* [ADICIONADO] Cria um grafo com nv vertices e nenhuma aresta.
 * Usa a struct _grafo do slide 26 (PDF 10): viz[i] = NULL (lista vazia). */
Grafo* grafoCria(int nv) {
   int i;
   Grafo* g = (Grafo*)malloc(sizeof(Grafo));
   if (g == NULL) { printf("Memoria insuficiente\n"); exit(1); }
   g->nv = nv;
   g->na = 0;
   g->viz = (Viz**)malloc(nv * sizeof(Viz*));
   if (g->viz == NULL) { printf("Memoria insuficiente\n"); exit(1); }
   for (i = 0; i < nv; i++)
      g->viz[i] = NULL;
   return g;
}

/* [ADICIONADO] Cria um no da lista de vizinhos (struct _viz do slide 26) */
static Viz* criaViz(int noj, float peso) {
   Viz* v = (Viz*)malloc(sizeof(Viz));
   if (v == NULL) { printf("Memoria insuficiente\n"); exit(1); }
   v->noj = noj;
   v->peso = peso;
   v->prox = NULL;
   return v;
}

/* [ADICIONADO] Insere v no FINAL da lista (assim a lista fica na mesma
 * ordem em que as arestas foram informadas, facilitando a leitura). */
static void insereNoFinal(Viz** lista, Viz* v) {
   Viz* p;
   if (*lista == NULL) { *lista = v; return; }
   for (p = *lista; p->prox != NULL; p = p->prox)
      ;
   p->prox = v;
}

/* [ADICIONADO] Insere a aresta {a,b} com peso.
 * Como o grafo do enunciado e NAO dirigido, a aresta {a,b} e um conjunto
 * de 2 vertices (PDF 10, slide 4), entao ela aparece na lista de a e na
 * lista de b (como nas listas de adjacencias do PDF 10, slides 24-25). */
void grafoInsereAresta(Grafo* g, int a, int b, float peso) {
   insereNoFinal(&g->viz[a], criaViz(b, peso));
   insereNoFinal(&g->viz[b], criaViz(a, peso));
   g->na++;
}

/* [ADICIONADO] Imprime as listas de adjacencias no formato
 * [i]-> (vizinho, peso) -> ... -> NULL, inspirado no desenho
 * "[0]-> 1 28 -> 5 10 -> •" do PDF 11 (slides 22-30). */
void grafoMostraListas(Grafo* g) {
   int i;
   Viz* p;
   for (i = 0; i < g->nv; i++) {
      printf("  [%d]->", i);
      for (p = g->viz[i]; p != NULL; p = p->prox)
         printf(" (%d, %2.0f) ->", p->noj, p->peso);
      printf(" NULL\n");
   }
}

/* [PDF 10, slides 20-23 "Matriz de adjacencias"]
 *     mat[i][j] = 1, se houver uma aresta do no i para o no j
 *                 0, caso contrario
 * A definicao e do slide; o codigo que monta a matriz a partir das
 * listas de adjacencias e [ADICIONADO]. */
void grafoMostraMatriz(Grafo* g) {
   int i, j;
   Viz* p;
   /* [ADICIONADO] alocacao e preenchimento da matriz */
   int** mat = (int**)malloc(g->nv * sizeof(int*));
   for (i = 0; i < g->nv; i++) {
      mat[i] = (int*)calloc(g->nv, sizeof(int)); /* tudo 0 */
      for (p = g->viz[i]; p != NULL; p = p->prox)
         mat[i][p->noj] = 1;                      /* existe aresta i-j */
   }
   /* [ADICIONADO] impressao */
   printf("     ");
   for (j = 0; j < g->nv; j++) printf("%3d", j);
   printf("\n");
   for (i = 0; i < g->nv; i++) {
      printf("  %d |", i);
      for (j = 0; j < g->nv; j++) printf("%3d", mat[i][j]);
      printf("\n");
      free(mat[i]);
   }
   free(mat);
}

/* [ADICIONADO] Variante da matriz para grafo PONDERADO G=(V,E,p)
 * (PDF 10, slide 7): em vez de 1 guarda o peso p da aresta; 0 = sem aresta. */
void grafoMostraMatrizPesos(Grafo* g) {
   int i, j;
   Viz* p;
   float** mat = (float**)malloc(g->nv * sizeof(float*));
   for (i = 0; i < g->nv; i++) {
      mat[i] = (float*)calloc(g->nv, sizeof(float));
      for (p = g->viz[i]; p != NULL; p = p->prox)
         mat[i][p->noj] = p->peso;
   }
   printf("     ");
   for (j = 0; j < g->nv; j++) printf("%3d", j);
   printf("\n");
   for (i = 0; i < g->nv; i++) {
      printf("  %d |", i);
      for (j = 0; j < g->nv; j++) printf("%3.0f", mat[i][j]);
      printf("\n");
      free(mat[i]);
   }
   free(mat);
}

/* [ADICIONADO] Libera toda a memoria do grafo */
void grafoLibera(Grafo* g) {
   int i;
   Viz *p, *t;
   for (i = 0; i < g->nv; i++) {
      p = g->viz[i];
      while (p != NULL) { t = p->prox; free(p); p = t; }
   }
   free(g->viz);
   free(g);
}
