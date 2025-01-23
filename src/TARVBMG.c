#pragma once

#include "TARVBMG.h"

TARVBMG *TARVBMG_cria(const int t){
  TARVBMG* novo = (TARVBMG*) falloc(sizeof(TARVBMG));
  novo->nchaves = 0;
  novo->chaves = (CHAVE*) falloc(sizeof(CHAVE)*((t*2)-1));
  novo->folha = 1;
  novo->filhos = (TARVBMG**) falloc(sizeof(TARVBMG*)*t*2);
  novo->prox = NULL;
  for(int i = 0; i<(t*2); i++) novo->filhos[i] = NULL;
  return novo;
}


TARVBMG *TARVBMG_inicializa() {
  return NULL;
}

void TARVBMG_libera(TARVBMG *a){
  if(a){
    if(!a->folha){
      for(int i = 0; i <= a->nchaves; i++) TARVBMG_libera(a->filhos[i]);
    }
    falloc_free(a->filhos);
    // for (int i = 0; i < a->nchaves; i++) {
    //   falloc_free(a->chaves[i]);
    // }
    falloc_free(a->chaves);
    falloc_free(a);
  }
}

TARVBMG *TARVBMG_busca(TARVBMG *a, CHAVE data, char (*cmp)(void*, void*)){
  if (!a) return NULL;
  int i = 0;

  // while ((i < a->nchaves) && (mat > a->chave[i])) i++;
  while ((i < a->nchaves) && GT(&data, &a->chaves[i])) i++;
  if ((i < a->nchaves) && EQ(&data, &a->chaves[i])) return a;

  // if ((i < a->nchaves) && (a->folha) && (mat == a->chave[i])) return a;
  if ((i < a->nchaves) && (a->folha) && EQ(&data, &a->chaves[i])) return a;

  if (a-> folha) return NULL;

  // if (a->chave[i] == mat) i++; // Nathan acha que tem undefined behavior neste código da rosset, testar depois
  if ((i < a->nchaves) && EQ(&data, &a->chaves[i])) i++;

  return TARVBMG_busca(a->filhos[i], data, cmp);
}


void TARVBMG_imprime_chaves(TARVBMG *a, void(*imprime)(void*)){
  if(!a) return;
  TARVBMG *p = a;
  while(p->filhos[0]) p = p->filhos[0];
  while(p){
    // for(i = 0; i < p->nchaves; i++) printf("%d ", p->chave[i]);
    for(int i = 0; i < p->nchaves; i++) imprime(&p->chaves[i]);

    p = p->prox;
  }
  printf("\n");
}


void imp(const TARVBMG *a, const int andar, void(*imprime)(void*)){
  if(a){
    // imp(a->filho[a->nchaves],andar+1);
    imp(a->filhos[0],andar+1, imprime);


    for(int i = a->nchaves - 1; i >= 0; i--){
      for(int j = 0; j<=andar; j++) printf("\t");

      // printf("%d\n", a->chave[i]);
      // imp(a->filho[i],andar+1);
      imprime(&a->chaves[i]); printf("\n");
      imp(a->filhos[i+1],andar+1, imprime);
    }
  }
}

void TARVBMG_imprime(const TARVBMG *a, void(*imprime)(void*)){
  imp(a, 0, imprime);
}

// Não usado em local algum
// void imp0(TARVBMG *a, int andar){
//   if(a){
//     int i,j;
//     for(i=0; i<=a->nchaves-1; i++){
//       imp(a->filho[i],andar+1);
//       for(j=0; j<=andar; j++) printf("\t");
//       printf("%d\n", a->chave[i]);
//     }
//     imp(a->filho[i],andar+1);
//   }
// }
//
// void TARVBMG_imprime0(TARVBMG *a){
//   imp0(a, 0);
// }

TARVBMG *divisao(TARVBMG *x, int i, TARVBMG* y, int t) {
  TARVBMG *z = TARVBMG_cria(t);
  z->folha = y->folha;
  int j;
  if(!y->folha){
    z->nchaves = t-1;
    for(j=0;j<t-1;j++) z->chaves[j] = y->chaves[j+t];
    for(j=0;j<t;j++){
      z->filhos[j] = y->filhos[j+t];
      y->filhos[j+t] = NULL;
    }
  }
  else {
    z->nchaves = t; //z possuirá uma chave a mais que y se for folha
    for(j=0;j < t;j++) z->chaves[j] = y->chaves[j+t-1];//Caso em que y é folha, temos que passar a info para o nó da direita
    z->prox = y->prox; //ultima revisao: 04/2020
    y->prox = z;
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->filhos[j+1]=x->filhos[j];
  x->filhos[i] = z;
  for(j=x->nchaves; j>=i; j--) x->chaves[j] = x->chaves[j-1];
  x->chaves[i-1] = y->chaves[t-1];
  x->nchaves++;
  return x;
}


TARVBMG *insere_nao_completo(TARVBMG *x, CHAVE data, int t, char (*cmp)(void*, void*)) {
  int i = x->nchaves-1;
  if(x->folha){
    // while((i>=0) && (mat < x->chave[i])){
    while((i>=0) && LT(&data, &x->chaves[i])){
      x->chaves[i+1] = x->chaves[i];
      i--;
    }
    // x->chave[i+1] = mat;
    x->chaves[i+1] = data;
    x->nchaves++;
    return x;
  }
  // while((i>=0) && (mat < x->chave[i])) i--;
  while((i>=0) && LT(&data, &x->chaves[i])) i--;
  i++;
  if(x->filhos[i]->nchaves == ((2*t)-1)){
    x = divisao(x, (i+1), x->filhos[i], t);
    // if(mat > x->chave[i]) i++;
    if(GT(&data, &x->chaves[i])) i++;
  }
  //x->filho[i] = insere_nao_completo(x->filho[i], mat, t);
  x->filhos[i] = insere_nao_completo(x->filhos[i], data, t, cmp);
  return x;
}

// data tem que ser alocado dinamicamente
TARVBMG *TARVBMG_insere(TARVBMG *T, CHAVE data, int t, char (*cmp)(void*, void*)) {
  // if(TARVBMG_busca(T, mat)) return T;
  //if(TARVBMG_busca(T, data, cmp)) return T;  // Aqui eu apago a protreção contra inclusão de elementos repetidos

  if(!T){
    T=TARVBMG_cria(t);
    // T->chave[0] = mat;
    T->chaves[0] = data;
    T->nchaves=1;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TARVBMG *S = TARVBMG_cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filhos[0] = T;
    S = divisao(S,1,T,t);
    // S = insere_nao_completo(S, mat, t);
    S = insere_nao_completo(S, data, t, cmp);
    return S;
  }
  // T = insere_nao_completo(T, mat, t);
  T = insere_nao_completo(T, data, t, cmp);
  return T;
}


TARVBMG* remover(TARVBMG* arv, CHAVE data, int t, char (*cmp)(void*, void*)) {
  if(!arv) return arv;
  int i = 0;

  // for(i = 0; i < arv->nchaves && arv->chave[i] < ch; i++);
  // for (i = 0; i < arv->nchaves && LT(&arv->chaves[i], &data); i++) {}
  // Localiza a posição onde `data` se encaixaria ou está localizada
  while (i < arv->nchaves && GT(&data, &arv->chaves[i])) {
    i++;
  }

  // if((i < arv->nchaves) && (ch == arv->chave[i]) && (arv->folha)){ //CASO 1
  if((i < arv->nchaves) && EQ(&data, &arv->chaves[i]) && arv->folha){   //CASO 1
    //printf("\nCASO 1\n");
    for(int j = i; j<arv->nchaves-1;j++) arv->chaves[j] = arv->chaves[j+1];
    arv->nchaves--;
    if(!arv->nchaves) { //ultima revisao: 04/2020
      TARVBMG_libera(arv);
      arv = NULL;
    }
    return arv;
  }

  // if((i < arv->nchaves) && (ch == arv->chave[i])) i++;
  if((i < arv->nchaves) && EQ(&data, &arv->chaves[i])) i++;
  TARVBMG *y = arv->filhos[i], *z = NULL;

  // Não deveria contecer, é só precaução
  if (!y) {
    printf("\033[0;31m???\033[0m\n");
    return arv;
  }

  if (y->nchaves == t-1) { //CASOS 3A e 3B
    if((i < arv->nchaves) && (arv->filhos[i+1]->nchaves >=t)){ //CASO 3A
      //printf("\nCASO 3A: i menor que nchaves\n");
      z = arv->filhos[i+1];
      if(!y->folha) {
        y->chaves[t-1] = arv->chaves[i];   //dar a y a chave i da arv
        arv->chaves[i] = z->chaves[0];     //dar a arv uma chave de z
      }
      else{
        //arv->chaves[i] = z->chaves[0] + 1;                       // pq mais 1? descobri já
        arv->chaves[i] = z->chaves[0];  arv->chaves[i].pato++;     // (*(int*)arv->chaves[i])++; // Este é o +1.
        y->chaves[t-1] = z->chaves[0];
      }
      y->nchaves++;

      int j;
      for(j=0; j < z->nchaves-1; j++)       //ajustar chaves de z
        z->chaves[j] = z->chaves[j+1];
      y->filhos[y->nchaves] = z->filhos[0]; //enviar ponteiro menor de z para o novo elemento em y
      for(j=0; j < z->nchaves; j++)         //ajustar filhos de z
        z->filhos[j] = z->filhos[j+1];
      z->nchaves--;
      // arv->filho[i] = remover(arv->filho[i], ch, t);
      arv->filhos[i] = remover(arv->filhos[i], data, t, cmp);
      return arv;
    }
    if((i > 0) && (arv->filhos[i-1]->nchaves >= t)){ //CASO 3A
      //printf("\nCASO 3A: i igual a nchaves\n");
      z = arv->filhos[i-1];
      int j;
      for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
        y->chaves[j] = y->chaves[j-1];
      for(j = y->nchaves+1; j>0; j--) //encaixar lugar dos filhos da nova chave
        y->filhos[j] = y->filhos[j-1];

      if(!y->folha){
        y->chaves[0] = arv->chaves[i-1];    //dar a y a chave i da arv
        arv->chaves[i-1] = z->chaves[z->nchaves - 1];   //dar a arv uma chave de z
      }
      else{
        arv->chaves[i-1] = z->chaves[z->nchaves - 1];
        y->chaves[0] = z->chaves[z->nchaves-1];
      }
      y->nchaves++;

      //enviar ponteiro de z para o novo elemento em y
      y->filhos[0] = z->filhos[z->nchaves];
      z->nchaves--;
      // arv->filho[i] = remover(y, ch, t);
      arv->filhos[i] = remover(y, data, t, cmp);
      return arv;
    }
    //CASO 3B
    if(i < arv->nchaves && arv->filhos[i+1]->nchaves == t-1){
      //printf("\nCASO 3B: i menor que nchaves\n");
      z = arv->filhos[i+1];
      if(!y->folha){
        y->chaves[t-1] = arv->chaves[i];//pegar chave [i] e coloca ao final de filho[i]
        y->nchaves++;
      }
      int j = 0;
      while(j < t-1){
        if(!y->folha) y->chaves[t+j] = z->chaves[j];
        else y->chaves[t+j-1] = z->chaves[j];
        y->nchaves++;
        j++;
      }
      y->prox = z->prox;
      if(!y->folha){
        for(j=0; j<t; j++){
          y->filhos[t+j] = z->filhos[j];
          z->filhos[j] = NULL; //ultima revisao: 04/2020
        }
        //TARVBMG_libera(z); 07/2024
      }
      TARVBMG_libera(z); // 07/2024
      for(j=i; j < arv->nchaves-1; j++){ // limpar referências de i
        arv->chaves[j] = arv->chaves[j+1];
        arv->filhos[j+1] = arv->filhos[j+2];
      }
      arv->filhos[arv->nchaves] = NULL;
      arv->nchaves--;
      if(!arv->nchaves){ //ultima revisao: 04/2020
        TARVBMG *temp = arv;
        arv = arv->filhos[0];
        temp->filhos[0] = NULL;
        TARVBMG_libera(temp);
      }
      // arv = remover(arv, ch, t);
      arv = remover(arv, data, t, cmp);
      return arv;
    }
    if((i > 0) && (arv->filhos[i-1]->nchaves == t-1)){
      //printf("\nCASO 3B: i igual a nchaves\n");
      z = arv->filhos[i-1];
      if(!y->folha){
        if(i == arv->nchaves){
          z->chaves[t-1] = arv->chaves[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
        }else{
          z->chaves[t-1] = arv->chaves[i];   //pegar chave [i] e poe ao final de filho[i-1]
        }
        z->nchaves++;
      }
      int j = 0;
      while(j < t-1){
        if(!y->folha) z->chaves[t+j] = y->chaves[j];
        else z->chaves[t+j-1] = y->chaves[j];
        z->nchaves++;
        j++;
      }
      z->prox = y->prox;
      if(!z->folha){
        for(j=0; j<t; j++){
          z->filhos[t+j] = y->filhos[j];
          y->filhos[j] = NULL; //ultima revisao: 04/2020
        }
        //TARVBMG_libera(y); 07/2024
      }
      TARVBMG_libera(y); // 07/2024
      arv->filhos[arv->nchaves] = NULL;
      arv->nchaves--;
      if(!arv->nchaves){ //ultima revisao: 04/2020
        TARVBMG *temp = arv;
        arv = arv->filhos[0];
        temp->filhos[0] = NULL;
        TARVBMG_libera(temp);
        //arv = remover(arv, ch, t);
        arv = remover(arv, data, t, cmp);
      }
      else{
        i--;
        //arv->filho[i] = remover(arv->filho[i], ch, t);
        arv->filhos[i] = remover(arv->filhos[i], data, t, cmp);
      }
      return arv;
    }
  }
  //arv->filho[i] = remover(arv->filho[i], ch, t);
  arv->filhos[i] = remover(arv->filhos[i], data, t, cmp);
  return arv;
}

TARVBMG* TARVBMG_retira(TARVBMG* arv, CHAVE data, int t, char (*cmp)(void*, void*)) {
  //if(!arv || !TARVBMG_busca(arv, k)) return arv;
  if(!arv || !TARVBMG_busca(arv, data, cmp)) {
    printf("Não pode remover, pois não está na árvore\n");
    return arv;
  }
  //return remover(arv, k, t);
  return remover(arv, data, t, cmp);
}

// minhas alterações 28/12/2024

void TARVBMG_json(TARVBMG* a, char* buffer, void(*imprime)(void*, char*)) {
  if (!a) {
    strcat(buffer, "null");
    return;
  }

  strcat(buffer, "{ \"nchaves\": ");
  char nchaves[10];
  sprintf(nchaves, "%d", a->nchaves);
  strcat(buffer, nchaves);

  strcat(buffer, ", \"folha\": ");
  strcat(buffer, a->folha ? "true" : "false");

  strcat(buffer, ", \"chaves\": [");
  for (int i = 0; i < a->nchaves; i++) {
    imprime(&a->chaves[i], buffer);
    if (i < a->nchaves - 1) strcat(buffer, ", ");
  }
  strcat(buffer, "]");

  if (!a->folha) {
    strcat(buffer, ", \"filhos\": [");
    for (int i = 0; i <= a->nchaves; i++) {
      TARVBMG_json(a->filhos[i], buffer, imprime);
      if (i < a->nchaves) strcat(buffer, ", ");
    }
    strcat(buffer, "]}");
  }
  else {
    strcat(buffer, "}");
  }
}

TARVBMG* TARVBMG_busca_menor(TARVBMG* a) {
  if (!a) return NULL;

  while (!a->folha) a = a->filhos[0];
  return a;
}
TARVBMG* TARVBMG_busca_maior(TARVBMG* a) {
  if (!a) return NULL;

  while (!a->folha) a = a->filhos[a->nchaves];
  return a;
}


// Executa uma função em todos os elementos da árvore
void TARVBMG_map(TARVBMG* a, void(*map)(void*)) {
  if (!a) return;

  TARVBMG* menor = TARVBMG_busca_menor(a);

  TARVBMG* p = menor;

  while (p) {
    for (int i = 0; i < p->nchaves; i++) {
      map(&p->chaves[i]);
    }
    p = p->prox;
  }
}

// Busca o primeiro elemento maior ou igual a data
TARVBMG* TARVBMG_busca_maior_que(TARVBMG* a, CHAVE data, char (*cmp)(void*, void*)) {
  if (!a) return NULL;
  if (a->folha) return a;

  int i = 0;
  while (i < a->nchaves && !LT(&data, &a->chaves[i])) i++;

  return TARVBMG_busca_maior_que(a->filhos[i], data, cmp);
}

// Executa uma função em todos os elementos da árvore que estão no intervalo [min, max]
void TARVBMG_map_range(TARVBMG* a, CHAVE min, CHAVE max, char (*cmp)(void*, void*), void(*map)(void*)) {
  if (!a) return;

  const TARVBMG* menor = TARVBMG_busca_maior_que(a, min, cmp);

  const TARVBMG* p = menor;

  while (p) {
    for (int i = 0; i < p->nchaves; i++) {
      if (GT(&p->chaves[i], &max)) return;

      if (LT(&min, &p->chaves[i]) || PEQ(&min, &p->chaves[i])) {
        map(&p->chaves[i]);
      }
    }
    p = p->prox;
  }
}

// Executa uma função em todos os elementos da árvore que estão no intervalo [min, max]
// cmp deve retornar 2 se os elementos forem iguais
void TARVBMG_map_range_2(TARVBMG* a, CHAVE min, CHAVE max, char (*cmp)(void*, void*), void(*map)(void*, void*), void* arg) {
  if (!a) return;

  const TARVBMG* menor = TARVBMG_busca_maior_que(a, min, cmp);

  const TARVBMG* p = menor;

  while (p) {
    for (int i = 0; i < p->nchaves; i++) {
      if (GT(&p->chaves[i], &max)) return;

      if (LT(&min, &p->chaves[i]) || PEQ(&min, &p->chaves[i])) {
        map(&p->chaves[i], arg);
      }
    }
    p = p->prox;
  }
}
