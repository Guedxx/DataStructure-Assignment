//
// Created by nathan on 02/10/2024.
//
#include "falloc.c"

#include <stdlib.h>

typedef struct type_list {
    size_t len;
    size_t cap;
    size_t data_size;
    char* arr;
} TL;

TL TL_new(const size_t data_size) {
    TL tl;

    tl.len = 0;
    tl.cap = 2;
    tl.data_size = data_size;
    tl.arr = (char*) falloc(data_size * tl.cap);

    return tl;
}

void TL_free(const TL* tl) {
    falloc_free(tl->arr);
}

void TL_push(TL* tl, const void* data) {
    if (tl->len >= tl->cap) {
        tl->cap *= 2;
        tl->arr = (char*) falloc_realloc(tl->arr, tl->data_size * tl->cap);
    }

    for (size_t i = 0; i < tl->data_size; i++) {
        tl->arr[tl->len * tl->data_size + i] = ((char*) data)[i];
    }

    tl->len++;
}

void* TL_pop(TL* tl) {
    tl->len--;
    return tl->arr + tl->len * tl->data_size;
}

void* TL_get(const TL* tl, const size_t index) {
    return tl->arr + index * tl->data_size;
}

void TL_set(TL* tl, const size_t index, const void* data) {
    for (size_t i = 0; i < tl->data_size; i++) {
        tl->arr[index * tl->data_size + i] = ((char*) data)[i];
    }
}
