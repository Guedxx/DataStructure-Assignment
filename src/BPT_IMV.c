//
// Created by nathan on 1/1/25.
//

#include "TImoveis.c"
#include "BPT_INT_IMV.c"
#include "BPT_DUB_IMV.c"
#include "BPT_STR_IMV.c"

typedef struct imovel_tree {
    BPT_INT_IMV* id;
    BPT_STR_IMV* bairro;
    BPT_STR_IMV* rua;
    BPT_STR_IMV* tipo;
    BPT_STR_IMV* cep;

    BPT_INT_IMV* preco;

    BPT_DUB_IMV* latitude;
    BPT_DUB_IMV* longitude;
} BPT_IMV;

BPT_IMV* BPT_IMV_cria(const int t) {
    BPT_IMV* novo = (BPT_IMV*) falloc(sizeof(BPT_IMV));
    novo->id = BPT_INT_IMV_cria(t);
    novo->bairro = BPT_STR_IMV_cria(t);
    novo->rua = BPT_STR_IMV_cria(t);
    novo->tipo = BPT_STR_IMV_cria(t);
    novo->cep = BPT_STR_IMV_cria(t);
    novo->preco = BPT_INT_IMV_cria(t);
    novo->latitude = BPT_DUB_IMV_cria(t);
    novo->longitude = BPT_DUB_IMV_cria(t);
    return novo;
}

void BPT_IMV_libera(BPT_IMV* a) {
    BPT_INT_IMV_libera(a->id);
    BPT_STR_IMV_libera(a->bairro);
    BPT_STR_IMV_libera(a->rua);
    BPT_STR_IMV_libera(a->tipo);
    BPT_STR_IMV_libera(a->cep);
    BPT_INT_IMV_libera(a->preco);
    BPT_DUB_IMV_libera(a->latitude);
    BPT_DUB_IMV_libera(a->longitude);
    falloc_free(a);
}

void BPT_IMV_insere(BPT_IMV* a, Imovel* imovel, const int t) {
    a->id = BPT_INT_IMV_insere(a->id, imovel->id, imovel, t);
    a->bairro = BPT_STR_IMV_insere(a->bairro, imovel->bairro, imovel, t);
    a->rua = BPT_STR_IMV_insere(a->rua, imovel->rua, imovel, t);
    a->tipo = BPT_STR_IMV_insere(a->tipo, imovel->tipo, imovel, t);
    a->cep = BPT_STR_IMV_insere(a->cep, imovel->cep, imovel, t);
    a->preco = BPT_INT_IMV_insere(a->preco, imovel->precoTotal, imovel, t);
    a->latitude = BPT_DUB_IMV_insere(a->latitude, imovel->latitude, imovel, t);
    a->longitude = BPT_DUB_IMV_insere(a->longitude, imovel->longitude, imovel, t);
}

void BPT_IMV_remove(BPT_IMV* a, Imovel* imovel, const int t) {
    a->id = BPT_INT_IMV_retira(a->id, imovel->id, imovel, t);
    a->bairro = BPT_STR_IMV_retira(a->bairro, imovel->bairro, imovel, t);
    a->rua = BPT_STR_IMV_retira(a->rua, imovel->rua, imovel, t);
    a->tipo = BPT_STR_IMV_retira(a->tipo, imovel->tipo, imovel, t);
    a->cep = BPT_STR_IMV_retira(a->cep, imovel->cep, imovel, t);
    a->preco = BPT_INT_IMV_retira(a->preco, imovel->precoTotal, imovel, t);
    a->latitude = BPT_DUB_IMV_retira(a->latitude, imovel->latitude, imovel, t);
    a->longitude = BPT_DUB_IMV_retira(a->longitude, imovel->longitude, imovel, t);
}

Imovel* BPT_IMV_busca_id(const BPT_IMV* a, const int id) {
    BPT_INT_IMV* b = BPT_INT_IMV_busca(a->id, id, NULL);
    if (!b) return NULL;

    int i = 0;
    const INT_IMV* imv = b->chaves[i];
    while (imv->data != id) {
        i++;
        imv = b->chaves[i];
    }

    return imv->imv;
}



