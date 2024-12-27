#include "FTARVBM.h"

TARVBM *TARVBM_cria(int t){
  TARVBM* novo = (TARVBM*)falloc(sizeof(TARVBM));
  novo->nchaves = 0;
  novo->chave =(int*)falloc(sizeof(int)*((t*2)-1));
  novo->folha = 1;
  novo->filho = (TARVBM**)falloc(sizeof(TARVBM*)*t*2);
  novo->prox = NULL;
  int i;
  for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
  return novo;
}


TARVBM *TARVBM_inicializa(void) {
  return NULL;
}

void TARVBM_libera(TARVBM *a){
  if(a){
    if(!a->folha){
      int i;
      for(i = 0; i <= a->nchaves; i++) TARVBM_libera(a->filho[i]);
    }
    falloc_free(a->filho);
    falloc_free(a->chave);
    falloc_free(a);
  }
}

TARVBM *TARVBM_busca(TARVBM *a, int mat){
  if (!a) return NULL;
  int i = 0;
  while ((i < a->nchaves) && (mat > a->chave[i])) i++;
  if ((i < a->nchaves) && (a->folha) && (mat == a->chave[i])) return a;
  if (a-> folha) return NULL;
  if (a->chave[i] == mat) i++;
  return TARVBM_busca(a->filho[i], mat);
}


void TARVBM_imprime_chaves(TARVBM *a){
  if(!a) return;
  TARVBM *p = a;
  while(p->filho[0]) p = p->filho[0];
  while(p){
    int i;
    for(i = 0; i < p->nchaves; i++) printf("%d ", p->chave[i]);
    p = p->prox;
  }
  printf("\n");
}


void imp(TARVBM *a, int andar){
  if(a){
    int i,j;
    imp(a->filho[a->nchaves],andar+1);
    for(i=a->nchaves-1; i >= 0; i--){
      for(j=0; j<=andar; j++) printf("\t");
      printf("%d\n", a->chave[i]);
      imp(a->filho[i],andar+1);
    }
  }
}

void TARVBM_imprime(TARVBM *a){
  imp(a, 0);
}


void imp0(TARVBM *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      imp(a->filho[i],andar+1);
      for(j=0; j<=andar; j++) printf("\t");
      printf("%d\n", a->chave[i]);
    }
    imp(a->filho[i],andar+1);
  }
}

void TARVBM_imprime0(TARVBM *a){
  imp0(a, 0);
}

TARVBM *divisao(TARVBM *x, int i, TARVBM* y, int t){
  TARVBM *z = TARVBM_cria(t);
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
    for(j=0;j < t;j++) z->chave[j] = y->chave[j+t-1];//Caso em que y é folha, temos q passar a info para o nó da direita
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


TARVBM *insere_nao_completo(TARVBM *x, int mat, int t){
  int i = x->nchaves-1;
  if(x->folha){
    while((i>=0) && (mat < x->chave[i])){
      x->chave[i+1] = x->chave[i];
      i--;
    }
    x->chave[i+1] = mat;
    x->nchaves++;
    return x;
  }
  while((i>=0) && (mat < x->chave[i])) i--;
  i++;
  if(x->filho[i]->nchaves == ((2*t)-1)){
    x = divisao(x, (i+1), x->filho[i], t);
    if(mat > x->chave[i]) i++;
  }
  x->filho[i] = insere_nao_completo(x->filho[i], mat, t);
  return x;
}

TARVBM *TARVBM_insere(TARVBM *T, int mat, int t){
  if(TARVBM_busca(T, mat)) return T;
  if(!T){
    T=TARVBM_cria(t);
    T->chave[0] = mat;
    T->nchaves=1;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TARVBM *S = TARVBM_cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    S = divisao(S,1,T,t);
    S = insere_nao_completo(S, mat, t);
    return S;
  }
  T = insere_nao_completo(T, mat, t);
  return T;
}


TARVBM* remover(TARVBM* arv, int ch, int t){
  if(!arv) return arv;
  int i;
  for(i = 0; i < arv->nchaves && arv->chave[i] < ch; i++);
  if((i < arv->nchaves) && (ch == arv->chave[i]) && (arv->folha)){ //CASO 1
    printf("\nCASO 1\n");
    int j;
    for(j=i; j<arv->nchaves-1;j++) arv->chave[j] = arv->chave[j+1];
    arv->nchaves--;
    if(!arv->nchaves){ //ultima revisao: 04/2020
      TARVBM_libera(arv);
      arv = NULL;
    }
    return arv;
  }

  if((i < arv->nchaves) && (ch == arv->chave[i])) i++;
  TARVBM *y = arv->filho[i], *z = NULL;
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
      arv->filho[i] = remover(arv->filho[i], ch, t);
      return arv;
    }
    if((i > 0) && (!z) && (arv->filho[i-1]->nchaves >= t)){ //CASO 3A
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
      arv->filho[i] = remover(y, ch, t);
      return arv;
    }
    if(!z){ //CASO 3B
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
          //TARVBM_libera(z); 07/2024
        }
        TARVBM_libera(z); // 07/2024
        for(j=i; j < arv->nchaves-1; j++){ //limpar referências de i
          arv->chave[j] = arv->chave[j+1];
          arv->filho[j+1] = arv->filho[j+2];
        }
        arv->filho[arv->nchaves] = NULL;
        arv->nchaves--;
        if(!arv->nchaves){ //ultima revisao: 04/2020
          TARVBM *temp = arv;
          arv = arv->filho[0];
          temp->filho[0] = NULL;
          TARVBM_libera(temp);
        }
        arv = remover(arv, ch, t);
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
          //TARVBM_libera(y); 07/2024
        }
        TARVBM_libera(y); // 07/2024
        arv->filho[arv->nchaves] = NULL;
        arv->nchaves--;
        if(!arv->nchaves){ //ultima revisao: 04/2020
          TARVBM *temp = arv;
          arv = arv->filho[0];
          temp->filho[0] = NULL;
          TARVBM_libera(temp);
          arv = remover(arv, ch, t);
        }
        else{
          i--;
          arv->filho[i] = remover(arv->filho[i], ch, t);
        }
        return arv;
      }
    }
  }
  arv->filho[i] = remover(arv->filho[i], ch, t);
  return arv;
}


TARVBM* TARVBM_retira(TARVBM* arv, int k, int t){
  if(!arv || !TARVBM_busca(arv, k)) return arv;
  return remover(arv, k, t);
}
