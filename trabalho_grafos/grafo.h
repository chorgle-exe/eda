/* =====================================================================
 * grafo.h - Representacao de grafo por listas de adjacencias
 *
 * LEGENDA DOS COMENTARIOS USADOS EM TODO O TRABALHO:
 *   [PDF ...]     -> trecho copiado/adaptado dos slides da disciplina
 *                    (indica o arquivo e o numero do slide)
 *   [ADICIONADO]  -> trecho que nao existe nos PDFs e foi escrito para
 *                    atingir o objetivo pedido no enunciado
 * ===================================================================== */
#ifndef GRAFO_H
#define GRAFO_H

/* ---------------------------------------------------------------------
 * [PDF "INF1010 - 10 Definicoes e algoritmos sobre grafos", slide 26
 *  "Listas de adjacencias - o que vamos usar no lab"]
 * Copiado literalmente do slide.
 * --------------------------------------------------------------------- */
typedef struct _viz Viz;
struct _viz {
   int noj;
   float peso;
   Viz* prox;
};

struct _grafo {
   int nv;    /* numero de nos ou vertices */
   int na;    /* numero de arestas */
   Viz** viz; /* viz[i] aponta para a lista de arestas incidindo em i */
};
/* ------------------------- fim do trecho do PDF ---------------------- */

/* [ADICIONADO] typedef para facilitar o uso da struct do slide */
typedef struct _grafo Grafo;

/* [ADICIONADO] funcoes de manipulacao do grafo (os slides mostram apenas
 * a struct; as operacoes abaixo foram escritas para o trabalho)        */
Grafo* grafoCria(int nv);
void   grafoInsereAresta(Grafo* g, int a, int b, float peso);
void   grafoMostraListas(Grafo* g);
void   grafoMostraMatriz(Grafo* g);
void   grafoLibera(Grafo* g);

#endif
