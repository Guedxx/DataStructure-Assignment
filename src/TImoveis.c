#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "falloc.c"


typedef struct {
    uint64_t id;                                 // ID único do imóvel
    char bairro[50];                       // Bairro do imóvel
    char tipo[30];                        // Tipo do imóvel (ex.: "casa", "apartamento")
    char rua[100];                       // Rua do imóvel (pode estar vazia)
    int numero;                         // Número do imóvel (pode ser vazio, -1 se não informado)
    double precoTotal;                 // Preço total do imóvel
    double precoMetroQ;               // Preço por metro quadrado
    char descricao[3867];            // Descrição do imóvel
    char cep[9];                    // CEP do imóvel
    double latitude;               // Latitude do imóvel
    double longitude;             // Longitude do imóvel
} Imovel;

void Imovel_print(Imovel* imovel){
    printf("ID: %lu\n", imovel->id);
    printf("Bairro: %s\n", imovel->bairro);
    printf("Tipo: %s\n", imovel->tipo);
    printf("Rua: %s\n", imovel->rua);
    printf("Número: %d\n", imovel->numero);
    printf("Preço total: %f\n", imovel->precoTotal);
    printf("Preço por metro quadrado: %f\n", imovel->precoMetroQ);
    printf("Descrição: %s\n", imovel->descricao);
    printf("CEP: %s\n", imovel->cep);
    printf("Latitude: %f\n", imovel->latitude);
    printf("Longitude: %f\n", imovel->longitude);
}

void Imovel_from_string(Imovel* imovel, char* str) {
    char* token = strtok(str, ";");
    imovel->id = atol(token);

    token = strtok(NULL, ";");
    strncpy(imovel->bairro, token, 50);

    token = strtok(NULL, ";");
    strncpy(imovel->tipo, token, 30);

    token = strtok(NULL, ";");
    strncpy(imovel->rua, token, 100);

    token = strtok(NULL, ";");
    imovel->numero = atoi(token);  // Não pode ter sequencias como: ';;' no numero, todos têm que estar ocupados

    token = strtok(NULL, ";");
    imovel->precoTotal = atof(token);

    token = strtok(NULL, ";");
    imovel->precoMetroQ = atof(token);

    token = strtok(NULL, ";");
    strncpy(imovel->descricao, token, 3867);

    token = strtok(NULL, ";");
    strncpy(imovel->cep, token, 9);

    token = strtok(NULL, ";");
    imovel->latitude = atof(token);

    token = strtok(NULL, ";");
    imovel->longitude = atof(token);
}

void Imovel_to_json(Imovel* imovel, char* json) {
    sprintf(json, "{\"id\": %lu, \"bairro\": \"%s\", \"tipo\": \"%s\", \"rua\": \"%s\", \"numero\": %d, \"precoTotal\": %f, \"precoMetroQ\": %f, \"descricao\": \"%s\", \"cep\": \"%s\", \"latitude\": %f, \"longitude\": %f}", imovel->id, imovel->bairro, imovel->tipo, imovel->rua, imovel->numero, imovel->precoTotal, imovel->precoMetroQ, imovel->descricao, imovel->cep, imovel->latitude, imovel->longitude);
}

void Imovel_from_json(Imovel* imovel, const char* json) {
    sscanf(json, "{\"id\": %lu, \"bairro\": \"%[^\"]\", \"tipo\": \"%[^\"]\", \"rua\": \"%[^\"]\", \"numero\": %d, \"precoTotal\": %lf, \"precoMetroQ\": %lf, \"descricao\": \"%[^\"]\", \"cep\": \"%[^\"]\", \"latitude\": %lf, \"longitude\": %lf}", &imovel->id, imovel->bairro, imovel->tipo, imovel->rua, &imovel->numero, &imovel->precoTotal, &imovel->precoMetroQ, imovel->descricao, imovel->cep, &imovel->latitude, &imovel->longitude);
}

// imv ----------------------------------------------------------------------------------------------------------------------------

typedef struct {
    uint64_t id;                             // ID único do imóvel
    int numero;                        // Número do imóvel (pode ser vazio, -1 se não informado)

    double precoTotal;               // Preço total do imóvel
    double precoMetroQ;             // Preço por metro quadrado
    double latitude;               // Latitude do imóvel
    double longitude;             // Longitude do imóvel

    char cep[9];                // CEP do imóvel

    char* descricao;          // Descrição do imóvel
    char* bairro;            // Bairro do imóvel
    char* tipo;             // Tipo do imóvel (ex.: "casa", "apartamento")
    char* rua;             // Rua do imóvel (pode estar vazia)
} IMV;

void IMV_print(IMV* imv) {
    if (imv == NULL) {
        printf("Imóvel é nulo\n");
        return;
    }
    printf("ID: %lu\n", imv->id);
    printf("Bairro: %s\n", imv->bairro);
    printf("Tipo: %s\n", imv->tipo);
    printf("Rua: %s\n", imv->rua);
    printf("Número: %d\n", imv->numero);
    printf("Preço total: %f\n", imv->precoTotal);
    printf("Preço por metro quadrado: %f\n", imv->precoMetroQ);
    printf("Descrição: %s\n", imv->descricao);
    printf("CEP: %s\n", imv->cep);
    printf("Latitude: %f\n", imv->latitude);
    printf("Longitude: %f\n", imv->longitude);
}

void IMV_to_json(IMV* imv, char* json) {
    if (imv == NULL) {
        sprintf(json, "{}");
        return;
    }
    sprintf(json, "{\"id\": %lu, \"bairro\": \"%s\", \"tipo\": \"%s\", \"rua\": \"%s\", \"numero\": %d, \"precoTotal\": %f, \"precoMetroQ\": %f, \"descricao\": \"%s\", \"cep\": \"%s\", \"latitude\": %f, \"longitude\": %f}", imv->id, imv->bairro, imv->tipo, imv->rua, imv->numero, imv->precoTotal, imv->precoMetroQ, imv->descricao, imv->cep, imv->latitude, imv->longitude);
}

#include <stddef.h>

char remover_acentos(char c) {
    static const char acentuados[] =
        "áàãâéèêíìîóòõôúùûçÁÀÃÂÉÈÊÍÌÎÓÒÕÔÚÙÛÇ";
    static const char sem_acento[] =
        "aaaaeeeiiioooouuucAAAAEEEIIIOOOOUUUC";

    for (size_t i = 0; acentuados[i] != '\0'; i++) {
        if (c == acentuados[i]) {
            return sem_acento[i];
        }
    }
    return c;
}

void strcpy_acentos(char* dest, const char* src) {
    while (*src) {
        *dest = remover_acentos(*src);
        dest++;
        src++;
    }
    *dest = '\0';
}

IMV* IMV_from_Imovel(Imovel* imovel) {
    IMV* imv = falloc(sizeof(IMV));
    imv->id = imovel->id;
    imv->numero = imovel->numero;
    imv->precoTotal = imovel->precoTotal;
    imv->precoMetroQ = imovel->precoMetroQ;
    imv->latitude = imovel->latitude;
    imv->longitude = imovel->longitude;
    strncpy(imv->cep, imovel->cep, 9);

    u_int32_t len = strlen(imovel->descricao);
    imv->descricao = falloc(len + 1);
    strcpy(imv->descricao, imovel->descricao);

    len = strlen(imovel->bairro);
    imv->bairro = falloc(len + 1);
    strcpy_acentos(imv->bairro, imovel->bairro);

    len = strlen(imovel->tipo);
    imv->tipo = falloc(len + 1);
    strcpy_acentos(imv->tipo, imovel->tipo);

    len = strlen(imovel->rua);
    imv->rua = falloc(len + 1);
    strcpy_acentos(imv->rua, imovel->rua);

    return imv;
}

void IMV_free(IMV* a) {
    falloc_free(a->descricao);
    falloc_free(a->bairro);
    falloc_free(a->tipo);
    falloc_free(a->rua);
    falloc_free(a);
}
