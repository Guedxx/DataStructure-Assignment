//
// Created by nathan on 1/2/25.
//

//#include "TImoveis.c"
#include "TARVBMG.c"

typedef struct DubImv {
    double data;
    Imovel* imv;
} DUB_IMV;

// Dub functions -=-
bool BPT_DUB_IMV_menor_que(void* a, void* b) {
    const DUB_IMV* a1 = a;
    const DUB_IMV* b1 = b;

    if (a1->data < b1->data) {
        return true;
    }
    if (a1->data == b1->data) {
        return a1->imv == NULL || b1->imv == NULL || a1->imv->id < b1->imv->id;
    }
    return false;
}
void BPT_DUB_IMV_imprime_chave(void* a) {
    const DUB_IMV* a1 = a;
    printf("%f ", a1->data);
}
void BPT_DUB_IMV_imprime_chave_json(void* a, char* buffer) {
    const DUB_IMV* a1 = a;
    char buffer2[20];
    sprintf(buffer2, "%f", a1->data);
    strcat(buffer, buffer2);
}
// Dub functions -=-

// Pois BPT_DUB_IMV herda de TARVBMG. kkkkk
typedef TARVBMG BPT_DUB_IMV;

BPT_DUB_IMV* BPT_DUB_IMV_cria(const int t) {
    return TARVBMG_cria(t);
}

BPT_DUB_IMV* BPT_DUB_IMV_inicializa() {
    return TARVBMG_inicializa();
}

BPT_DUB_IMV* BPT_DUB_IMV_busca(BPT_DUB_IMV* a, const double data, Imovel* imv) {
    DUB_IMV dub_imv;
    dub_imv.data = data;
    dub_imv.imv = imv;
    return TARVBMG_busca(a, &dub_imv, BPT_DUB_IMV_menor_que);
}

BPT_DUB_IMV* BPT_DUB_IMV_insere(BPT_DUB_IMV* T, const double data, Imovel* imv, const int t) {
    DUB_IMV* data_ptr = falloc(sizeof(DUB_IMV));
    data_ptr->data = data;
    data_ptr->imv = imv;
    return TARVBMG_insere(T, data_ptr, t, BPT_DUB_IMV_menor_que);
}

BPT_DUB_IMV* BPT_DUB_IMV_retira(BPT_DUB_IMV* arv, double data, Imovel* imv, const int t) {
    DUB_IMV dub_imv;
    dub_imv.data = data;
    dub_imv.imv = imv;
    return TARVBMG_retira(arv, &dub_imv, t, BPT_DUB_IMV_menor_que);
}

void BPT_DUB_IMV_libera(BPT_DUB_IMV *a) {
    TARVBMG_libera(a);
}

void BPT_DUB_IMV_imprime(const BPT_DUB_IMV *a) {
    TARVBMG_imprime(a, BPT_DUB_IMV_imprime_chave);
}

void BPT_DUB_IMV_imprime_chaves(BPT_DUB_IMV *a) {
    TARVBMG_imprime_chaves(a, BPT_DUB_IMV_imprime_chave);
}

void BPT_DUB_IMV_json(BPT_DUB_IMV* a, char* buffer) {
    TARVBMG_json(a, buffer, BPT_DUB_IMV_imprime_chave_json);
}

