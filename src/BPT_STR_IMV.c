//
// Created by nathan on 1/2/25.
//

//#include "TImoveis.c"
#include "TARVBMG.c"

typedef struct strImv {
    Imovel* imv;
    uint32_t len;
    //char* data;
} STR_IMV;

#define GET_DATA(a) (((char*)a) + sizeof(STR_IMV))

// Str functions -=-
bool BPT_STR_IMV_menor_que(void* a, void* b) {
    const STR_IMV* a1 = a;
    const STR_IMV* b1 = b;

    const int cmp = strncmp(GET_DATA(a1), GET_DATA(b1), a1->len < b1->len ? a1->len : b1->len);
    if (cmp < 0) {
        return true;
    }
    if (cmp == 0) {
        return a1->imv == NULL || b1->imv == NULL || a1->imv->id < b1->imv->id;
    }
    return false;
}
void BPT_STR_IMV_imprime_chave(void* a) {
    const STR_IMV* a1 = a;
    printf("%.*s ", a1->len, GET_DATA(a1));
}
void BPT_STR_IMV_imprime_chave_json(void* a, char* buffer) {
    const STR_IMV* a1 = a;
    while (!*buffer) buffer++;
    sprintf(buffer, "\"%.*s\"", a1->len, GET_DATA(a1));
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

BPT_STR_IMV* BPT_STR_IMV_busca(BPT_STR_IMV* a, const char* data, Imovel* imv) {
    uint32_t len = strlen(data);
    char key[sizeof(STR_IMV) + len + 1];
    STR_IMV* key_ptr = (STR_IMV*) key;

    key_ptr->len = len;
    memcpy(GET_DATA(key_ptr), data, len);
    key_ptr->imv = imv;

    return TARVBMG_busca(a, key_ptr, BPT_STR_IMV_menor_que);
}

BPT_STR_IMV* BPT_STR_IMV_insere(BPT_STR_IMV* T, const char* data, Imovel* imv, const int t) {
    uint32_t len = strlen(data);
    char* key = falloc(sizeof(STR_IMV) + len + 1);
    STR_IMV* key_ptr = (STR_IMV*) key;

    key_ptr->len = len;
    memcpy(GET_DATA(key_ptr), data, len);
    key_ptr->imv = imv;

    return TARVBMG_insere(T, key, t, BPT_STR_IMV_menor_que);
}

BPT_STR_IMV* BPT_STR_IMV_retira(BPT_STR_IMV* arv, const char* data, Imovel* imv, const int t) {
    const uint32_t len = strlen(data);
    char key[sizeof(STR_IMV) + len + 1];
    STR_IMV* key_ptr = (STR_IMV*) key;

    key_ptr->len = len;
    memcpy(GET_DATA(key_ptr), data, len);
    key_ptr->imv = imv;

    return TARVBMG_retira(arv, key, t, BPT_STR_IMV_menor_que);
}

void BPT_STR_IMV_libera(BPT_STR_IMV* a) {
    TARVBMG_libera(a);
}

void BPT_STR_IMV_imprime(const BPT_STR_IMV* a) {
    TARVBMG_imprime(a, BPT_STR_IMV_imprime_chave);
}

void BPT_STR_IMV_imprime_chaves(BPT_STR_IMV* a) {
    TARVBMG_imprime_chaves(a, BPT_STR_IMV_imprime_chave);
}

void BPT_STR_IMV_json(BPT_STR_IMV* a, char* buffer) {
    TARVBMG_json(a, buffer, BPT_STR_IMV_imprime_chave_json);
}







