//
// Created by nathan on 1/1/25.
//

#include "TImoveis.c"

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