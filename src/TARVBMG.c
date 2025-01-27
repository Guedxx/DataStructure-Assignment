#pragma once

#include "falloc.c"

typedef struct {
  uint64_t id;
  IMV* imv;
  intptr_t data;
} CHAVE;

// No campo de chaves, a primeira coisa da struct da chave deve ser um int de desempate.
typedef struct ArvB {
  int nchaves, folha;
  CHAVE* chaves;
  struct ArvB **filho;
} TARVBMG;


// TARVBMG* TARVBMG_cria            (int t);
// TARVBMG* TARVBMG_inicializa      ();
// TARVBMG* TARVBMG_busca           (TARVBMG *a, CHAVE data, char (*cmp)(void*, void*));
// TARVBMG* TARVBMG_insere          (TARVBMG *T, CHAVE data, int t, char (*cmp)(void*, void*));
// TARVBMG* TARVBMG_retira          (TARVBMG* arv, CHAVE data, int t, char (*cmp)(void*, void*));
// void     TARVBMG_libera          (TARVBMG *a);
// void     TARVBMG_imprime         (const TARVBMG *a, void(*imprime)(void*));
//
// void     TARVBMG_imprime_chaves  (TARVBMG *a, void(*imprime)(void*));
//
// // mede by nathan 27/12/24
// void TARVBMG_json(TARVBMG* a, char* buffer, void(*imprime)(void*, char*));



#define PARCIAL_EQ ((char)-17)

#define PEQ(a, b) (cmp((a), (b)) == PARCIAL_EQ || cmp((a), (b)) == 0)
#define EQ(a, b)  (cmp((a), (b)) == 0)
#define NEQ(a, b) (cmp((a), (b)) != 0)
#define GT(a, b)  (cmp((a), (b)) > 0)
#define LT(a, b)  (cmp((a), (b)) < 0)

TARVBMG* TARVBMG_cria(int t){
  TARVBMG* novo = (TARVBMG*) falloc(sizeof(TARVBMG));
  novo->nchaves = 0;
  novo->chaves = (CHAVE*) falloc(sizeof(CHAVE)*(2*t-1));
  novo->folha=1;
  novo->filho = (TARVBMG**) falloc(sizeof(TARVBMG*)*t*2);
  for(int i = 0; i < t*2; i++) novo->filho[i] = NULL;
  return novo;
}


void TARVBMG_libera(TARVBMG *a) {
  if(a) {
    if(!a->folha){
      for(int i = 0; i <= a->nchaves; i++) TARVBMG_libera(a->filho[i]);
    }
    falloc_free(a->chaves);
    falloc_free(a->filho);
    falloc_free(a);
  }
}


void imp_rec(const TARVBMG *a, const int andar, int* len, void(*imprime)(void*)){
  if(a) {
    int i;
    for(i=0; i<=a->nchaves-1; i++){
      imp_rec(a->filho[i],andar+1, len, imprime);
      for(int j = 0; j<=andar; j++) printf("\t");
      //printf("%d\n", a->chaves[i]);
      imprime(&a->chaves[i]);
      printf("\n");
      (*len)++;
    }
    imp_rec(a->filho[i],andar+1, len, imprime);
  }
}


int TARVBMG_imprime(const TARVBMG *a, void(*imprime)(void*)) {
  int len = 0;
  imp_rec(a, 0, &len, imprime);
  return len;
}

TARVBMG *TARVBMG_busca(TARVBMG* x, CHAVE data, char (*cmp)(void*, void*)) {
  if(!x) return NULL;
  int i = 0;
  //while(i < x->nchaves && ch > x->chaves[i]) i++;
  while(i < x->nchaves && GT(&data, &x->chaves[i])) i++;
  
  //if(i < x->nchaves && ch == x->chaves[i]) return x;
  if(i < x->nchaves && PEQ(&data, &x->chaves[i])) return x;
  if(x->folha) return NULL;
  return TARVBMG_busca(x->filho[i], data, cmp);
}


TARVBMG *TARVBMG_inicializa(){
  return NULL;
}


TARVBMG *Divisao(TARVBMG *x, int i, TARVBMG* y, int t){
  TARVBMG *z= TARVBMG_cria(t);
  z->nchaves= t - 1;
  z->folha = y->folha;
  int j;
  for(j=0;j<t-1;j++) z->chaves[j] = y->chaves[j+t];
  if(!y->folha){
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--) x->chaves[j] = x->chaves[j-1];
  x->chaves[i-1] = y->chaves[t-1];
  x->nchaves++;
  return x;
}


TARVBMG *Insere_Nao_Completo(TARVBMG *x, CHAVE k, int t, char (*cmp)(void*, void*)) {
  int i = x->nchaves-1;
  if(x->folha){
    //while((i>=0) && (k<x->chaves[i])){
    while((i>=0) && LT(&k, &x->chaves[i])) {
      x->chaves[i+1] = x->chaves[i];
      i--;
    }
    x->chaves[i+1] = k;
    x->nchaves++;
    return x;
  }
  //while((i>=0) && (k<x->chaves[i])) i--;
  while((i>=0) && LT(&k, &x->chaves[i])) i--;
  i++;
  if(x->filho[i]->nchaves == 2*t-1){
    x = Divisao(x, (i+1), x->filho[i], t);
    //if(k>x->chaves[i]) i++;
    if(GT(&k, &x->chaves[i])) i++;
  }
  x->filho[i] = Insere_Nao_Completo(x->filho[i], k, t, cmp);
  return x;
}

TARVBMG *TARVBMG_insere(TARVBMG *T, CHAVE data, int t, char (*cmp)(void*, void*)) {
  if(TARVBMG_busca(T, data, cmp)) return T;
  if(!T) {
    T = TARVBMG_cria(t);
    T->chaves[0] = data;
    T->nchaves = 1;
    return T;
  }
  if(T->nchaves == 2*t-1) {
    TARVBMG *S = TARVBMG_cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    S = Divisao(S,1,T,t);
    S = Insere_Nao_Completo(S, data, t, cmp);
    return S;
  }
  T = Insere_Nao_Completo(T, data, t, cmp);
  return T;
}


TARVBMG *TARVBMG_Libera_Remocao(TARVBMG *a){
  falloc_free(a->chaves);
  falloc_free(a->filho);
  falloc_free(a);
  return NULL;
}


TARVBMG* remover(TARVBMG* arv, CHAVE ch, int t, char (*cmp)(void*, void*)) {
  if(!arv) return arv;
  int i;
  for(i = 0; i<arv->nchaves && LT(&arv->chaves[i], &ch); i++) {}
  if (i < arv->nchaves && EQ(&ch, &arv->chaves[i])) { //CASOS 1, 2A, 2B e 2C
    if (arv->folha) { //CASO 1
      //printf("\nCASO 1\n");
      for(int j = i; j < arv->nchaves-1; j++) {
        arv->chaves[j] = arv->chaves[j+1];
      }
      arv->nchaves--;
      if(!arv->nchaves){ //ultima revisao: 04/2020
        TARVBMG_libera(arv);
        arv = NULL;
      }
      return arv;
    }
    if (!arv->folha && arv->filho[i]->nchaves >= t) { //CASO 2A
      //printf("\nCASO 2A\n");
      const TARVBMG *y = arv->filho[i];  //Encontrar o predecessor k' de k na árvore com raiz em y
      while(!y->folha) y = y->filho[y->nchaves];
      const CHAVE temp = y->chaves[y->nchaves-1];
      arv->filho[i] = remover(arv->filho[i], temp, t, cmp);
      //Eliminar recursivamente K e substitua K por K' em x
      arv->chaves[i] = temp;
      return arv;
    }
    if(!arv->folha && arv->filho[i+1]->nchaves >= t) { //CASO 2B
      //printf("\nCASO 2B\n");
      const TARVBMG *y = arv->filho[i+1];  //Encontrar o sucessor k' de k na árvore com raiz em y
      while(!y->folha) y = y->filho[0];
      const CHAVE temp = y->chaves[0];
      y = remover(arv->filho[i+1], temp, t, cmp);
      arv->chaves[i] = temp;
      return arv;
    }
    if(!arv->folha && arv->filho[i+1]->nchaves == t-1 && arv->filho[i]->nchaves == t-1) { //CASO 2C
      //("\nCASO 2C\n");
      TARVBMG *y = arv->filho[i];
      TARVBMG *z = arv->filho[i+1];
      y->chaves[y->nchaves] = ch;          //colocar ch ao final de filho[i]
      int j;
      for(j=0; j<t-1; j++)                //juntar chaves[i+1] com chaves[i]
        y->chaves[t+j] = z->chaves[j];
      for(j=0; j<t; j++){                 //juntar filho[i+1] com filho[i]
        y->filho[t+j] = z->filho[j];
        z->filho[j] = NULL; //ultima revisao: 04/2020
      }
      y->nchaves = 2*t-1;
      for(j=i; j < arv->nchaves-1; j++)   //remover ch de arv
        arv->chaves[j] = arv->chaves[j+1];
      for(j=i+1; j < arv->nchaves; j++)
        arv->filho[j] = arv->filho[j+1]; //remover ponteiro para filho[i+1]
      arv->filho[j] = NULL;
      TARVBMG_Libera_Remocao(z);
      arv->nchaves--;
      if(!arv->nchaves){ //ultima revisao: 04/2020
        TARVBMG *temp = arv;
        arv = arv->filho[0];
        temp->filho[0] = NULL;
        TARVBMG_Libera_Remocao(temp);
        arv = remover(arv, ch, t, cmp);
      }
      else arv->filho[i] = remover(arv->filho[i], ch, t, cmp);
      return arv;
    }
  }

  TARVBMG *y = arv->filho[i], *z = NULL;
  if (y->nchaves == t-1){ //CASOS 3A e 3B
    if((i < arv->nchaves) && (arv->filho[i+1]->nchaves >=t)){ //CASO 3A
      //printf("\nCASO 3A: i menor que nchaves\n");
      z = arv->filho[i+1];
      y->chaves[t-1] = arv->chaves[i];   //dar a y a chaves i da arv
      y->nchaves++;
      arv->chaves[i] = z->chaves[0];     //dar a arv uma chaves de z
      int j;
      for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
        z->chaves[j] = z->chaves[j+1];
      y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
      for(j=0; j < z->nchaves; j++)       //ajustar filhos de z
        z->filho[j] = z->filho[j+1];
      z->nchaves--;
      arv->filho[i] = remover(arv->filho[i], ch, t, cmp);
      return arv;
    }
    if((i > 0) && (!z) && (arv->filho[i-1]->nchaves >=t)){ //CASO 3A
      //printf("\nCASO 3A: i igual a nchaves\n");
      z = arv->filho[i-1];
      int j;
      for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chaves
        y->chaves[j] = y->chaves[j-1];
      for(j = y->nchaves+1; j>0; j--)             //encaixar lugar dos filhos da nova chaves
        y->filho[j] = y->filho[j-1];
      y->chaves[0] = arv->chaves[i-1];              //dar a y a chaves i da arv
      y->nchaves++;
      arv->chaves[i-1] = z->chaves[z->nchaves-1];   //dar a arv uma chaves de z
      y->filho[0] = z->filho[z->nchaves];         //enviar ponteiro de z para o novo elemento em y
      z->nchaves--;
      arv->filho[i] = remover(y, ch, t, cmp);
      return arv;
    }
    if(!z){ //CASO 3B
      if(i < arv->nchaves && arv->filho[i+1]->nchaves == t-1){
        //printf("\nCASO 3B: i menor que nchaves\n");
        z = arv->filho[i+1];
        y->chaves[t-1] = arv->chaves[i];     //pegar chaves [i] e coloca ao final de filho[i]
        y->nchaves++;
        int j;
        for(j=0; j < t-1; j++){
          y->chaves[t+j] = z->chaves[j];     //passar filho[i+1] para filho[i]
          y->nchaves++;
        }
        if(!y->folha){
          for(j=0; j<t; j++){
            y->filho[t+j] = z->filho[j];
            z->filho[j] = NULL; //ultima revisao: 04/2020
          }
        }
        TARVBMG_libera(z);
        for(j=i; j < arv->nchaves-1; j++){ //limpar referências de i
          arv->chaves[j] = arv->chaves[j+1];
          arv->filho[j+1] = arv->filho[j+2];
        }
        arv->filho[arv->nchaves] = NULL;
        arv->nchaves--;
        if(!arv->nchaves){ //ultima revisao: 04/2020
          TARVBMG *temp = arv;
          arv = arv->filho[0];
          temp->filho[0] = NULL;
          TARVBMG_libera(temp);
        }
        arv = remover(arv, ch, t, cmp);
        return arv;
      }
      if((i > 0) && (arv->filho[i-1]->nchaves == t-1)){
        //printf("\nCASO 3B: i igual a nchaves\n");
        z = arv->filho[i-1];
        if(i == arv->nchaves)
          z->chaves[t-1] = arv->chaves[i-1]; //pegar chaves[i] e poe ao final de filho[i-1]
        else
          z->chaves[t-1] = arv->chaves[i];   //pegar chaves [i] e poe ao final de filho[i-1]
        z->nchaves++;
        int j;
        for(j=0; j < t-1; j++){
          z->chaves[t+j] = y->chaves[j];     //passar filho[i+1] para filho[i]
          z->nchaves++;
        }
        if(!z->folha){
          for(j=0; j<t; j++){
            z->filho[t+j] = y->filho[j];
            y->filho[j] = NULL; //ultima revisao: 04/2020
          }
        }
        TARVBMG_libera(y);
        arv->filho[arv->nchaves] = NULL;
        arv->nchaves--;
        if(!arv->nchaves){ //ultima revisao: 04/2020
          TARVBMG *temp = arv;
          arv = arv->filho[0];
          temp->filho[0] = NULL;
          TARVBMG_libera(temp);
        }
        else arv->filho[i-1] = z;
        arv = remover(arv, ch, t, cmp);
        return arv;
      }
    }
  }
  arv->filho[i] = remover(arv->filho[i], ch, t, cmp);
  return arv;
}


TARVBMG* TARVBMG_retira(TARVBMG* arv, CHAVE data, int t, char (*cmp)(void*, void*)) {
  if(!arv || !TARVBMG_busca(arv, data, cmp)) {
    printf("Chave não encontrada: '%s'\n", (char*)data.data);
    return arv;
  }
  return remover(arv, data, t, cmp);
}


// Made by nathan 23/01/24
void TARVBMG_json(TARVBMG* a, char* buffer, void(*imprime)(void*, char*)) {
  if(a){
    int i;
    for(i=0; i<=a->nchaves-1; i++){
      TARVBMG_json(a->filho[i], buffer, imprime);
      imprime(&a->chaves[i], buffer);
    }
    TARVBMG_json(a->filho[i], buffer, imprime);
  }
}

void TARVBMG_map(TARVBMG* a, void(*map)(void*)) {
  if(a){
    int i;
    for(i=0; i<=a->nchaves-1; i++){
      TARVBMG_map(a->filho[i], map);
      map(&a->chaves[i]);
    }
    TARVBMG_map(a->filho[i], map);
  }
}

void TARVBMG_map_range_2(TARVBMG* a, CHAVE min, CHAVE max, char (*cmp)(void*, void*), void(*map)(void*, void*), void* arg) {
  if (!a) return;

  int i;

  // Localiza a primeira chave que pode ser >= min
  for (i = 0; i < a->nchaves && LT(&a->chaves[i], &min) && !PEQ(&a->chaves[i], &max); i++);

  // Percorre as chaves na faixa min <= x <= max
  while (i < a->nchaves && (PEQ(&a->chaves[i], &max) || LT(&a->chaves[i], &max))) {
    if (!a->folha) {
      TARVBMG_map_range_2(a->filho[i], min, max, cmp, map, arg);
    }
    map(&a->chaves[i], arg);
    i++;
  }

  // Verifica o último filho caso a árvore não seja folha
  if (!a->folha) {
    TARVBMG_map_range_2(a->filho[i], min, max, cmp, map, arg);
  }
}

