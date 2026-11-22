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

   /* [PDF 10, slide 38, passo 1] cada no e uma arvore separada.
    * Implementado com MakeSet do Union-Find [PDF 9, slides 5 e 9]. */
   floresta = partCria(g->nv);

   *custo = 0;
   /* [PDF 10, slide 38, passos 2 e 3] examina a aresta de menor custo e,
    * se unir duas arvores, inclui. partUnion [PDF 9] faz os Finds e
    * devolve 0 quando u e v ja estao na mesma arvore (formaria ciclo).
    * Repete ate todos conectados = nv-1 arestas incluidas. */
   for (k = 0; k < na && incluidas < g->nv - 1; k++) {
      if (partUnion(floresta, arestas[k].u, arestas[k].v)) {
         arvore[incluidas++] = arestas[k];
         *custo += arestas[k].peso;
      }
   }

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

void dijkstra(Grafo* g, int origem, float* dist, int* ant) {
   int i, corrente;
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

   while (corrente != -1) {
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
         if (bvGet(visitados, b))
            continue;                     /* vizinho ja visitado: ignora */
         nova = dist[corrente] + p->peso;                         /* 4.1 */
         if (nova < dist[b]) {                                    /* 4.2 */
            dist[b] = nova;
            ant[b] = corrente;            /* [ADICIONADO] guarda o caminho */
         }
      }

      /* 5. Ao terminar de considerar todos os vizinhos do no atual A,
       *    marque-o como visitado. Um no visitado nao sera mais verificado;
       *    sua distancia registrada agora e final e minima. */
      bvSet(visitados, corrente);

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
