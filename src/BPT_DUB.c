//
// Created by nathan on 1/1/25.
//

#include "TARVBMG.c"

// Double functions -=-
bool BPT_DUB_menor_que(void* a, void* b){
  return *(double*)a < *(double*)b;
}
void BPT_DUB_imprime_chave(void* a){
  printf("%f ", *(double*)a);
}
void BPT_DUB_imprime_chave_json(void* a, char* buffer) {
  char buffer2[20];
  sprintf(buffer2, "%f", *(double*)a);
  strcat(buffer, buffer2);
}
// Double functions -=-

// Pois BPT_DUB herda de TARVBMG. kkkkk
typedef TARVBMG BPT_DUB;

BPT_DUB* BPT_DUB_cria(const int t) {
  return TARVBMG_cria(t);
}

BPT_DUB* BPT_DUB_inicializa(){
  return TARVBMG_inicializa();
}

BPT_DUB* BPT_DUB_busca(BPT_DUB* a, double data) {
  return TARVBMG_busca(a, &data, BPT_DUB_menor_que);
}

BPT_DUB* BPT_DUB_insere(BPT_DUB* T, const double data, const int t) {
  double* data_ptr = falloc(sizeof(double));
  *data_ptr = data;
  return TARVBMG_insere(T, data_ptr, t, BPT_DUB_menor_que);
}

BPT_DUB* BPT_DUB_retira(BPT_DUB* arv, double data, const int t){
  return TARVBMG_retira(arv, &data, t, BPT_DUB_menor_que);
}

void BPT_DUB_libera(BPT_DUB *a){
  TARVBMG_libera(a);
}

void BPT_DUB_imprime(const BPT_DUB *a) {
  TARVBMG_imprime(a, BPT_DUB_imprime_chave);
}

void BPT_DUB_imprime_chaves(BPT_DUB *a){
  TARVBMG_imprime_chaves(a, BPT_DUB_imprime_chave);
}

void BPT_DUB_json(BPT_DUB* a, char* buffer){
  TARVBMG_json(a, buffer, BPT_DUB_imprime_chave_json);
}

BPT_DUB* BPT_DUB_busca_maior(BPT_DUB* a, double data) {
  return TARVBMG_busca_maior(a, &data, BPT_DUB_menor_que);
}

