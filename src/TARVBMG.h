#pragma once
#include "falloc.c"

typedef struct {
    int id;
    IMV* imv;
    intptr_t data;
} CHAVE;

// No campo de chaves, a primeira coisa da struct da chave deve ser um int de desempate.
typedef struct ArvB {
    int nchaves, folha;
    CHAVE* chaves;
    struct ArvB **filho;
} TARVBMG;


TARVBMG* TARVBMG_cria            (int t);
TARVBMG* TARVBMG_inicializa      ();
TARVBMG* TARVBMG_busca           (TARVBMG *a, CHAVE data, char (*cmp)(void*, void*));
TARVBMG* TARVBMG_insere          (TARVBMG *T, CHAVE data, int t, char (*cmp)(void*, void*));
TARVBMG* TARVBMG_retira          (TARVBMG* arv, CHAVE data, int t, char (*cmp)(void*, void*));
void     TARVBMG_libera          (TARVBMG *a);
void     TARVBMG_imprime         (const TARVBMG *a, void(*imprime)(void*));

void     TARVBMG_imprime_chaves  (TARVBMG *a, void(*imprime)(void*)); //Não tempara B

// mede by nathan 27/12/24
void TARVBMG_json(TARVBMG* a, char* buffer, void(*imprime)(void*, char*));



#define PARCIAL_EQ ((char)-17)

#define PEQ(a, b) (cmp((a), (b)) == PARCIAL_EQ || cmp((a), (b)) == 0)
#define EQ(a, b)  (cmp((a), (b)) == 0)
#define NEQ(a, b) (cmp((a), (b)) != 0)
#define GT(a, b)  (cmp((a), (b)) > 0)
#define LT(a, b)  (cmp((a), (b)) < 0 && cmp((a), (b)) != PARCIAL_EQ)

