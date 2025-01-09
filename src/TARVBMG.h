#pragma once
#include "falloc.c"


typedef struct arvbm{
  int nchaves, folha;
  void** chaves;
  struct arvbm **filhos, *prox;
}TARVBMG;


TARVBMG* TARVBMG_cria            (int t);
TARVBMG* TARVBMG_inicializa      ();
TARVBMG* TARVBMG_busca           (TARVBMG *a, void* data, char (*menor_que)(void*, void*));
TARVBMG* TARVBMG_insere          (TARVBMG *T, void* data, int t, char (*menor_que)(void*, void*));
TARVBMG* TARVBMG_retira          (TARVBMG* arv, void* data, int t, char (*menor_que)(void*, void*));
void     TARVBMG_libera          (TARVBMG *a);
void     TARVBMG_imprime         (const TARVBMG *a, void(*imprime)(void*));
void     TARVBMG_imprime_chaves  (TARVBMG *a, void(*imprime)(void*));

// mede by nathan 27/12/24
void TARVBMG_json(TARVBMG* a, char* buffer, void(*imprime)(void*, char*));

#define EQ(a, b)  (!(menor_que((a), (b)) || menor_que((b), (a))))
#define NEQ(a, b) (menor_que((a), (b)) || menor_que((b), (a)))
#define GT(a, b)  (menor_que((b), (a)) && !menor_que((a), (b)))
#define LT(a, b)  (menor_que((a), (b)))

