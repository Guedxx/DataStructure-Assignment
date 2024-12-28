//
// Created by nathan on 12/26/24.
//

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

#define FILE_SIZE     (1024 * 4) // 4 KB (1 page)
#define FIXED_ADDRESS ((void*)0x40000000)

typedef struct block_header {
    size_t size;
    bool is_free;
} BlockHeader;

typedef struct falloc_context {
    void* base_addr;
    size_t total_size;
    int fd;
} FallocContext;

static FallocContext falloc_ctx;

void falloc_start(const char* file_name) {
    const int fd = open(file_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    falloc_ctx.fd = fd;

    size_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size < FILE_SIZE) {
        ftruncate(fd, FILE_SIZE);
    }

    falloc_ctx.total_size = FILE_SIZE > file_size ? FILE_SIZE : file_size;

    // Map the file at the fixed address
    falloc_ctx.base_addr = mmap(
        FIXED_ADDRESS,
        falloc_ctx.total_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_FIXED_NOREPLACE,
        fd, 0
    );
    if (falloc_ctx.base_addr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    // Initialize metadata if the file is empty
    BlockHeader* header = falloc_ctx.base_addr;
    if (header->size == 0) {
        header->size = falloc_ctx.total_size - sizeof(BlockHeader);
        header->is_free = true;
    }
}

void* falloc(const size_t size) {
    void* current = falloc_ctx.base_addr;
    while (current < falloc_ctx.base_addr + falloc_ctx.total_size) {
        BlockHeader* header = current;
        if (header->is_free && header->size >= size) {
            header->is_free = false;

            // dividir o bloco em dois se o tamanho for maior que o requisitado
            if (header->size > size + sizeof(BlockHeader)) {
                BlockHeader* next = (BlockHeader*)((char*)current + sizeof(BlockHeader) + size);
                next->size = header->size - size - sizeof(BlockHeader);
                next->is_free = true;
                header->size = size;
            }

            return (char*)current + sizeof(BlockHeader);
        }

        current = (char*)current + sizeof(BlockHeader) + header->size;
    }

    // allocar mais memória no arquivo
    size_t new_size = falloc_ctx.total_size + FILE_SIZE;
    if (ftruncate(falloc_ctx.fd, (long) new_size) == -1) {
        perror("ftruncate failed");
        exit(1);
    }
    falloc_ctx.total_size = new_size;

    falloc_ctx.base_addr = mremap(
        falloc_ctx.base_addr,
        falloc_ctx.total_size - FILE_SIZE,
        falloc_ctx.total_size,
        0
    );
    if (falloc_ctx.base_addr == MAP_FAILED || falloc_ctx.base_addr != FIXED_ADDRESS) {
        perror("mremap failed");
        exit(1);
    }

    BlockHeader* header = current;
    header->size = FILE_SIZE - sizeof(BlockHeader);
    header->is_free = true;

    return falloc(size);
}

void falloc_end() {
    msync(falloc_ctx.base_addr, falloc_ctx.total_size, MS_SYNC);
    munmap(falloc_ctx.base_addr, falloc_ctx.total_size);
    close(falloc_ctx.fd);
}

void falloc_free(void* ptr) {
    if (!ptr) return;

    BlockHeader* header = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));
    header->is_free = true;

    BlockHeader* next = (BlockHeader*)((char*)header + sizeof(BlockHeader) + header->size);
    while (next < falloc_ctx.base_addr + falloc_ctx.total_size && next->is_free) {
        header->size += next->size + sizeof(BlockHeader);

        next = (BlockHeader*)((char*)next + sizeof(BlockHeader) + next->size);
    }
}

void falloc_free_all() {
    BlockHeader* header = falloc_ctx.base_addr;
    header->is_free = true;
    header->size = falloc_ctx.total_size - sizeof(BlockHeader);
}

void* falloc_realloc(void* ptr, size_t size) {
    if (!ptr) return falloc(size);

    const BlockHeader* header = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));
    if (header->size >= size) {
        return ptr;
    }

    void* new_ptr = falloc(size);
    if (!new_ptr) { return NULL; }

    memcpy(new_ptr, ptr, header->size);
    falloc_free(ptr);

    return new_ptr;
}

uint32_t falloc_allocated_size() {
    void* current = falloc_ctx.base_addr;
    uint32_t total = 0;
    while (current < falloc_ctx.base_addr + falloc_ctx.total_size) {
        BlockHeader* header = current;
        if (!header->is_free) {
            total += header->size;
        }

        current = (char*)current + sizeof(BlockHeader) + header->size;
    }

    return total;
}
uint32_t falloc_free_size() {
    const uint32_t allocated = falloc_allocated_size();
    return falloc_ctx.total_size - allocated;
}



// write test
// int main() {
//     falloc_start("falloc_test.bin");
//
//     char* str = falloc(50);
//     strcpy(str, "Hello Nathan");
//     printf("%s\n", str);
//
//     str = falloc_realloc(str, 20);
//     strcat(str, " Pedro World Tuco");
//     printf("%s\n", str);
//
//     printf("Str at %p\n", str);
//
//     falloc_end();
//     return 0;
// }

// read test
// int main() {
//     falloc_start("falloc_test.bin");
//
//     char* str = (char*) 0x4000004e;
//     printf("%s\n", str);
//
//     falloc_end();
//     return 0;
// }