#include <stdio.h>
#include <stdlib.h>


typedef struct arvbm{
  int nchaves, folha, *chave;
  struct arvbm **filho, *prox;
}TARVBM;


TARVBM *TARVBM_cria(int t);
TARVBM *TARVBM_inicializa(void);
TARVBM *TARVBM_busca(TARVBM *a, int mat);
TARVBM *TARVBM_insere(TARVBM *T, int mat, int t);
TARVBM* TARVBM_retira(TARVBM* arv, int k, int t);
void TARVBM_libera(TARVBM *a);
void TARVBM_imprime(TARVBM *a);
void TARVBM_imprime_chaves(TARVBM *a);

