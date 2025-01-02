//
// Created by nathan on 1/1/25.
//

#include "BPT_INT.c"
#include "BPT_DUB.c"
#include "BPT_STR.c"
#include "TImoveis.c"

BPT_INT* BPT_INT_insere_IMV(BPT_INT* T, const int data, Imovel* imv, const int t) {
    char* data_ptr = falloc(sizeof(int) + sizeof(Imovel*));
    *(int*) data_ptr = data;
    *(Imovel**) (data_ptr + sizeof(int)) = imv;

    return TARVBMG_insere(T, data_ptr, t, BPT_INT_menor_que);
}
#define INT_GET_IMV(a) (*(Imovel**) ((a) + sizeof(int)))

BPT_DUB* BPT_DUB_insere_IMV(BPT_DUB* T, const double data, Imovel* imv, const int t) {
    char* data_ptr = falloc(sizeof(double) + sizeof(Imovel*));
    *(double*) data_ptr = data;
    *(Imovel**) (data_ptr + sizeof(double)) = imv;

    return TARVBMG_insere(T, data_ptr, t, BPT_DUB_menor_que);
}
#define DUB_GET_IMV(a) (*(Imovel**) ((a) + sizeof(double)))

BPT_STR* BPT_STR_insere_IMV(BPT_STR* T, const char* data, Imovel* imv, const int t) {
    char* data_ptr = falloc(strlen(data) + 1 + sizeof(Imovel*));
    strcpy(data_ptr, data);
    *(Imovel**) (data_ptr + strlen(data) + 1) = imv;

    return TARVBMG_insere(T, data_ptr, t, BPT_STR_menor_que);
}
#define STR_GET_IMV(a) (*(Imovel**) ((a) + strlen(a) + 1))

typedef struct imovel_tree {
    BPT_INT* id;
    BPT_STR* bairro;
    BPT_STR* rua;
    BPT_STR* tipo;
    BPT_STR* cep;

    BPT_INT* preco;

    BPT_DUB* latitude;
    BPT_DUB* longitude;
} BPT_IMV;

BPT_IMV* BPT_IMV_cria(const int t) {
    BPT_IMV* imv = falloc(sizeof(BPT_IMV));
    imv->id = BPT_INT_cria(t);
    imv->bairro = BPT_STR_cria(t);
    imv->rua = BPT_STR_cria(t);
    imv->tipo = BPT_STR_cria(t);
    imv->cep = BPT_STR_cria(t);
    imv->preco = BPT_INT_cria(t);
    imv->latitude = BPT_DUB_cria(t);
    imv->longitude = BPT_DUB_cria(t);
    return imv;
}

void BPT_IMV_insere(BPT_IMV* imv, Imovel* data, const int t) {
    imv->id = BPT_INT_insere_IMV(imv->id, data->id, data, t);
    imv->bairro = BPT_STR_insere_IMV(imv->bairro, data->bairro, data, t);
    imv->rua = BPT_STR_insere_IMV(imv->rua, data->rua, data, t);
    imv->tipo = BPT_STR_insere_IMV(imv->tipo, data->tipo, data, t);
    imv->cep = BPT_STR_insere_IMV(imv->cep, data->cep, data, t);
    imv->preco = BPT_INT_insere_IMV(imv->preco, data->precoTotal, data, t);
    imv->latitude = BPT_DUB_insere_IMV(imv->latitude, data->latitude, data, t);
    imv->longitude = BPT_DUB_insere_IMV(imv->longitude, data->longitude, data, t);
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