//
// Created by nathan on 1/2/25.
//

//#include "TImoveis.c"
#include "TARVBMG.c"

typedef struct strImv {
    IMV* imv;
    uint32_t len;
    char* data;
} STR_IMV;

// Str functions -=-
char BPT_STR_IMV_menor_que(void* a, void* b) {
    const STR_IMV* a1 = a;
    const STR_IMV* b1 = b;

    // gambiarra
    while (a1->data > (void*)0x800000000000 || a1->imv > (void*)0x800000000000) {
        a1 = (STR_IMV*) ((char*) a1 - 1);
    }
    while (b1->data > (void*)0x800000000000 || b1->imv > (void*)0x800000000000) {
        b1 = (STR_IMV*) ((char*) b1 - 1);
    }

    const int cmp = strncmp(a1->data, b1->data, a1->len < b1->len ? a1->len : b1->len);
    if (cmp < 0) {
        return true;
    }
    if (cmp == 0) {
        if (a1->imv == NULL || b1->imv == NULL) {
            return 2;
        }
        return a1->imv->id < b1->imv->id;
    }
    return false;
}
void BPT_STR_IMV_imprime_chave(void* a) {
    const STR_IMV* a1 = a;

    // gambiarra
    if (a1->data > (void*)0x800000000000 || a1->imv > (void*)0x800000000000) {
        a1 = (STR_IMV*) ((char*) a1 - 1);
    }

    printf("%.*s ", a1->len, a1->data);
}
void BPT_STR_IMV_imprime_chave_json(void* a, char* buffer) {
    const STR_IMV* a1 = a;

    // gambiarra
    while (a1->data > (void*)0x800000000000 || a1->imv > (void*)0x800000000000) {
        a1 = (STR_IMV*) ((char*) a1 - 1);
    }

    while (!*buffer) buffer++;
    sprintf(buffer, "\"%.*s\"", a1->len, a1->data);
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

BPT_STR_IMV* BPT_STR_IMV_busca(BPT_STR_IMV* a, const char* data, IMV* imv) {
    STR_IMV key_ptr = {
        .len = strlen(data),
        .data = (char*) data,
        .imv = imv
    };

    return TARVBMG_busca(a, &key_ptr, BPT_STR_IMV_menor_que);
}

BPT_STR_IMV* BPT_STR_IMV_insere(BPT_STR_IMV* T, const char* data, IMV* imv, const int t) {
    if (!imv) {
        perror("Imovel nulo");
        return NULL;
    }

    STR_IMV* key = falloc(sizeof(STR_IMV));
    key->len = strlen(data);
    key->data = (char*) data;
    key->imv = imv;

    return TARVBMG_insere(T, key, t, BPT_STR_IMV_menor_que);
}

void BPT_STR_IMV_imprime(const BPT_STR_IMV* a) {
    TARVBMG_imprime(a, BPT_STR_IMV_imprime_chave);
}

BPT_STR_IMV* BPT_STR_IMV_retira(BPT_STR_IMV* arv, const char* data, IMV* imv, const int t) {
    if (!imv) {
        perror("Imovel nulo");
        return NULL;
    }

    STR_IMV key = {
        .len = strlen(data),
        .data = (char*) data,
        .imv = imv
    };

    BPT_STR_IMV* ret = TARVBMG_retira(arv, &key, t, BPT_STR_IMV_menor_que);
    BPT_STR_IMV_imprime(ret);
    return ret;
}

void BPT_STR_IMV_libera(BPT_STR_IMV* a) {
    TARVBMG_libera(a);
}

void BPT_STR_IMV_imprime_chaves(BPT_STR_IMV* a) {
    TARVBMG_imprime_chaves(a, BPT_STR_IMV_imprime_chave);
}

void BPT_STR_IMV_json(BPT_STR_IMV* a, char* buffer) {
    TARVBMG_json(a, buffer, BPT_STR_IMV_imprime_chave_json);
}

void BPT_STR_IMV_map_range_2(BPT_STR_IMV* a, const char* min, const char* max, void(*map)(void*, void*), void* arg) {
    const uint32_t min_len = strlen(min);
    const uint32_t max_len = strlen(max);

    STR_IMV min_key = {
        .len = min_len,
        .data = (char*) min,
        .imv = NULL
    };
    STR_IMV max_key = {
        .len = max_len,
        .data = (char*) max,
        .imv = NULL
    };

    TARVBMG_map_range_2(a, &min_key, &max_key, BPT_STR_IMV_menor_que, map, arg);
}





