/* =====================================================================
 * algoritmos.c - Kruskal (arvore geradora minima) e Dijkstra (menores
 *                distancias a partir de um vertice)
 * Legenda: [PDF ...] = trecho vindo dos slides | [ADICIONADO] = novo
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "algoritmos.h"
#include "particao.h"
#include "bitvet.h"

/* [ADICIONADO] valor usado como "infinito" (simbolo ∞ dos slides) */
#define INFINITO FLT_MAX

/* =====================================================================
 *                          ALGORITMO DE KRUSKAL
 * [PDF 10, slide 38]
 *   Entrada: Um grafo ponderado G = (V,E,p)
 *   Saida: Arvore geradora de custo minimo
 *   1. Considere cada no em V como uma arvore separada (floresta)
 *   2. Examine a aresta de menor custo.
 *      Se ela unir duas arvores na floresta, inclua-a.
 *   3. Repita o Passo (2) ate todos os nos estarem conectados.
 * [PDF 9, slide 55] "O de Kruskal pode ser obtido atraves de Union-Find.
 *   ... seleciona as arestas de custo minimo no grafo, gerando florestas
 *   que sao ligadas via union-find."
 * Os slides trazem o algoritmo em passos; o codigo C e [ADICIONADO].
 * ===================================================================== */

/* [ADICIONADO] comparacao para o qsort: menor peso primeiro.
 * Em caso de empate, ordena por (u,v) para a saida ser sempre a mesma. */
static int comparaArestas(const void* a, const void* b) {
   const Aresta* x = (const Aresta*)a;
   const Aresta* y = (const Aresta*)b;
   if (x->peso < y->peso) return -1;
   if (x->peso > y->peso) return 1;
   if (x->u != y->u) return x->u - y->u;
   return x->v - y->v;
}

int kruskal(Grafo* g, Aresta* arvore, float* custo) {
   int i, k = 0, na = 0, incluidas = 0;
   Viz* p;
   Aresta* arestas;
   Particao* floresta;

   /* [ADICIONADO] Monta o vetor E de arestas a partir das listas de
    * adjacencias. Como cada aresta {i,j} aparece nas duas listas, so
    * pegamos quando i < j para nao repetir. */
   arestas = (Aresta*)malloc(g->na * sizeof(Aresta));
   for (i = 0; i < g->nv; i++)
      for (p = g->viz[i]; p != NULL; p = p->prox)
         if (i < p->noj) {
            arestas[na].u = i;
            arestas[na].v = p->noj;
            arestas[na].peso = p->peso;
            na++;
         }

   /* [ADICIONADO] Ordena as arestas para que "examinar a aresta de menor
    * custo" (passo 2) seja simplesmente percorrer o vetor em ordem. */
   qsort(arestas, na, sizeof(Aresta), comparaArestas);

   printf("  Arestas ordenadas por peso:\n   ");
   for (i = 0; i < na; i++)
      printf(" %d-%d(%.0f)", arestas[i].u, arestas[i].v, arestas[i].peso);
   printf("\n\n");

   /* [PDF 10, slide 38, passo 1] cada no e uma arvore separada.
    * Implementado com MakeSet do Union-Find [PDF 9, slides 5 e 9]. */
   floresta = partCria(g->nv);
   printf("  Passo 1: floresta inicial = %d arvores {0} {1} ... {%d}\n\n",
          g->nv, g->nv - 1);

   *custo = 0;
   /* [PDF 10, slide 38, passos 2 e 3] examina a aresta de menor custo e,
    * se unir duas arvores (Find diferentes), inclui (Union).
    * Repete ate todos conectados = nv-1 arestas incluidas. */
   for (k = 0; k < na && incluidas < g->nv - 1; k++) {
      int u = arestas[k].u, v = arestas[k].v;
      int ru = partFind(floresta, u);   /* [PDF 9] Find */
      int rv = partFind(floresta, v);
      printf("  Examina %d-%d (peso %2.0f): Find(%d)=%d, Find(%d)=%d -> ",
             u, v, arestas[k].peso, u, ru, v, rv);
      if (partUnion(floresta, u, v)) {  /* [PDF 9] Union */
         arvore[incluidas++] = arestas[k];
         *custo += arestas[k].peso;
         printf("INCLUI   (une duas arvores)   custo parcial = %.0f\n", *custo);
      } else {
         printf("REJEITA  (formaria ciclo)\n");
      }
   }
   /* [ADICIONADO] arestas que nem precisaram ser examinadas */
   for (; k < na; k++)
      printf("  %d-%d (peso %2.0f): nao examinada (arvore ja completa)\n",
             arestas[k].u, arestas[k].v, arestas[k].peso);

   printf("\n  Vetor de pais da particao ao final (PDF 9, slide 15):\n");
   partMostra(floresta);

   partLibera(floresta);
   free(arestas);
   return incluidas;
}

/* =====================================================================
 *                          ALGORITMO DE DIJKSTRA
 * [PDF 11, slides 19-20] - os passos abaixo estao copiados do slide e
 * aparecem como comentario junto do codigo que os implementa.
 * O codigo C e [ADICIONADO] (os slides nao trazem codigo).
 * A estrutura usada e a dos slides 22-30 do PDF 11: vetor dist[] ao lado
 * das listas de adjacencias (vertex, weight, link).
 * ===================================================================== */

/* [ADICIONADO] imprime uma distancia (∞ quando infinita) */
static void mostraDist(float d) {
   if (d == INFINITO) printf("  inf");
   else               printf("%5.0f", d);
}

/* [ADICIONADO] imprime a tabela dist[] como nas figuras dos slides 7-18 */
static void mostraTabela(float* dist, BitVector* visitados, int nv) {
   int i;
   printf("      vertice:");
   for (i = 0; i < nv; i++) printf("%5d", i);
   printf("\n      dist   :");
   for (i = 0; i < nv; i++) mostraDist(dist[i]);
   printf("\n      visit. :");
   for (i = 0; i < nv; i++) printf("%5s", bvGet(visitados, i) ? "x" : ".");
   printf("\n");
}

void dijkstra(Grafo* g, int origem, float* dist, int* ant) {
   int i, corrente, passo = 1;
   Viz* p;
   /* [PDF EDA_07_Conjuntos, slides 15-17] conjunto de visitados como
    * vetor de bits: bvSet = inserir, bvGet = pertence? */
   BitVector* visitados = bvInit(g->nv);

   /* 1. Defina o no de origem  (parametro "origem")
    * 2. Atribua a todos os nos um valor de distancia ao no de origem:
    *    valor zero ao no de origem, e infinito para todos os outros nos. */
   for (i = 0; i < g->nv; i++) {
      dist[i] = INFINITO;
      ant[i] = -1;            /* [ADICIONADO] para reconstruir o caminho */
   }
   dist[origem] = 0;

   /* 3. Marque todos os demais nos como nao visitados e o no origem como
    *    corrente (A). (bvInit ja deixou todos os bits em 0 = nao visitado) */
   corrente = origem;

   printf("  Inicio: origem = %d\n", origem);
   mostraTabela(dist, visitados, g->nv);

   while (corrente != -1) {
      printf("\n  Passo %d: no corrente = %d (dist = %.0f)\n",
             passo++, corrente, dist[corrente]);

      /* 4. Considere a distancia de todos os nos vizinhos nao visitados ao
       *    no corrente e calcule uma distancia deles ao no origem atraves
       *    do no corrente.
       *    4.1 se o no atual A tiver distancia 6 e houver uma aresta de peso
       *        2 conectando-o com B, a distancia de B atraves de A sera 8.
       *    4.2 Se essa distancia for menor do que a registrada
       *        anteriormente, sobrescreva a distancia de B. */
      for (p = g->viz[corrente]; p != NULL; p = p->prox) {
         int b = p->noj;
         float nova;
         if (bvGet(visitados, b)) {
            printf("    ignora vertice %d (ja visitado)\n", b);
            continue;
         }
         nova = dist[corrente] + p->peso;                         /* 4.1 */
         if (nova < dist[b]) {                                    /* 4.2 */
            printf("    modifica distancia do vertice %d: ", b);
            mostraDist(dist[b]);
            printf(" -> %.0f  (%.0f + %.0f, via %d)\n",
                   nova, dist[corrente], p->peso, corrente);
            dist[b] = nova;
            ant[b] = corrente;            /* [ADICIONADO] guarda o caminho */
         } else {
            printf("    mantem distancia do vertice %d: %.0f <= %.0f + %.0f\n",
                   b, dist[b], dist[corrente], p->peso);
         }
      }

      /* 5. Ao terminar de considerar todos os vizinhos do no atual A,
       *    marque-o como visitado. Um no visitado nao sera mais verificado;
       *    sua distancia registrada agora e final e minima. */
      bvSet(visitados, corrente);
      mostraTabela(dist, visitados, g->nv);

      /* 6. Se todos os nos tiverem sido visitados, termine.
       *    Caso contrario, marque o no nao visitado com a menor distancia
       *    (ao no de origem) como o proximo "no corrente", e repita a
       *    partir do passo 4.
       *    [ADICIONADO] O conjunto Q de nao visitados e percorrido de forma
       *    linear (PDF 11, slide 21: a complexidade depende de Q) => O(V^2).
       *    Se so restarem nos com distancia infinita (grafo desconexo),
       *    tambem termina. */
      corrente = -1;
      for (i = 0; i < g->nv; i++)
         if (!bvGet(visitados, i) && dist[i] != INFINITO &&
             (corrente == -1 || dist[i] < dist[corrente]))
            corrente = i;
   }
   printf("\n  Nao ha mais vertices nao visitados - FIM!\n");
   bvShow(visitados, "  Conjunto de visitados = ");
   bvFree(visitados);
}

/* [ADICIONADO] Reconstroi o caminho seguindo ant[] do destino ate a
 * origem (recursivamente, para imprimir na ordem origem -> destino). */
void mostraCaminho(int* ant, int destino) {
   if (ant[destino] != -1) {
      mostraCaminho(ant, ant[destino]);
      printf(" -> ");
   }
   printf("%d", destino);
}
