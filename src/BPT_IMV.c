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
    BPT_INT_IMV_insere(a->id, imovel->id, imovel, t);
    BPT_STR_IMV_insere(a->bairro, imovel->bairro, imovel, t);
    BPT_STR_IMV_insere(a->rua, imovel->rua, imovel, t);
    BPT_STR_IMV_insere(a->tipo, imovel->tipo, imovel, t);
    BPT_STR_IMV_insere(a->cep, imovel->cep, imovel, t);
    BPT_INT_IMV_insere(a->preco, imovel->precoTotal, imovel, t);
    BPT_DUB_IMV_insere(a->latitude, imovel->latitude, imovel, t);
    BPT_DUB_IMV_insere(a->longitude, imovel->longitude, imovel, t);
}

void BPT_IMV_remove(BPT_IMV* a, Imovel* imovel, const int t) {
    BPT_INT_IMV_retira(a->id, imovel->id, t);
    BPT_STR_IMV_retira(a->bairro, imovel->bairro, t);
    BPT_STR_IMV_retira(a->rua, imovel->rua, t);
    BPT_STR_IMV_retira(a->tipo, imovel->tipo, t);
    BPT_STR_IMV_retira(a->cep, imovel->cep, t);
    BPT_INT_IMV_retira(a->preco, imovel->precoTotal, t);
    BPT_DUB_IMV_retira(a->latitude, imovel->latitude, t);
    BPT_DUB_IMV_retira(a->longitude, imovel->longitude, t);
}



/*
    int id;                                 // ID único do imóvel
    char bairro[50];                       // Bairro do imóvel
    char tipo[30];                        // Tipo do imóvel (ex.: "casa", "apartamento")
    char rua[100];                       // Rua do imóvel (pode estar vazia)
    int numero;                         // Número do imóvel (pode ser vazio, -1 se não informado)
    int precoTotal;                    // Preço total do imóvel
    double precoMetroQ;               // Preço por metro quadrado
    char descricao[3867];            // Descrição do imóvel
    char cep[9];                    // CEP do imóvel
    double latitude;               // Latitude do imóvel
    double longitude;             // Longitude do imóvel
 */