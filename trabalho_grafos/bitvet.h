/* =====================================================================
 * [PDF "EDA_07_Conjuntos", slide 15 "Uma interface para conjuntos
 *  quaisquer"] - interface copiada do slide.
 * Usada no Dijkstra para guardar o CONJUNTO de vertices ja visitados.
 * ===================================================================== */
/* EDA: bitvet.h */
/* ADT: Sets, where pertinence is related to a bit in a Bit Vector */

#ifndef _bitvet
#define _bitvet

typedef struct _bitvector BitVector;

BitVector* bvInit(int mx);

int bvGet(BitVector* v,int i);

void bvSet(BitVector* v,int i);

void bvShow(BitVector* v, char* title);

/* (bvIntrsec existe no slide 15/18, mas nao e usada neste trabalho,
 *  por isso nao foi incluida) */

/* [ADICIONADO] libera a memoria (nao existe nos slides) */
void bvFree(BitVector* v);

#endif
