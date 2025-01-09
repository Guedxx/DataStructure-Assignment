//
// Created by nathan on 1/7/25.
//
#include "falloc.c"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "TL.c"
#include "BPT_IMV.c"
#include "WebServer.c"
//#include "TImoveis.c"
#include "BPT_IMV.c"

typedef struct {
    int t;
    BPT_IMV* imoveis;
} IMV_TREE_P_T;

TL* imoveis_list = NULL; // (IMV_TREE_P_T*)(imoveis_list->arr)[1]
void resetar_file() {
    falloc_free_all();
    TL* imoveis_list_n = falloc(sizeof(TL));
    if ((void*)imoveis_list_n != (((char*) FIXED_ADDRESS) + sizeof(BlockHeader))) {
        perror("falloc undefined behavior");
        exit(1);
    }
    TL new_tl = TL_new(sizeof(IMV_TREE_P_T));
    memcpy(imoveis_list_n, &new_tl, sizeof(TL));
    imoveis_list = imoveis_list_n;
}
void load_files() {
    imoveis_list = (void*) (((char*) FIXED_ADDRESS) + sizeof(BlockHeader));
}


int main() {
    printf(
    "Presado usuário, por favor, insira o caminho do arquivo que deseja abrir."
        "Caso não informado, será aberto o arquivo 'ALL_DATA.bin'.\n"
    );
    char buf[100];
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0'; // Remove newline character
    if (buf[0] == '\0') {
        strcpy(buf, "ALL_DATA.bin");
    }
    falloc_start(buf);
    printf("Presado usuário, você deseja resetar o arquivo? (s/N)\n");
    scanf("%s", buf);
    if (buf[0] == 's' || buf[0] == 'S' || buf[0] == 'y' || buf[0] == 'Y') {
        resetar_file();
    }
    else {
        load_files();
    }

    printf("Bom, usuário. O arquivo foi carregado com sucesso.\n");
    printf("E tem essas Árvores B+: [");
    for (size_t i = 0; i < imoveis_list->len; i++) {
        BPT_IMV* imv = *(BPT_IMV**) TL_get(imoveis_list, i);
        printf("%p", imv);
        if (i != imoveis_list->len - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    // printf("Usuário, qual árvore deseja acessar?\n Escreva o ponteiro da arvore mostrado acima: ");
    // scanf("%p", &imoveis);

    pthread_t thread_id;
    start_web_server_args args;
    args.server_directory[0] = '\0';
    args.port = 7789;
    printf("Tudo certo, usuário estamos iniciando o servidor!\n");
    if (pthread_create(&thread_id, NULL, start_web_server, &args) != 0) {
        perror("pthread_create");
    } else {
        pthread_detach(thread_id);
    }

    while (strncmp(buf, "sair", 4) != 0) {
        printf("Digite 'sair' para encerrar o programa.\n");
        printf("Digite 'ver' para ver as árvores.\n");
        printf("Digite 'add t' para adcionar uma arvore.\n");
        printf("Digite 'set arvore_pointer' para setar arvore_pointer como arvore atual");
        printf("Digite 'del arvore_pointer' para deletar uma arvore.\n");
        printf("Digite 'csv filename' para ler um arquivo csv.\n");
        fgets(buf, sizeof(buf), stdin);

        if (strncmp(buf, "ver", 3) == 0) {
            printf("\033[H\033[J");
            printf("Você tem essas Árvores B+: [");
            for (size_t i = 0; i < imoveis_list->len; i++) {
                IMV_TREE_P_T* imv = TL_get(imoveis_list, i);
                printf("'%p t: %d'", imv->imoveis, imv->t);
                if (i != imoveis_list->len - 1) {
                    printf(", ");
                }
            }
            printf("]\n");
            printf("Arvore atual: %p\n", imoveis);
        }
        else if (strncmp(buf, "add", 3) == 0) {
            int t;
            const char* t_str = strstr(buf, " ");
            if (t_str == NULL) {
                printf("\033[H\033[J");
                printf("Por favor, insira o valor de t.\n");
                continue;
            }
            sscanf(t_str, "%d", &t);
            BPT_IMV* new_imv = BPT_IMV_cria(t);
            IMV_TREE_P_T new_imv_t = {t, new_imv};
            TL_push(imoveis_list, &new_imv_t);
            imoveis = new_imv;
            imoveis_t = t;
        }
        else if (strncmp(buf, "del", 3) == 0) {
            const char* ptr_str = strstr(buf, " ");
            if (ptr_str == NULL) {
                printf("\033[H\033[J");
                printf("Por favor, insira o ponteiro da árvore que deseja deletar.\n");
                continue;
            }
            BPT_IMV* procurando;
            sscanf(ptr_str, "%p", &procurando);

            for (size_t i = 0; i < imoveis_list->len; i++) {
                IMV_TREE_P_T* imv = TL_get(imoveis_list, i);
                if (imv->imoveis == procurando) {
                    TL_set(imoveis_list, i, TL_get(imoveis_list, imoveis_list->len - 1));
                    TL_pop(imoveis_list);
                    falloc_free(imv->imoveis);
                    break;
                }
            }
        }
        else if (strncmp(buf, "csv", 3) == 0) {
            char* file_name = strstr(buf, " ");
            if (file_name == NULL) {
                printf("\033[H\033[J");
                printf("Por favor, insira o nome do arquivo csv que deseja ler.\n");
                continue;
            }
            // remove '\n'
            file_name[strlen(file_name) - 1] = '\0';
            Imovel_add_from_csv(file_name + 1);
        }
        else if (strncmp(buf, "set", 3) == 0) {
            const char* ptr_str = strstr(buf, " ");
            if (ptr_str == NULL) {
                printf("\033[H\033[J");
                printf("Por favor, insira o ponteiro da árvore que deseja setar.\n");
                continue;
            }
            BPT_IMV* procurando;
            sscanf(ptr_str, "%p", &procurando);

            for (size_t i = 0; i < imoveis_list->len; i++) {
                const IMV_TREE_P_T* imv = TL_get(imoveis_list, i);
                if (imv->imoveis == procurando) {
                    imoveis = imv->imoveis;
                    imoveis_t = imv->t;
                    printf("\033[H\033[J");
                    printf("Arvore setada com sucesso.\n");
                    break;
                }
            }
        }
        else {
            printf("\033[H\033[J");
            printf("Comando não reconhecido.\n");
        }
    }

    handle_sigint(0);
}