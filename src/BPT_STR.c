//
// Created by nathan on 12/28/24.
//

#include "TARVBMG.c"

// String functions -=-
bool BPT_STR_menor_que(const void* a, const void* b){
  return strcmp(a, b) < 0;
}
void BPT_STR_imprime_chave(void* a){
  printf("%s ", (char*)a);
}
void BPT_STR_imprime_chave_json(const void* a, char* buffer) {
  strcat(buffer, "\"");
  strcat(buffer, a);
  strcat(buffer, "\"");
}
// String functions -=-

// Pois BPT_STR herda de TARVBMG. kkkkk
typedef TARVBMG BPT_STR;

BPT_STR* BPT_STR_cria(const int t) {
  return TARVBMG_cria(t);
}

BPT_STR* BPT_STR_inicializa(){
  return TARVBMG_inicializa();
}

BPT_STR* BPT_STR_busca(BPT_STR* a, char* data) {
  return TARVBMG_busca(a, data, BPT_STR_menor_que);
}

BPT_STR* BPT_STR_insere(BPT_STR* T, char* data, int t) {
  return TARVBMG_insere(T, data, t, BPT_STR_menor_que);
}

BPT_STR* BPT_STR_retira(BPT_STR* arv, char* data, int t){
  return TARVBMG_retira(arv, data, t, BPT_STR_menor_que);
}

void BPT_STR_libera(BPT_STR *a){
  TARVBMG_libera(a);
}

void BPT_STR_imprime(const BPT_STR *a){
  TARVBMG_imprime(a, BPT_STR_imprime_chave);
}

void BPT_STR_imprime_chaves(BPT_STR *a){
  TARVBMG_imprime_chaves(a, BPT_STR_imprime_chave);
}

void BPT_STR_json(BPT_STR* a, char* buffer){
  TARVBMG_json(a, buffer, BPT_STR_imprime_chave_json);
}

// tests
int main() {
  falloc_start("BPT_STR_test.bin");
  //falloc_free_all();

  BPT_STR* a = BPT_STR_inicializa();

  for (int i = 0; i < 10; i++) {
    char* data = falloc(10);
    sprintf(data, "num: %d", i);
    a = BPT_STR_insere(a, data, 2);
  }

  BPT_STR_imprime(a);
  printf("\n");
  BPT_STR_libera(a);

  printf("Allocated: %d\n", falloc_allocated_size());

  falloc_end();
  return 0;
}