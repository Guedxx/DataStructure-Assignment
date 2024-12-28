//
// Created by nathan on 12/27/24.
//

#include "TARVBMG.c"

// Int functions -=-
bool BPT_INT_menor_que(void* a, void* b){
  return *(int*)a < *(int*)b;
}
void BPT_INT_imprime_chave(void* a){
  printf("%d ", *(int*)a);
}
void BPT_INT_imprime_chave_json(void* a, char* buffer) {
  char buffer2[20];
  sprintf(buffer2, "%d", *(int*)a);
  strcat(buffer, buffer2);
}
// Int functions -=-

// Pois BPT_INT herda de TARVBMG. kkkkk
typedef TARVBMG BPT_INT;

BPT_INT* BPT_INT_cria(const int t) {
  return TARVBMG_cria(t);
}

BPT_INT* BPT_INT_inicializa(){
  return TARVBMG_inicializa();
}

BPT_INT* BPT_INT_busca(BPT_INT* a, int data) {
  return TARVBMG_busca(a, &data, BPT_INT_menor_que);
}

BPT_INT* BPT_INT_insere(BPT_INT* T, int data, int t) {
  int* data_ptr = falloc(sizeof(int));
  *data_ptr = data;
  return TARVBMG_insere(T, data_ptr, t, BPT_INT_menor_que);
}

BPT_INT* BPT_INT_retira(BPT_INT* arv, int data, int t){
  return TARVBMG_retira(arv, &data, t, BPT_INT_menor_que);
}

void BPT_INT_libera(BPT_INT *a){
  TARVBMG_libera(a);
}

void BPT_INT_imprime(const BPT_INT *a){
  TARVBMG_imprime(a, BPT_INT_imprime_chave);
}

void BPT_INT_imprime_chaves(BPT_INT *a){
  TARVBMG_imprime_chaves(a, BPT_INT_imprime_chave);
}

void BPT_INT_json(BPT_INT* a, char* buffer){
  TARVBMG_json(a, buffer, BPT_INT_imprime_chave_json);
}


// Testes
// int main() {
//   falloc_start("BPT_INT_test.bin");
//   BPT_INT *a = BPT_INT_inicializa();
//
//   for (int i = 0; i < 11; i += 1) {
//       a = BPT_INT_insere(a, i, 2);
//   }
//
//   BPT_INT_imprime(a);
//   // BPT_INT_libera(a);
//
//   printf("Arvore adr: %p\n", a);
//
//   falloc_end();
//   return 0;
// }

// Testes
int main() {
    falloc_start("BPT_INT_test.bin");
    BPT_INT *a = (TARVBMG*) 0x40001388;

    BPT_INT_imprime(a);

    printf("Jason: \n");
    char buffer[10000];
    for (int i = 0; i < 10000; i++) buffer[i] = 0;
    BPT_INT_json(a, buffer);
    printf("%s\n", buffer);

    falloc_end();
    return 0;
}
