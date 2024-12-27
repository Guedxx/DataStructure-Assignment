//
// Created by nathan on 12/26/24.
//

#include "TARVBMG.c"

// testes write
// int main() {
//     falloc_start("TARVBM_test.bin");
//     TARVBMG *a = TARVBM_inicializa();
//
//     for (int i = 9; i < 55; i += 2) {
//         a = TARVBM_insere(a, i, 2);
//     }
//
//     TARVBM_imprime(a);
//     // TARVBM_libera(a);
//
//     printf("Arvore adr: %p\n", a);
//
//     falloc_end();
//     return 0;
// }

// testes read
// int main() {
//     falloc_start("TARVBM_test.bin");
//     // TARVBM *a = (TARVBM*) 0x4000027c;
//     TARVBM *a = (TARVBM*) 0x40001adc;
//
//     TARVBM_imprime(a);
//     TARVBM_libera(a);
//
//     falloc_end();
//     return 0;
// }