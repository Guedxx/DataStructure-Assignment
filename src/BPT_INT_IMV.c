//
// Created by nathan on 1/2/25.
//

//#include "TImoveis.c"
#include "TARVBMG.c"

typedef struct IntImv {
    int data;
    Imovel* imv;
} INT_IMV;

// Int functions -=-
bool BPT_INT_IMV_menor_que(void* a, void* b) {
    const INT_IMV* a1 = a;
    const INT_IMV* b1 = b;

    if (a1->data < b1->data) {
        return true;
    }
    if (a1->data == b1->data) {
        return a1->imv == NULL || b1->imv == NULL || a1->imv->id < b1->imv->id;
    }
    return false;
}
void BPT_INT_IMV_imprime_chave(void* a) {
    const INT_IMV* a1 = a;
    printf("%d ", a1->data);
}
void BPT_INT_IMV_imprime_chave_json(void* a, char* buffer) {
    const INT_IMV* a1 = a;
    char buffer2[20];
    sprintf(buffer2, "%d", a1->data);
    strcat(buffer, buffer2);
}
// Int functions -=-

// Pois BPT_INT_IMV herda de TARVBMG. kkkkk
typedef TARVBMG BPT_INT_IMV;

BPT_INT_IMV* BPT_INT_IMV_cria(const int t) {
    return TARVBMG_cria(t);
}

BPT_INT_IMV* BPT_INT_IMV_inicializa() {
    return TARVBMG_inicializa();
}

BPT_INT_IMV* BPT_INT_IMV_busca(BPT_INT_IMV* a, int data, Imovel* imv) {
    INT_IMV int_imv;
    int_imv.data = data;
    int_imv.imv = imv;
    return TARVBMG_busca(a, &int_imv, BPT_INT_IMV_menor_que);
}

BPT_INT_IMV* BPT_INT_IMV_insere(BPT_INT_IMV* T, const int data, Imovel* imv, const int t) {
    if (!imv) {
        perror("Imovel nulo");
        return T;
    }
    INT_IMV* data_ptr = falloc(sizeof(INT_IMV));
    data_ptr->data = data;
    data_ptr->imv = imv;
    return TARVBMG_insere(T, data_ptr, t, BPT_INT_IMV_menor_que);
}

BPT_INT_IMV* BPT_INT_IMV_retira(BPT_INT_IMV* arv, int data, Imovel* imv, const int t) {
    if (!imv) {
        perror("Imovel nulo");
        return T;
    }
    INT_IMV int_imv;
    int_imv.data = data;
    int_imv.imv = imv;
    return TARVBMG_retira(arv, &int_imv, t, BPT_INT_IMV_menor_que);
}

void BPT_INT_IMV_libera(BPT_INT_IMV* a) {
    TARVBMG_libera(a);
}

void BPT_INT_IMV_imprime(const BPT_INT_IMV* a) {
    TARVBMG_imprime(a, BPT_INT_IMV_imprime_chave);
}

void BPT_INT_IMV_imprime_chaves(BPT_INT_IMV* a) {
    TARVBMG_imprime_chaves(a, BPT_INT_IMV_imprime_chave);
}

void BPT_INT_IMV_json(BPT_INT_IMV* a, char* buffer) {
    TARVBMG_json(a, buffer, BPT_INT_IMV_imprime_chave_json);
}

BPT_INT_IMV* BPT_INT_IMV_busca_maior(BPT_INT_IMV* a, int data) {
    return TARVBMG_busca_maior_que(a, &data, BPT_INT_IMV_menor_que);
}

