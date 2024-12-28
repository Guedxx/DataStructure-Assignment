//
// Created by nathan non 06/10/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>


// /run/media/nathan/Acer/Users/miche/Videos/Series e Filmes
//char* base_file_path = "/run/media/nathan/Acer/Users/miche/Videos/Series e Filmes/%s";
char* base_file_path = "./files/%s";

int server_socket;
#include <signal.h>
void handle_sigint(int sig) {
    close(server_socket);
    printf("\nServidor encerrado.\n");
    exit(0);
}

void format_file_path(char *path) {
    // replace all %20 with space
    char *p = path;
    while ((p = strstr(p, "%20")) != NULL) {
        *p = ' ';
        memmove(p + 1, p + 3, strlen(p + 3) + 1);
    }
}


void send_file(const int client_socket, char *filename, const char *content_type) {
    char path[1024];
    format_file_path(filename);
    sprintf(path, base_file_path, filename);
    printf(path);
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        // Arquivo não encontrado
        const char *error_message = "HTTP/1.1 404 Not Found\r\n"
                                    "Content-Type: text/plain\r\n\r\n"
                                    "404 - File Not Found\n";
        write(client_socket, error_message, strlen(error_message));
        return;
    }

    // Enviar cabeçalho HTTP
    char header[1024];
    sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", content_type);
    write(client_socket, header, strlen(header));

    // Enviar conteúdo do arquivo
    char buffer[1024];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        write(client_socket, buffer, n);
    }

    fclose(file);
}

void handle_request(int client_socket) {
    char buffer[4096];
    const int n = read(client_socket, buffer, sizeof(buffer) - 1);
    if (n <= 0) return;
    buffer[n] = '\0';

    write(STDOUT_FILENO, "Requisição recebida:\n", 21);
    write(STDOUT_FILENO, buffer, n);

    // printf("Requisição recebida:\n%s\n", buffer);

    // Checar se é GET ou POST
    if (strncmp(buffer, "GET", 3) == 0) {
        // Tratar requisição GET
        if (strstr(buffer, "GET / ") != NULL) {
            send_file(client_socket, "index.html", "text/html");
        }
        else {
            char *filename = strtok(buffer + 5, " ");
            send_file(client_socket, filename, "text/plain");
        }
    } else if (strncmp(buffer, "POST", 4) == 0) {
        // Tratar requisição POST
        // Lógica para lidar com dados de um POST (ex: processar formulários)
        const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nPOST request received!";
        write(client_socket, response, strlen(response));
    } else {
        // Método não suportado
        const char *error_message = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/plain\r\n\r\n405 - Method Not Allowed\n";
        write(client_socket, error_message, strlen(error_message));
    }
}

void* thread_handle_request(void* arg) {
    int client_socket = (int) arg;
    handle_request(client_socket);
    close(client_socket);
    return NULL;
}


int main(const int argc, char *argv[]) {
    if (argc > 1) {
        base_file_path = argv[1];
    }

    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7789);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(server_socket, 10) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Servidor iniciado na porta 7789...\nhttp://localhost:7789/\n");

    signal(SIGTERM, handle_sigint);
    while (1) {
        const int client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &addr_len);

        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, thread_handle_request, (void*) client_socket) != 0) {
            perror("pthread_create");
            close(client_socket);
        } else {
            pthread_detach(thread_id); // Detach the thread to handle its own cleanup
        }
    }
}

