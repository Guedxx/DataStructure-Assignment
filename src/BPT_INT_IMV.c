//
// Created by nathan on 1/2/25.
//
#include "TARVBMG.c"

typedef struct {
    uint64_t id;
    IMV* imv;
    intptr_t data;
} INT_IMV;

// Int functions -=-
char BPT_INT_IMV_cmp(void* a, void* b) {
    const INT_IMV* a1 = a;
    const INT_IMV* b1 = b;

    if (a1->data < b1->data) {
        return -1;
    }
    if (a1->data == b1->data) {
        if (a1->id == -1 || b1->id == -1) {
            return PARCIAL_EQ;
        }
        if (a1->id == b1->id) {
            return 0;
        }
        return a1->id < b1->id ? -1 : 1;
    }
    return 1;
}
void BPT_INT_IMV_imprime_chave(void* a) {
    const INT_IMV* a1 = a;
    printf("%lu ", a1->data);
}
void BPT_INT_IMV_imprime_pnt(void* a) {
    const INT_IMV* a1 = a;
    printf("%p, ", a1->imv);
}
void BPT_INT_IMV_imprime_chave_json(void* a, char* buffer) {
    const INT_IMV* a1 = a;
    char buffer2[20];
    sprintf(buffer2, "%lu", a1->data);
    strcat(buffer, buffer2);
}
// Int functions -=-

// Pois BPT_INT_IMV herda de TARVBMG. kkkkk
typedef TARVBMG BPT_INT_IMV;

BPT_INT_IMV* BPT_INT_IMV_cria(const int t) {
    return TARVBMG_cria(t);
}

BPT_INT_IMV* BPT_INT_IMV_inicializa() {
    return TARVBMG_inicializa();
}

BPT_INT_IMV* BPT_INT_IMV_busca(BPT_INT_IMV* a, uint64_t data, IMV* imv) {
    CHAVE int_imv = {
        .id = imv ? imv->id : -1,
        .data = data,
        .imv = imv
    };
    return TARVBMG_busca(a, int_imv, BPT_INT_IMV_cmp);
}

BPT_INT_IMV* BPT_INT_IMV_insere(BPT_INT_IMV* T, const uint64_t data, IMV* imv, const int t) {
    if (!imv) {
        perror("Imovel nulo");
        return NULL;
    }
    CHAVE int_imv = {
        .id = imv->id,
        .data = data,
        .imv = imv
    };
    return TARVBMG_insere(T, int_imv, t, BPT_INT_IMV_cmp);
}

BPT_INT_IMV* BPT_INT_IMV_retira(BPT_INT_IMV* arv, uint64_t data, IMV* imv, const int t) {
    if (!imv) {
        perror("Imovel nulo");
        return NULL;
    }
    CHAVE int_imv = {
        .id = imv->id,
        .data = data,
        .imv = imv
    };
    return TARVBMG_retira(arv, int_imv, t, BPT_INT_IMV_cmp);
}

void BPT_INT_IMV_libera(BPT_INT_IMV* a) {
    TARVBMG_libera(a);
}

void BPT_INT_IMV_imprime(const BPT_INT_IMV* a) {
    TARVBMG_imprime(a, BPT_INT_IMV_imprime_chave);
}

void BPT_INT_IMV_json(BPT_INT_IMV* a, char* buffer) {
    TARVBMG_json(a, buffer, BPT_INT_IMV_imprime_chave_json);
}

void BPT_INT_IMV_map(BPT_INT_IMV* a, void(*map)(void*)) {
    TARVBMG_map(a, map);
}

void BPT_INT_IMV_map_range_2(BPT_INT_IMV* a, const uint64_t min, const uint64_t max, void(*map)(void*, void*), void* arg) {
    CHAVE min_imv = {-1, NULL, min};
    CHAVE max_imv = {-1, NULL, max};
    TARVBMG_map_range_2(a, min_imv, max_imv, BPT_INT_IMV_cmp, map, arg);
}

