//
// Created by nathan on 1/2/25.
//

//#include "TImoveis.c"
#include "TARVBMG.c"

typedef struct {
    uint64_t id;
    IMV* imv;
    char* data;
} STR_IMV;

// Str functions -=-
char BPT_STR_IMV_cmp(void* a, void* b) {
    const STR_IMV* a1 = a;
    const STR_IMV* b1 = b;

    const char cmp = strcmp(a1->data, b1->data);
    if (cmp < 0) {
        return -1;
    }
    if (cmp == 0) {
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
void BPT_STR_IMV_imprime_chave(void* a) {
    const STR_IMV* a1 = a;
    printf("%s", a1->data);
}
void BPT_STR_IMV_imprime_chave_json(void* a, char* buffer) {
    const STR_IMV* a1 = a;

    while (!*buffer) buffer++;
    sprintf(buffer, "\"%s\"", a1->data);
}
// Str functions -=-

// Pois BPT_STR_IMV herda de TARVBMG. kkkkk
typedef TARVBMG BPT_STR_IMV;

BPT_STR_IMV* BPT_STR_IMV_cria(const int t) {
    return TARVBMG_cria(t);
}

BPT_STR_IMV* BPT_STR_IMV_inicializa() {
    return TARVBMG_inicializa();
}

BPT_STR_IMV* BPT_STR_IMV_busca(BPT_STR_IMV* a, char* data, IMV* imv) {
    CHAVE key = {
        .data = (intptr_t) data,
        .imv = imv,
        .id = imv->id
    };

    return TARVBMG_busca(a, key, BPT_STR_IMV_cmp);
}

BPT_STR_IMV* BPT_STR_IMV_insere(BPT_STR_IMV* T, const char* data, IMV* imv, const int t) {
    if (!imv) {
        perror("Imovel nulo");
        return NULL;
    }

    CHAVE key = {
        .data = (intptr_t) data,
        .imv = imv,
        .id = imv->id
    };

    return TARVBMG_insere(T, key, t, BPT_STR_IMV_cmp);
}

void BPT_STR_IMV_imprime(const BPT_STR_IMV* a) {
    TARVBMG_imprime(a, BPT_STR_IMV_imprime_chave);
}

BPT_STR_IMV* BPT_STR_IMV_retira(BPT_STR_IMV* arv, const char* data, IMV* imv, const int t) {
    if (!imv) {
        perror("Imovel nulo");
        return NULL;
    }

    CHAVE key = {
        .data = (intptr_t) data,
        .imv = imv,
        .id = imv->id
    };

    return TARVBMG_retira(arv, key, t, BPT_STR_IMV_cmp);
}

void BPT_STR_IMV_libera(BPT_STR_IMV* a) {
    TARVBMG_libera(a);
}

void BPT_STR_IMV_json(BPT_STR_IMV* a, char* buffer) {
    TARVBMG_json(a, buffer, BPT_STR_IMV_imprime_chave_json);
}

void BPT_STR_IMV_map_range_2(BPT_STR_IMV* a, const char* min, const char* max, void(*map)(void*, void*), void* arg) {
    CHAVE min_key = {
        .data = (intptr_t) min,
        .imv = NULL,
        .id = -1
    };
    CHAVE max_key = {
        .data = (intptr_t) max,
        .imv = NULL,
        .id = -1
    };
    TARVBMG_map_range_2(a, min_key, max_key, BPT_STR_IMV_cmp, map, arg);
}





