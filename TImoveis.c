#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    u_int32_t id;                           // ID único do imóvel
    char bairro[50];                       // Bairro do imóvel
    char tipo[30];                        // Tipo do imóvel (ex.: "casa", "apartamento")
    char rua[100];                       // Rua do imóvel (pode estar vazia)
    int numero;                         // Número do imóvel (pode ser vazio, -1 se não informado)
    u_int32_t precoTotal;              // Preço total do imóvel
    double precoMetroQ;               // Preço por metro quadrado
    char descricao[3867];            // Descrição do imóvel
    char cep[9];                    // CEP do imóvel
    double latitude;               // Latitude do imóvel
    double longitude;             // Longitude do imóvel
} Imovel;


Imovel* criaImovel(u_int32_t id, char bairro[], char tipo[], char rua[], int numero, u_int32_t precoTotal, double precoMetroQ, char descricao[], char cep[], double latitude, double longitude){
    Imovel* imovel = (Imovel*)malloc(sizeof(Imovel));
    if (!imovel){
        printf("Erro ao alocar memória para o imóvel\n");
        exit(1);
    }

    strcpy(imovel->bairro, bairro);
    strcpy(imovel->tipo, tipo);
    strcpy(imovel->rua, rua);
    strcpy(imovel->descricao, descricao);
    strcpy(imovel->cep, cep);

    imovel->id = id;
    imovel->numero = numero;
    imovel->precoTotal = precoTotal;
    imovel->precoMetroQ = precoMetroQ;
    imovel->latitude = latitude;
    imovel->longitude = longitude;

    return imovel;
}
