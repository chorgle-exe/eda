/* =====================================================================
 * particao.h - Particao dinamica de conjuntos (Union-Find)
 *
 * [PDF "INF1010 - 9 Particao de conjuntos"]
 *  - Operacoes MakeSet / Find / Union ............... slide 5
 *  - Representacao por vetor (raiz tem -1) .......... slides 14-15
 *  - Union "seguro" (Find dentro do Union) .......... slide 18
 *  - Uniao por altura / posto (rank) ................ slides 23-30
 *  - Compressao de trajetoria ....................... slides 35-39
 *  - Uso no algoritmo de Kruskal .................... slides 52-55
 * Os slides trazem apenas a DESCRICAO dessas operacoes (nao ha codigo C),
 * entao a implementacao em C abaixo e [ADICIONADO], seguindo fielmente
 * as regras descritas em cada slide indicado.
 * ===================================================================== */
#ifndef PARTICAO_H
#define PARTICAO_H

typedef struct _particao Particao;

Particao* partCria(int n);                   /* MakeSet para 0..n-1     */
int       partFind(Particao* p, int x);      /* Find(x)                 */
int       partUnion(Particao* p, int x, int y); /* Union(x,y)           */
void      partLibera(Particao* p);           /* [ADICIONADO]            */

#endif
