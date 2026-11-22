/* =====================================================================
 * bitvet.c - Conjunto representado como vetor de bits
 * Legenda: [PDF ...] = trecho vindo dos slides | [ADICIONADO] = novo
 * ===================================================================== */
#include <stdio.h>   /* [ADICIONADO] includes necessarios para compilar */
#include <stdlib.h>
#include "bitvet.h"

/* ---------------------------------------------------------------------
 * [PDF "EDA_07_Conjuntos", slides 16 e 17] - copiado do slide.
 * Observacao: o slide usa sizeof(int) (= 4) como quantidade de bits por
 * int, embora um int tenha 32 bits. O codigo continua correto, pois a
 * mesma conta e usada em bvInit, bvGet e bvSet (apenas usa 4 bits de
 * cada int, desperdicando espaco). Mantivemos igual ao slide.
 * --------------------------------------------------------------------- */
struct _bitvector {
       int max;
       int *vector;
};

BitVector* bvInit(int max) {
       int i,size = sizeof(int);
       int num =((max-1)/size)+1; /* numero de ints para max bits */

         BitVector* bv=(BitVector*)malloc(sizeof(BitVector));
         bv->max=max;
         bv->vector=(int *)malloc(num*sizeof(int));
         for (i=0; i<num; i++)
                bv->vector[i]=0;
         return bv;
}

int bvGet(BitVector* bv,int i){
  int res=(bv->vector[i/sizeof(int)])&(1 << i%sizeof(int));
       return (res!=0);
}

void bvSet(BitVector* bv,int i){
       bv->vector[i/sizeof(int)] |= 1 << i%sizeof(int);
}
/* ------------------------- fim do trecho do PDF ---------------------- */

/* [ADICIONADO] bvShow esta declarada na interface do slide 15, mas o
 * slide nao mostra sua implementacao. Imprime os elementos do conjunto. */
void bvShow(BitVector* bv, char* title) {
       int i;
       printf("%s{", title);
       for (i = 0; i < bv->max; i++)
              if (bvGet(bv, i)) printf(" %d", i);
       printf(" }\n");
}

/* [ADICIONADO] libera a memoria */
void bvFree(BitVector* bv) {
       free(bv->vector);
       free(bv);
}
