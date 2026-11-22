/* =====================================================================
 * particao.c - Union-Find com vetor, uniao por altura e compressao
 * Legenda: [PDF ...] = regra/descricao vinda dos slides
 *          [ADICIONADO] = codigo C escrito para o trabalho
 * (O PDF 9 nao tem codigo C: todo o codigo aqui foi escrito a partir das
 *  descricoes dos slides indicados em cada funcao.)
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include "particao.h"

/* [PDF 9, slides 14-15 "Representacao por Vetor"]
 *  "Cada elemento do vetor e diretamente mapeado para um elemento do
 *   universo; cada elemento possui um apontador para o pai dele;
 *   a raiz possui -1 como valor do ponteiro."
 * [PDF 9, slide 30 "Posto (rank)"] vetor posto[] guarda a altura.     */
struct _particao {
   int n;       /* tamanho do universo U = {0..n-1}          */
   int* pai;    /* pai[i] = -1 se i e raiz (representante)   */
   int* posto;  /* altura da arvore (valido para as raizes)  */
};

/* MakeSet(x) para todos os x [PDF 9, slides 5 e 9]:
 * "Cria um conjunto com um so elemento (singleton): Si = {x}".
 * Configuracao inicial: "uma floresta com n nos (singletons)" (slide 17). */
Particao* partCria(int n) {
   int i;
   Particao* p = (Particao*)malloc(sizeof(Particao));
   p->n = n;
   p->pai = (int*)malloc(n * sizeof(int));
   p->posto = (int*)malloc(n * sizeof(int));
   for (i = 0; i < n; i++) {
      p->pai[i] = -1;   /* cada no e raiz de si mesmo (slide 15) */
      p->posto[i] = 0;  /* altura 0                            */
   }
   return p;
}

/* Find(x) [PDF 9, slides 5, 10 e 12]:
 *  "busca a raiz da arvore que contem o elemento".
 * + Compressao de trajetoria [PDF 9, slide 35]:
 *  "Se j e um no no caminho de i ate sua raiz, faca j apontar para raiz". */
int partFind(Particao* p, int x) {
   int raiz = x, j, prox;
   /* 1a passada: sobe ate a raiz (pai == -1) */
   while (p->pai[raiz] != -1)
      raiz = p->pai[raiz];
   /* 2a passada: compressao - cada no do caminho passa a apontar p/ raiz */
   j = x;
   while (j != raiz) {
      prox = p->pai[j];
      if (prox != raiz) p->pai[j] = raiz;
      j = prox;
   }
   return raiz;
}

/* Union(x,y) [PDF 9, slides 5 e 11]: "faz a raiz de um grupo apontar
 * para a raiz do outro grupo".
 * Union seguro [slide 18]: executa Find dentro do Union para achar raizes.
 * Uniao por altura [slides 23-25]: "conjunto mais baixo e pendurado no
 * mais alto (altura nao cresce); em caso de empate e indiferente (altura
 * cresce de 1)".
 * Retorno [ADICIONADO]: 1 se uniu dois conjuntos diferentes, 0 se x e y
 * ja estavam no mesmo conjunto (usado pelo Kruskal para detectar ciclo). */
int partUnion(Particao* p, int x, int y) {
   int rx = partFind(p, x);
   int ry = partFind(p, y);
   if (rx == ry)
      return 0;                       /* mesmo conjunto: nada a fazer   */
   if (p->posto[rx] < p->posto[ry]) {
      p->pai[rx] = ry;                /* rx (mais baixo) pendurado em ry */
   } else if (p->posto[rx] > p->posto[ry]) {
      p->pai[ry] = rx;                /* ry (mais baixo) pendurado em rx */
   } else {
      p->pai[ry] = rx;                /* empate: indiferente...          */
      p->posto[rx]++;                 /* ...e a altura cresce de 1       */
   }
   return 1;
}

/* [ADICIONADO] */
void partLibera(Particao* p) {
   free(p->pai);
   free(p->posto);
   free(p);
}
