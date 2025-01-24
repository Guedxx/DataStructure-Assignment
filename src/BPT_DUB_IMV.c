//
// Created by nathan on 1/2/25.
//

//#include "TImoveis.c"
#include "TARVBMG.c"

typedef struct {
    int id;
    IMV* imv;
    double data;
} DUB_IMV;

// Dub functions -=-
char BPT_DUB_IMV_cmp(void* a, void* b) {
    const DUB_IMV* a1 = a;
    const DUB_IMV* b1 = b;

    if (a1->data < b1->data) {
        return -1;
    }
    if (a1->data == b1->data) {
        if (a1->id == -1 || b1->id == -1) {
            return PARCIAL_EQ;
        }
        if (a1->id == b1->id) {
            return 0;
        }
        return a1->id < b1->id ? -1 : 1;
    }
    return 1;
}
void BPT_DUB_IMV_imprime_chave(void* a) {
    const DUB_IMV* a1 = a;
    printf("%f ", a1->data);
}
void BPT_DUB_IMV_imprime_chave_json(void* a, char* buffer) {
    const DUB_IMV* a1 = a;
    char buffer2[20];
    sprintf(buffer2, "%f", a1->data);
    strcat(buffer, buffer2);
}
// Dub functions -=-

// Pois BPT_DUB_IMV herda de TARVBMG. kkkkk
typedef TARVBMG BPT_DUB_IMV;

BPT_DUB_IMV* BPT_DUB_IMV_cria(const int t) {
    return TARVBMG_cria(t);
}

BPT_DUB_IMV* BPT_DUB_IMV_inicializa() {
    return TARVBMG_inicializa();
}

BPT_DUB_IMV* BPT_DUB_IMV_busca(BPT_DUB_IMV* a, const double data, IMV* imv) {
    CHAVE dub_imv = {
        .id = imv ? imv->id : -1,
        .imv = imv
    };
    memcpy(&dub_imv.data, &data, sizeof(double));
    return TARVBMG_busca(a, dub_imv, BPT_DUB_IMV_cmp);
}

BPT_DUB_IMV* BPT_DUB_IMV_insere(BPT_DUB_IMV* T, const double data, IMV* imv, const int t) {
    if (!imv) {
        perror("Imovel nulo");
        return NULL;
    }
    CHAVE dub_imv = {
        .id = imv->id,
        .imv = imv
    };
    memcpy(&dub_imv.data, &data, sizeof(double));
    return TARVBMG_insere(T, dub_imv, t, BPT_DUB_IMV_cmp);
}

BPT_DUB_IMV* BPT_DUB_IMV_retira(BPT_DUB_IMV* arv, double data, IMV* imv, const int t) {
    if (!imv) {
        perror("Imovel nulo");
        return NULL;
    }
    CHAVE dub_imv = {
        .id = imv->id,
        .imv = imv
    };
    memcpy(&dub_imv.data, &data, sizeof(double));
    return TARVBMG_retira(arv, dub_imv, t, BPT_DUB_IMV_cmp);
}

void BPT_DUB_IMV_libera(BPT_DUB_IMV *a) {
    TARVBMG_libera(a);
}

void BPT_DUB_IMV_imprime(const BPT_DUB_IMV *a) {
    TARVBMG_imprime(a, BPT_DUB_IMV_imprime_chave);
}

void BPT_DUB_IMV_json(BPT_DUB_IMV* a, char* buffer) {
    TARVBMG_json(a, buffer, BPT_DUB_IMV_imprime_chave_json);
}

void BPT_DUB_IMV_map_range_2(BPT_DUB_IMV* a, double min, double max, void(*map)(void*, void*), void* arg) {
    CHAVE min_imv = {-1, NULL};
    CHAVE max_imv = {-1, NULL};
    memcpy(&min_imv.data, &min, sizeof(double));
    memcpy(&max_imv.data, &max, sizeof(double));
    TARVBMG_map_range_2(a, min_imv, max_imv, BPT_DUB_IMV_cmp, map, arg);
}
