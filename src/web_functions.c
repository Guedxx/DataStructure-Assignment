//
// Created by nathan on 1/10/25.
//
#include "BPT_IMV.c"


void submit_imovel(const char* json) {
    #if defined(DEBUG_WEB2) || defined(DEBUG_WEB3)
        printf("/submit_imovel ");
        printf("Recebido JSON:\n    %s\n", json);
    #endif


    Imovel imovel;
    Imovel_from_json(&imovel, json);

    #if defined(DEBUG_WEB1) || defined(DEBUG_WEB2) || defined(DEBUG_WEB3)
        printf("Adicionando imovel\n");
    #endif

    IMV* imv = IMV_from_Imovel(&imovel);
    BPT_IMV_insere(imoveis, imv, imoveis_t);
}

void delete_imovel(const char* json) {
    #if defined(DEBUG_WEB2) || defined(DEBUG_WEB3)
        printf("/delete_imovel ");
        printf("Recebido JSON:\n    %s\n", json);
    #endif

    char buffer[128];
    if (!extract_json_value(json, "id", buffer, sizeof(buffer))) {
        return;
    }
    uint64_t id = atol(buffer);
    // IMV* imv = NULL;
    // BPT_INT_IMV_map_range_2(imoveis->id, id, id, set_imv_id, &imv);
    IMV* imv = BPT_IMV_busca_id(imoveis, id);


    #if defined(DEBUG_WEB1) || defined(DEBUG_WEB2) || defined(DEBUG_WEB3)
        printf("Removendo imovel com id %lu\n", id);
    #endif

    if (imv) {
        BPT_IMV_remove(imoveis, imv, imoveis_t);
    }
}



// search_imoveis --------------------------------------------------------------------------------------
typedef struct {
    uint64_t id;
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
        imv->id = atol(buffer);
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
    IMV** imv_list = (IMV**)l + 1;

    imv_list[len] = str_imv->imv;
    ((uint64_t*)l)[0]++;
}
void add_imv2list_int(void* v, void* l) {
    INT_IMV* int_imv = v;
    uint64_t len = ((uint64_t*)l)[0];
    IMV** imv_list = (IMV**)l + 1;

    imv_list[len] = int_imv->imv;
    ((uint64_t*)l)[0]++;
}
void add_imv2list_dub(void* v, void* l) {
    DUB_IMV* dub_imv = v;
    uint64_t len = ((uint64_t*)l)[0];
    IMV** imv_list = (IMV**)l + 1;

    imv_list[len] = dub_imv->imv;
    ((uint64_t*)l)[0]++;
}

// Remove da lista 1 os elementos que não estão na lista 2
void filter(IMV** list1, IMV** list2) {
    uint64_t len1 = ((uint32_t*) list1)[0];
    const uint64_t len2 = ((uint32_t*) list2)[0];
    list1++; list2++;

    for (int i = 0; i < len1; i++) {
        const IMV* imv1 = list1[i];

        bool found = false;
        for (int j = 0; j < len2; j++) {
            const IMV* imv2 = list2[j];
            if (imv1 == imv2) {
                found = true;
                break;
            }
        }

        if (!found) {
            list1[i] = list1[len1 - 1];
            len1--;
            i--;
        }
    }

    ((uint64_t*) list1 -1)[0] = len1;
}

void search_imoveis(const char* json, const int client_socket) {
    #if defined(DEBUG_WEB2) || defined(DEBUG_WEB3)
        printf("/search_imoveis ");
        printf("Recebido JSON:\n    %s\n", json);
    #endif
    ImovelSearch imv_s;
    ImovelSearch_from_json(&imv_s, json);

    if (imv_s.id != -1) {
        IMV* imv = BPT_IMV_busca_id(imoveis, imv_s.id);
        if (imv) {
            char buffer[4096];
            buffer[0] = '[';
            IMV_to_json(imv, buffer+1);
            strcat(buffer, "]");
            SEND_JSON(buffer);
        }
        else {
            SEND_JSON("[]");
        }

        #if defined(DEBUG_WEB1) || defined(DEBUG_WEB2) || defined(DEBUG_WEB3)
            if (imv) {
                printf("Encontrado imovel com id %d\n", imv_s.id);
                IMV_print(imv);
            }
            else {
                printf("Imovel com id %d não encontrado\n", imv_s.id);
            }
        #endif

        return;
    }

    IMV* imv_list1[imoveis->len + 1];
    uint64_t* imv_list1_len = (uint64_t*)imv_list1;
    *imv_list1_len = 0;

    IMV* imv_list2[imoveis->len + 1];
    uint64_t* imv_list2_len = (uint64_t*)imv_list2;
    *imv_list2_len = 0;

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
        IMV_to_json(imv_list1[i], buffer + strlen(buffer));
        if (i != ((uint64_t*)imv_list1)[0]) {
            strcat(buffer, ", ");
        }
    }
    strcat(buffer, "]");

    #if defined(DEBUG_WEB1) || defined(DEBUG_WEB2) || defined(DEBUG_WEB3)
        printf("Encontrados %lu imoveis\n[", ((uint64_t*)imv_list1)[0]);
        for (int i = 1; i <= ((uint64_t*)imv_list1)[0]; i++) {
            printf("%d", imv_list1[i]->id);
            if (i != ((uint64_t*)imv_list1)[0]) {
                printf(", ");
            }
        }
        printf("]\n");
    #endif

    SEND_JSON(buffer);
}