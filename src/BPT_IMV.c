//
// Created by nathan on 1/1/25.
//
#pragma once

#include "TImoveis.c"
#include "BPT_INT_IMV.c"
#include "BPT_DUB_IMV.c"
#include "BPT_STR_IMV.c"
#include "json.c"
#include <math.h>

typedef struct imovel_tree {
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

// void save_imoveis() {
//     const int fd = open("imoveis.pnt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
//     if (fd == -1) {
//         perror("open");
//         return;
//     }
//     write(fd, &imoveis, sizeof(BPT_IMV*));
//     printf("Imoveis saved to %p\n", imoveis);
//     close(fd);
// }
//
// void load_imoveis() {
//     const int fd = open("imoveis.pnt", O_RDONLY);
//     if (fd == -1) {
//         perror("open");
//         return;
//     }
//     read(fd, &imoveis, sizeof(BPT_IMV*));
//     printf("Imoveis loaded to %p\n", imoveis);
//     close(fd);
// }

Imovel* BPT_IMV_busca_id(const BPT_IMV* a, const int id);



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
    novo->len = 0;
    return novo;
}

void _BPT_INT_IMV_libera_all_imvs(void* a) {
    const INT_IMV* imv = a;
    falloc_free(imv->imv);
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

void BPT_IMV_insere(BPT_IMV* a, Imovel* imovel, const int t) {
    if (BPT_IMV_busca_id(a, imovel->id)) {
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

void BPT_IMV_remove(BPT_IMV* a, Imovel* imovel, const int t) {
    a->id = BPT_INT_IMV_retira(a->id, imovel->id, imovel, t);
    a->bairro = BPT_STR_IMV_retira(a->bairro, imovel->bairro, imovel, t);
    a->rua = BPT_STR_IMV_retira(a->rua, imovel->rua, imovel, t);
    a->tipo = BPT_STR_IMV_retira(a->tipo, imovel->tipo, imovel, t);
    a->cep = BPT_STR_IMV_retira(a->cep, imovel->cep, imovel, t);
    a->preco = BPT_DUB_IMV_retira(a->preco, imovel->precoTotal, imovel, t);
    a->latitude = BPT_DUB_IMV_retira(a->latitude, imovel->latitude, imovel, t);
    a->longitude = BPT_DUB_IMV_retira(a->longitude, imovel->longitude, imovel, t);
    a->len--;
    falloc_free(imovel);
}

void BPT_IMV_imprime(const BPT_IMV* a) {
    printf("Imovel len(%d) at (%p):\n", a->len, a);
    printf("ID\n");
    BPT_INT_IMV_imprime(a->id);
    printf("Bairro\n");
    BPT_STR_IMV_imprime(a->bairro);
    printf("Rua\n");
    BPT_STR_IMV_imprime(a->rua);
    printf("Tipo\n");
    BPT_STR_IMV_imprime(a->tipo);
    printf("CEP\n");
    BPT_STR_IMV_imprime(a->cep);
    printf("Preço\n");
    BPT_DUB_IMV_imprime(a->preco);
    printf("Latitude\n");
    BPT_DUB_IMV_imprime(a->latitude);
    printf("Longitude\n");
    BPT_DUB_IMV_imprime(a->longitude);
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

void submit_imovel(const char* json) {
    Imovel* imovel = falloc(sizeof(Imovel));
    Imovel_from_json(imovel, json);

    #ifdef DEBUG
    Imovel_print(imovel);
    #endif

    BPT_IMV_insere(imoveis, imovel, imoveis_t);
}

typedef struct {
    int id;
    char bairro[100];
    char rua[100];
    char tipo[50];
    char cep[20];
    double precoMin;
    double precoMax;
    double latitude_min;
    double longitude_min;
    double latitude_max;
    double longitude_max;
} ImovelSearch;

void ImovelSearch_from_json(ImovelSearch* imv, const char* json) {
    char buffer[128];

    // Parse each field
    if (extract_json_value(json, "id", buffer, sizeof(buffer))) {
        imv->id = atoi(buffer);
    } else {
        imv->id = -1;
    }

    if (extract_json_value(json, "bairro", imv->bairro, sizeof(imv->bairro))) {
        // Already set
    } else {
        strncpy(imv->bairro, "!NULL!", sizeof(imv->bairro));
    }

    if (extract_json_value(json, "rua", imv->rua, sizeof(imv->rua))) {
        // Already set
    } else {
        strncpy(imv->rua, "!NULL!", sizeof(imv->rua));
    }

    if (extract_json_value(json, "tipo", imv->tipo, sizeof(imv->tipo))) {
        // Already set
    } else {
        strncpy(imv->tipo, "!NULL!", sizeof(imv->tipo));
    }

    if (extract_json_value(json, "cep", imv->cep, sizeof(imv->cep))) {
        // Already set
    } else {
        strncpy(imv->cep, "!NULL!", sizeof(imv->cep));
    }

    if (extract_json_value(json, "precoMin", buffer, sizeof(buffer))) {
        imv->precoMin = atof(buffer);
    } else {
        imv->precoMin = -INFINITY;
    }

    if (extract_json_value(json, "precoMax", buffer, sizeof(buffer))) {
        imv->precoMax = atof(buffer);
    } else {
        imv->precoMax = INFINITY;
    }

    if (extract_json_value(json, "latitude_min", buffer, sizeof(buffer))) {
        imv->latitude_min = atof(buffer);
    } else {
        imv->latitude_min = -INFINITY;
    }

    if (extract_json_value(json, "longitude_min", buffer, sizeof(buffer))) {
        imv->longitude_min = atof(buffer);
    } else {
        imv->longitude_min = -INFINITY;
    }

    if (extract_json_value(json, "latitude_max", buffer, sizeof(buffer))) {
        imv->latitude_max = atof(buffer);
    } else {
        imv->latitude_max = INFINITY;
    }

    if (extract_json_value(json, "longitude_max", buffer, sizeof(buffer))) {
        imv->longitude_max = atof(buffer);
    } else {
        imv->longitude_max = INFINITY;
    }
}


void add_imv2list_str(void* v, void* l) {
    STR_IMV* str_imv = v;
    uint64_t len = ((uint64_t*)l)[0];
    Imovel** imv_list = (Imovel**)l + 1;

    imv_list[len] = str_imv->imv;
    ((uint64_t*)l)[0]++;
}
void add_imv2list_int(void* v, void* l) {
    INT_IMV* int_imv = v;
    uint64_t len = ((uint64_t*)l)[0];
    Imovel** imv_list = (Imovel**)l + 1;

    imv_list[len] = int_imv->imv;
    ((uint64_t*)l)[0]++;
}
void add_imv2list_dub(void* v, void* l) {
    DUB_IMV* dub_imv = v;
    uint64_t len = ((uint64_t*)l)[0];
    Imovel** imv_list = (Imovel**)l + 1;

    imv_list[len] = dub_imv->imv;
    ((uint64_t*)l)[0]++;
}

// Remove da lista 1 os elementos que não estão na lista 2
void filter(Imovel** list1, Imovel** list2) {
    uint64_t len1 = ((uint32_t*) list1)[0];
    const uint64_t len2 = ((uint32_t*) list2)[0];
    list1++; list2++;

    for (int i = 0; i < len1; i++) {
        const Imovel* imv1 = list1[i];

        bool found = false;
        for (int j = 0; j < len2; j++) {
            const Imovel* imv2 = list2[j];
            if (imv1 == imv2) {
                found = true;
                break;
            }
        }

        if (!found) {
            list1[i] = list1[len1 - 1];
            len1--;
        }
    }

    ((uint64_t*) list1 -1)[0] = len1;
}

void search_imoveis(const char* json, const int client_socket) {
    #ifdef DEBUG
    printf("Searching imoveis...\n");
    #endif
    ImovelSearch imv_s;
    ImovelSearch_from_json(&imv_s, json);

    if (imv_s.id != -1) {
        Imovel* imv = BPT_IMV_busca_id(imoveis, imv_s.id);
        if (imv) {
            char buffer[4096];
            buffer[0] = '[';
            Imovel_to_json(imv, buffer+1);
            strcat(buffer, "]");
            SEND_JSON(buffer);
        }
        else {
            #ifdef DEBUG
            printf("Imovel não encontrado\n");
            #endif
            SEND_JSON("[]");
        }
        return;
    }

    Imovel* imv_list1[imoveis->len + 1];
    for (int i = 0; i < imoveis->len + 1; i++) {
        imv_list1[i] = NULL;
    }
    Imovel* imv_list2[imoveis->len + 1];
    for (int i = 0; i < imoveis->len + 1; i++) {
        imv_list2[i] = NULL;
    }
    bool is_first = true;

    // Bairro
    if (strcmp(imv_s.bairro, "!NULL!") != 0) {
        BPT_STR_IMV_map_range_2(imoveis->bairro, imv_s.bairro, imv_s.bairro, add_imv2list_str, imv_list1);
        is_first = false;
    }

    // Rua
    if (strcmp(imv_s.rua, "!NULL!") != 0) {
        if (is_first) {
            BPT_STR_IMV_map_range_2(imoveis->rua, imv_s.rua, imv_s.rua, add_imv2list_str, imv_list1);
            is_first = false;
        } else {
            BPT_STR_IMV_map_range_2(imoveis->rua, imv_s.rua, imv_s.rua, add_imv2list_str, imv_list2);
            filter(imv_list1, imv_list2);
            ((uint64_t*)imv_list2)[0] = 0;
        }
    }

    // Tipo
    if (strcmp(imv_s.tipo, "!NULL!") != 0) {
        if (is_first) {
            BPT_STR_IMV_map_range_2(imoveis->tipo, imv_s.tipo, imv_s.tipo, add_imv2list_str, imv_list1);
            is_first = false;
        } else {
            BPT_STR_IMV_map_range_2(imoveis->tipo, imv_s.tipo, imv_s.tipo, add_imv2list_str, imv_list2);
            filter(imv_list1, imv_list2);
            ((uint64_t*)imv_list2)[0] = 0;
        }
    }

    // CEP
    if (strcmp(imv_s.cep, "!NULL!") != 0) {
        if (is_first) {
            BPT_STR_IMV_map_range_2(imoveis->cep, imv_s.cep, imv_s.cep, add_imv2list_str, imv_list1);
            is_first = false;
        } else {
            BPT_STR_IMV_map_range_2(imoveis->cep, imv_s.cep, imv_s.cep, add_imv2list_str, imv_list2);
            filter(imv_list1, imv_list2);
            ((uint64_t*)imv_list2)[0] = 0;
        }
    }

    // Preço
    if (imv_s.precoMin != -INFINITY || imv_s.precoMax != INFINITY) {
        if (is_first) {
            BPT_DUB_IMV_map_range_2(imoveis->preco, imv_s.precoMin, imv_s.precoMax, add_imv2list_dub, imv_list1);
            is_first = false;
        } else {
            BPT_DUB_IMV_map_range_2(imoveis->preco, imv_s.precoMin, imv_s.precoMax, add_imv2list_dub, imv_list2);
            filter(imv_list1, imv_list2);
            ((uint64_t*)imv_list2)[0] = 0;
        }
    }

    // Latitude
    if (imv_s.latitude_min != -INFINITY || imv_s.latitude_max != INFINITY) {
        if (is_first) {
            BPT_DUB_IMV_map_range_2(imoveis->latitude, imv_s.latitude_min, imv_s.latitude_max, add_imv2list_dub, imv_list1);
            is_first = false;
        } else {
            BPT_DUB_IMV_map_range_2(imoveis->latitude, imv_s.latitude_min, imv_s.latitude_max, add_imv2list_dub, imv_list2);
            filter(imv_list1, imv_list2);
            ((uint64_t*)imv_list2)[0] = 0;
        }
    }

    // Longitude
    if (imv_s.longitude_min != -INFINITY || imv_s.longitude_max != INFINITY) {
        if (is_first) {
            BPT_DUB_IMV_map_range_2(imoveis->longitude, imv_s.longitude_min, imv_s.longitude_max, add_imv2list_dub, imv_list1);
            is_first = false;
        } else {
            BPT_DUB_IMV_map_range_2(imoveis->longitude, imv_s.longitude_min, imv_s.longitude_max, add_imv2list_dub, imv_list2);
            filter(imv_list1, imv_list2);
            ((uint64_t*)imv_list2)[0] = 0;
        }
    }

    // Send the list
    char buffer[4096 * ((uint64_t*)imv_list1)[0]];
    buffer[0] = '['; buffer[1] = '\0';
    for (int i = 1; i <= ((uint64_t*)imv_list1)[0]; i++) {
        Imovel_to_json(imv_list1[i], buffer + strlen(buffer));
        if (i != ((uint64_t*)imv_list1)[0]) {
            strcat(buffer, ", ");
        }
    }
    strcat(buffer, "]");
    SEND_JSON(buffer);
}
void delete_imovel(const char* json, const int client_socket) {

    SEND_JSON("{}");
}


void Imovel_add_from_csv(const char* file) {
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
        Imovel* imovel = falloc(sizeof(Imovel));
        Imovel_from_string(imovel, token);

        BPT_IMV_insere(imoveis, imovel, imoveis_t);
    }

    munmap(data, size);
}
