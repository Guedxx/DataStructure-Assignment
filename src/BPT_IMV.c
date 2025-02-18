//
// Created by nathan on 1/1/25.
//
#pragma once
#define _GNU_SOURCE

#include "TImoveis.c"
#include "BPT_INT_IMV.c"
#include "BPT_DUB_IMV.c"
#include "BPT_STR_IMV.c"
#include "json.c"
#include <math.h>

typedef struct {
    uint32_t len;
    BPT_INT_IMV* id;
    BPT_STR_IMV* bairro;
    BPT_STR_IMV* rua;
    BPT_STR_IMV* tipo;
    BPT_STR_IMV* cep;

    BPT_DUB_IMV* preco;

    BPT_DUB_IMV* latitude;
    BPT_DUB_IMV* longitude;
} BPT_IMV;

BPT_IMV* imoveis;
int imoveis_t = 2;

IMV* BPT_IMV_busca_id(const BPT_IMV* a, uint64_t id);

BPT_IMV* BPT_IMV_cria(const int t) {
    BPT_IMV* novo = falloc(sizeof(BPT_IMV));
    novo->id = BPT_INT_IMV_cria(t);
    novo->bairro = BPT_STR_IMV_cria(t);
    novo->rua = BPT_STR_IMV_cria(t);
    novo->tipo = BPT_STR_IMV_cria(t);
    novo->cep = BPT_STR_IMV_cria(t);
    novo->preco = BPT_INT_IMV_cria(t);
    novo->latitude = BPT_DUB_IMV_cria(t);
    novo->longitude = BPT_DUB_IMV_cria(t);
    novo->len = 0;
    return novo;
}

void _BPT_INT_IMV_libera_all_imvs(void* a) {
    const INT_IMV* imv = a;
    IMV_free(imv->imv);
}
void BPT_IMV_libera(BPT_IMV* a) {
    TARVBMG_map(a->id, _BPT_INT_IMV_libera_all_imvs);

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

void BPT_IMV_insere(BPT_IMV* a, IMV* imovel, const int t) {
    if (BPT_IMV_busca_id(a, imovel->id)) {
        printf("\033[93mImovel com id %lu já existe\033[0;0m\n", imovel->id);
        return;
    }

    a->id = BPT_INT_IMV_insere(a->id, imovel->id, imovel, t);
    a->bairro = BPT_STR_IMV_insere(a->bairro, imovel->bairro, imovel, t);
    a->rua = BPT_STR_IMV_insere(a->rua, imovel->rua, imovel, t);
    a->tipo = BPT_STR_IMV_insere(a->tipo, imovel->tipo, imovel, t);
    a->cep = BPT_STR_IMV_insere(a->cep, imovel->cep, imovel, t);
    a->preco = BPT_DUB_IMV_insere(a->preco, imovel->precoTotal, imovel, t);
    a->latitude = BPT_DUB_IMV_insere(a->latitude, imovel->latitude, imovel, t);
    a->longitude = BPT_DUB_IMV_insere(a->longitude, imovel->longitude, imovel, t);
    a->len++;
}

void BPT_IMV_remove(BPT_IMV* a, IMV* imovel, const int t) {
    a->id = BPT_INT_IMV_retira(a->id, imovel->id, imovel, t);
    a->bairro = BPT_STR_IMV_retira(a->bairro, imovel->bairro, imovel, t);
    a->rua = BPT_STR_IMV_retira(a->rua, imovel->rua, imovel, t);
    a->tipo = BPT_STR_IMV_retira(a->tipo, imovel->tipo, imovel, t);
    a->cep = BPT_STR_IMV_retira(a->cep, imovel->cep, imovel, t);
    a->preco = BPT_DUB_IMV_retira(a->preco, imovel->precoTotal, imovel, t);
    a->latitude = BPT_DUB_IMV_retira(a->latitude, imovel->latitude, imovel, t);
    a->longitude = BPT_DUB_IMV_retira(a->longitude, imovel->longitude, imovel, t);
    a->len--;
    IMV_free(imovel);
}

void BPT_IMV_imprime(const BPT_IMV* a) {
    printf("imprimindo arvore de imoveis\n");
    printf("ID:\n");
    BPT_INT_IMV_imprime(a->id);
    printf("Bairro:\n");
    BPT_STR_IMV_imprime(a->bairro);
    printf("Rua:\n");
    BPT_STR_IMV_imprime(a->rua);
    printf("Tipo:\n");
    BPT_STR_IMV_imprime(a->tipo);
    printf("CEP:\n");
    BPT_STR_IMV_imprime(a->cep);
    printf("Preço:\n");
    BPT_DUB_IMV_imprime(a->preco);
    printf("Latitude:\n");
    BPT_DUB_IMV_imprime(a->latitude);
    printf("Longitude:\n");
    BPT_DUB_IMV_imprime(a->longitude);
    printf("Imovel len(%d) at (%p):\n", a->len, a);
}

IMV* BPT_IMV_busca_id(const BPT_IMV* a, const uint64_t id) {
    BPT_INT_IMV* b = BPT_INT_IMV_busca(a->id, id, NULL);
    if (!b) return NULL;

    int i = 0;
    const INT_IMV* imv = (INT_IMV*) &b->chaves[i];
    while (imv->data != id) {
        i++;
        imv = (INT_IMV*) &b->chaves[i];
    }

    return imv->imv;
}

void IMV_add_from_csv(const char* file) {
    const int fd = open(file, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    const size_t size = lseek(fd, 0, SEEK_END);
    char* data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);

    char* save;
    for (char* token = strtok_r(data, "\n", &save); token != NULL; token = strtok_r(NULL, "\n", &save) ) {
        Imovel imovel;
        Imovel_from_string(&imovel, token);

        IMV* imv = IMV_from_Imovel(&imovel);

        BPT_IMV_insere(imoveis, imv, imoveis_t);
    }

    munmap(data, size);
}





// TESTES
// #define NUM_TESTES 10000
// #define T_TESTES 3
//
// void main() {
//     falloc_start("BPT_IMV_TEST.bin");
//     falloc_free_all();
//     BPT_IMV* arvore = BPT_IMV_cria(T_TESTES);
//
//     uint64_t ids[NUM_TESTES];
//     int index = 0;
//
//     // Inserção de elementos aleatórios
//     for (int i = 0; i < NUM_TESTES; i++) {
//         Imovel imovel;
//         imovel.id = rand();
//         ids[index++] = imovel.id;
//         sprintf(imovel.bairro, "Bairro: %d", i);
//         sprintf(imovel.rua, "Rua: %d", i);
//         sprintf(imovel.tipo, "Típó");
//         sprintf(imovel.cep, "CEP");
//         imovel.precoTotal = (double)(rand() % 1000000) / 100.0;
//         imovel.latitude = (double)(rand() % 180) - 90;
//         imovel.longitude = (double)(rand() % 360) - 180;
//
//         IMV* imv = IMV_from_Imovel(&imovel);
//
//         BPT_IMV_insere(arvore, imv, T_TESTES);
//         printf("Inserido imóvel com ID %lu\n", imovel.id);
//     }
//
//     //BPT_IMV_imprime(arvore);
//     printf("=========================\n");
//     printf("Arvore len(%d)\n", arvore->len);
//     printf("=========================\n");
//
//     // Remoção de elementos aleatórios
//     for (int i = 0; i < NUM_TESTES / 2; i++) {
//         const uint64_t remove_index = rand() % index;
//         const uint64_t id_remover = ids[remove_index];
//         ids[remove_index] = ids[--index];
//
//         IMV* imovel = BPT_IMV_busca_id(arvore, id_remover);
//         if (imovel) {
//             BPT_IMV_remove(arvore, imovel, T_TESTES);
//             printf("Removido imóvel com ID %lu\n", id_remover);
//         } else {
//             printf("Imóvel com ID %lu não encontrado para remoção\n", id_remover);
//         }
//     }
//
//     //BPT_IMV_imprime(arvore);
//     printf("=========================\n");
//     printf("Arvore len(%d)\n", arvore->len);
//     printf("=========================\n");
//
//     BPT_IMV_libera(arvore);
//     falloc_end();
// }
