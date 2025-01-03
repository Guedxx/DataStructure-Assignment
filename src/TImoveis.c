#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct {
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
} Imovel;


void Imovel_init(Imovel* imovel, int id, char bairro[], char tipo[], char rua[], int numero, int precoTotal, double precoMetroQ, char descricao[], char cep[], double latitude, double longitude){
    strncpy(imovel->bairro, bairro, 50);
    strncpy(imovel->tipo, tipo, 30);
    strncpy(imovel->rua, rua, 100);
    strncpy(imovel->descricao, descricao, 3867);
    strncpy(imovel->cep, cep, 9);

    imovel->id = id;
    imovel->numero = numero;
    imovel->precoTotal = precoTotal;
    imovel->precoMetroQ = precoMetroQ;
    imovel->latitude = latitude;
    imovel->longitude = longitude;
}

void Imovel_print(Imovel* imovel){
    printf("ID: %u\n", imovel->id);
    printf("Bairro: %s\n", imovel->bairro);
    printf("Tipo: %s\n", imovel->tipo);
    printf("Rua: %s\n", imovel->rua);
    printf("Número: %d\n", imovel->numero);
    printf("Preço total: %d\n", imovel->precoTotal);
    printf("Preço por metro quadrado: %f\n", imovel->precoMetroQ);
    printf("Descrição: %s\n", imovel->descricao);
    printf("CEP: %s\n", imovel->cep);
    printf("Latitude: %f\n", imovel->latitude);
    printf("Longitude: %f\n", imovel->longitude);
}

void Imovel_from_string(Imovel* imovel, char* str){
    char* token = strtok(str, ";");
    imovel->id = atoi(token);

    token = strtok(NULL, ";");
    strncpy(imovel->bairro, token, 50);

    token = strtok(NULL, ";");
    strncpy(imovel->tipo, token, 30);

    token = strtok(NULL, ";");
    strncpy(imovel->rua, token, 100);

    token = strtok(NULL, ";");
    imovel->numero = atoi(token);  // Não pode ter sequencias como: `;;` no numero, todos têm que estar ocupados

    token = strtok(NULL, ";");
    imovel->precoTotal = atoi(token);

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
    sprintf(json, "{\"id\": %u, \"bairro\": \"%s\", \"tipo\": \"%s\", \"rua\": \"%s\", \"numero\": %d, \"precoTotal\": %d, \"precoMetroQ\": %f, \"descricao\": \"%s\", \"cep\": \"%s\", \"latitude\": %f, \"longitude\": %f}", imovel->id, imovel->bairro, imovel->tipo, imovel->rua, imovel->numero, imovel->precoTotal, imovel->precoMetroQ, imovel->descricao, imovel->cep, imovel->latitude, imovel->longitude);
}

void Imovel_from_json(Imovel* imovel, const char* json) {
    sscanf(json, "{\"id\": %u, \"bairro\": \"%[^\"]\", \"tipo\": \"%[^\"]\", \"rua\": \"%[^\"]\", \"numero\": %d, \"precoTotal\": %d, \"precoMetroQ\": %lf, \"descricao\": \"%[^\"]\", \"cep\": \"%[^\"]\", \"latitude\": %lf, \"longitude\": %lf}", &imovel->id, imovel->bairro, imovel->tipo, imovel->rua, &imovel->numero, &imovel->precoTotal, &imovel->precoMetroQ, imovel->descricao, imovel->cep, &imovel->latitude, &imovel->longitude);
}

void test_imovel(const char* json, const int client_socket) {
    Imovel imovel;
    Imovel_from_json(&imovel, json);
    Imovel_print(&imovel);
}

// teste
// int main() {
//     const int fd = open("TC_EDA_out.csv", O_RDONLY);
//     if (fd == -1) {
//         perror("open");
//         return 1;
//     }
//     size_t size = lseek(fd, 0, SEEK_END);
//     char* data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
//
//     char* save;
//     for (char* token = strtok_r(data, "\n", &save); token != NULL; token = strtok_r(NULL, "\n", &save) ) {
//         Imovel imovel;
//         Imovel_from_string(&imovel, token);
//         printf("Original:\n");
//         Imovel_print(&imovel);
//         printf("\n JSON:\n");
//         char json[4096];
//         Imovel_to_json(&imovel, json);
//         printf("%s\n", json);
//         printf("\n");
//         printf("From JSON:\n");
//         Imovel imovel2;
//         Imovel_from_json(&imovel2, json);
//         Imovel_print(&imovel2);
//         printf("\n\n");
//     }
//
//     munmap(data, size);
// }

