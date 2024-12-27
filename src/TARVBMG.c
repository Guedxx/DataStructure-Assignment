#include "TARVBMG.h"

TARVBMG *TARVBMG_cria(const int t){
  TARVBMG* novo = (TARVBMG*) falloc(sizeof(TARVBMG));
  novo->nchaves = 0;
  novo->chave = (void**) falloc(sizeof(void*)*((t*2)-1));
  novo->folha = 1;
  novo->filho = (TARVBMG**)falloc(sizeof(TARVBMG*)*t*2);
  novo->prox = NULL;
  for(int i = 0; i<(t*2); i++) novo->filho[i] = NULL;
  return novo;
}


TARVBMG *TARVBMG_inicializa() {
  return NULL;
}

void TARVBMG_libera(TARVBMG *a){
  if(a){
    if(!a->folha){
      for(int i = 0; i <= a->nchaves; i++) TARVBMG_libera(a->filho[i]);
    }
    falloc_free(a->filho);
    for (int i = 0; i < a->nchaves; i++) {
      falloc_free(a->chave[i]);
    }
    falloc_free(a->chave);
    falloc_free(a);
  }
}

TARVBMG *TARVBMG_busca(TARVBMG *a, void* data, bool (*menor_que)(void*, void*)){
  if (!a) return NULL;
  int i = 0;

  // while ((i < a->nchaves) && (mat > a->chave[i])) i++;
  while ((i < a->nchaves) && GT(data, a->chave[i])) i++;

  // if ((i < a->nchaves) && (a->folha) && (mat == a->chave[i])) return a;
  if ((i < a->nchaves) && (a->folha) && EQ(data, a->chave[i])) return a;

  if (a-> folha) return NULL;

  // if (a->chave[i] == mat) i++; // Nathan acha que tem undefined behavior neste código da rosset, testar depois
  if ((i < a->nchaves) && EQ(data, a->chave[i])) i++;

  return TARVBMG_busca(a->filho[i], data, menor_que);
}


void TARVBMG_imprime_chaves(TARVBMG *a, void(*imprime)(void*)){
  if(!a) return;
  TARVBMG *p = a;
  while(p->filho[0]) p = p->filho[0];
  while(p){
    // for(i = 0; i < p->nchaves; i++) printf("%d ", p->chave[i]);
    for(int i = 0; i < p->nchaves; i++) imprime(p->chave[i]);

    p = p->prox;
  }
  printf("\n");
}


void imp(const TARVBMG *a, const int andar, void(*imprime)(void*)){
  if(a){
    // imp(a->filho[a->nchaves],andar+1);
    imp(a->filho[0],andar+1, imprime);


    for(int i = a->nchaves - 1; i >= 0; i--){
      for(int j = 0; j<=andar; j++) printf("\t");

      // printf("%d\n", a->chave[i]);
      // imp(a->filho[i],andar+1);
      imprime(a->chave[i]); printf("\n");
      imp(a->filho[i+1],andar+1, imprime);
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
    for(j=0;j<t-1;j++) z->chave[j] = y->chave[j+t];
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }
  else {
    z->nchaves = t; //z possuirá uma chave a mais que y se for folha
    for(j=0;j < t;j++) z->chave[j] = y->chave[j+t-1];//Caso em que y é folha, temos que passar a info para o nó da direita
    z->prox = y->prox; //ultima revisao: 04/2020
    y->prox = z;
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
  x->chave[i-1] = y->chave[t-1];
  x->nchaves++;
  return x;
}


TARVBMG *insere_nao_completo(TARVBMG *x, void* data, int t, bool (*menor_que)(void*, void*)) {
  int i = x->nchaves-1;
  if(x->folha){
    // while((i>=0) && (mat < x->chave[i])){
    while((i>=0) && LT(data, x->chave[i])){
      x->chave[i+1] = x->chave[i];
      i--;
    }
    // x->chave[i+1] = mat;
    x->chave[i+1] = data;
    x->nchaves++;
    return x;
  }
  // while((i>=0) && (mat < x->chave[i])) i--;
  while((i>=0) && LT(data, x->chave[i])) i--;
  i++;
  if(x->filho[i]->nchaves == ((2*t)-1)){
    x = divisao(x, (i+1), x->filho[i], t);
    // if(mat > x->chave[i]) i++;
    if(GT(data, x->chave[i])) i++;
  }
  //x->filho[i] = insere_nao_completo(x->filho[i], mat, t);
  x->filho[i] = insere_nao_completo(x->filho[i], data, t, menor_que);
  return x;
}

// data tem que ser alocado dinamicamente
TARVBMG *TARVBMG_insere(TARVBMG *T, void* data, int t, bool (*menor_que)(void*, void*)) {
  // if(TARVBMG_busca(T, mat)) return T;
  if(TARVBMG_busca(T, data, menor_que)) return T;

  if(!T){
    T=TARVBMG_cria(t);
    // T->chave[0] = mat;
    T->chave[0] = data;
    T->nchaves=1;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TARVBMG *S = TARVBMG_cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    S = divisao(S,1,T,t);
    // S = insere_nao_completo(S, mat, t);
    S = insere_nao_completo(S, data, t, menor_que);
    return S;
  }
  // T = insere_nao_completo(T, mat, t);
  T = insere_nao_completo(T, data, t, menor_que);
  return T;
}


TARVBMG* remover(TARVBMG* arv, void* data, int t, bool (*menor_que)(void*, void*)) {
  if(!arv) return arv;
  int i;

  // for(i = 0; i < arv->nchaves && arv->chave[i] < ch; i++);
  for (i = 0; i < arv->nchaves && LT(arv->chave[i], data); i++) {}

  // if((i < arv->nchaves) && (ch == arv->chave[i]) && (arv->folha)){ //CASO 1
  if((i < arv->nchaves) && EQ(data, arv->chave[i]) && arv->folha){ //CASO 1
    printf("\nCASO 1\n");
    for(int j = i; j<arv->nchaves-1;j++) arv->chave[j] = arv->chave[j+1];
    arv->nchaves--;
    if(!arv->nchaves){ //ultima revisao: 04/2020
      TARVBMG_libera(arv);
      arv = NULL;
    }
    return arv;
  }

  // if((i < arv->nchaves) && (ch == arv->chave[i])) i++;
  if((i < arv->nchaves) && EQ(data, arv->chave[i])) i++;
  TARVBMG *y = arv->filho[i], *z = NULL;
  if (y->nchaves == t-1){ //CASOS 3A e 3B
    if((i < arv->nchaves) && (arv->filho[i+1]->nchaves >=t)){ //CASO 3A
      printf("\nCASO 3A: i menor que nchaves\n");
      z = arv->filho[i+1];
      if(!y->folha){
        y->chave[t-1] = arv->chave[i];   //dar a y a chave i da arv
        arv->chave[i] = z->chave[0];     //dar a arv uma chave de z
      }
      else{
        arv->chave[i] = z->chave[0] + 1;
        y->chave[t-1] = z->chave[0];
      }
      y->nchaves++;

      int j;
      for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
        z->chave[j] = z->chave[j+1];
      y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
      for(j=0; j < z->nchaves; j++)       //ajustar filhos de z
        z->filho[j] = z->filho[j+1];
      z->nchaves--;
      // arv->filho[i] = remover(arv->filho[i], ch, t);
      arv->filho[i] = remover(arv->filho[i], data, t, menor_que);
      return arv;
    }
    if((i > 0) && (arv->filho[i-1]->nchaves >= t)){ //CASO 3A
      printf("\nCASO 3A: i igual a nchaves\n");
      z = arv->filho[i-1];
      int j;
      for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
        y->chave[j] = y->chave[j-1];
      for(j = y->nchaves+1; j>0; j--) //encaixar lugar dos filhos da nova chave
        y->filho[j] = y->filho[j-1];

      if(!y->folha){
        y->chave[0] = arv->chave[i-1];    //dar a y a chave i da arv
        arv->chave[i-1] = z->chave[z->nchaves - 1];   //dar a arv uma chave de z
      }
      else{
        arv->chave[i-1] = z->chave[z->nchaves - 1];
        y->chave[0] = z->chave[z->nchaves-1];
      }
      y->nchaves++;

      //enviar ponteiro de z para o novo elemento em y
      y->filho[0] = z->filho[z->nchaves];
      z->nchaves--;
      // arv->filho[i] = remover(y, ch, t);
      arv->filho[i] = remover(y, data, t, menor_que);
      return arv;
    }
    //CASO 3B
    if(i < arv->nchaves && arv->filho[i+1]->nchaves == t-1){
      printf("\nCASO 3B: i menor que nchaves\n");
      z = arv->filho[i+1];
      if(!y->folha){
        y->chave[t-1] = arv->chave[i];//pegar chave [i] e coloca ao final de filho[i]
        y->nchaves++;
      }
      int j = 0;
      while(j < t-1){
        if(!y->folha) y->chave[t+j] = z->chave[j];
        else y->chave[t+j-1] = z->chave[j];
        y->nchaves++;
        j++;
      }
      y->prox = z->prox;
      if(!y->folha){
        for(j=0; j<t; j++){
          y->filho[t+j] = z->filho[j];
          z->filho[j] = NULL; //ultima revisao: 04/2020
        }
        //TARVBMG_libera(z); 07/2024
      }
      TARVBMG_libera(z); // 07/2024
      for(j=i; j < arv->nchaves-1; j++){ //limpar referências de i
        arv->chave[j] = arv->chave[j+1];
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
      // arv = remover(arv, ch, t);
      arv = remover(arv, data, t, menor_que);
      return arv;
    }
    if((i > 0) && (arv->filho[i-1]->nchaves == t-1)){
      printf("\nCASO 3B: i igual a nchaves\n");
      z = arv->filho[i-1];
      if(!y->folha){
        if(i == arv->nchaves){
          z->chave[t-1] = arv->chave[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
        }else{
          z->chave[t-1] = arv->chave[i];   //pegar chave [i] e poe ao final de filho[i-1]
        }
        z->nchaves++;
      }
      int j = 0;
      while(j < t-1){
        if(!y->folha) z->chave[t+j] = y->chave[j];
        else z->chave[t+j-1] = y->chave[j];
        z->nchaves++;
        j++;
      }
      z->prox = y->prox;
      if(!z->folha){
        for(j=0; j<t; j++){
          z->filho[t+j] = y->filho[j];
          y->filho[j] = NULL; //ultima revisao: 04/2020
        }
        //TARVBMG_libera(y); 07/2024
      }
      TARVBMG_libera(y); // 07/2024
      arv->filho[arv->nchaves] = NULL;
      arv->nchaves--;
      if(!arv->nchaves){ //ultima revisao: 04/2020
        TARVBMG *temp = arv;
        arv = arv->filho[0];
        temp->filho[0] = NULL;
        TARVBMG_libera(temp);
        //arv = remover(arv, ch, t);
        arv = remover(arv, data, t, menor_que);
      }
      else{
        i--;
        //arv->filho[i] = remover(arv->filho[i], ch, t);
        arv->filho[i] = remover(arv->filho[i], data, t, menor_que);
      }
      return arv;
    }
  }
  //arv->filho[i] = remover(arv->filho[i], ch, t);
  arv->filho[i] = remover(arv->filho[i], data, t, menor_que);
  return arv;
}


TARVBMG* TARVBMG_retira(TARVBMG* arv, void* data, int t, bool (*menor_que)(void*, void*)) {
  //if(!arv || !TARVBMG_busca(arv, k)) return arv;
  if(!arv || !TARVBMG_busca(arv, data, menor_que)) return arv;
  //return remover(arv, k, t);
  return remover(arv, data, t, menor_que);
}
