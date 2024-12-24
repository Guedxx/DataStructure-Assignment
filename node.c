#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define KEYS(node) ((u_int32_t*) (node + 1))
#define CHILDREN(node, t) ( \
    ( \
        ((u_int32_t*) (node + 1)) + (2 * t - 1) \
    ) \
)


#define NODE_SIZE(t) (sizeof(Node) + ((2 * (t) - 1) * sizeof(u_int32_t)) + (2 * (t) * sizeof(u_int32_t)))

typedef struct Node {
    bool is_leaf;
    u_int32_t num_keys;
    //u_int32_t keys[];     // Chaves será convertida para string para encontrar o arquivo
    // u_int32_t* children; // Posição dos filhos no arquivo
} Node;

void Node_new(uintptr_t data, const bool is_leaf, const size_t t) {
    Node* node = (Node*) data;
    node->is_leaf = is_leaf;
    node->num_keys = 0;
    // node->keys = (u_int32_t*) (data + sizeof(Node));
    // node->children = (u_int32_t*) (data + sizeof(Node) + (2 * t - 1) * sizeof(u_int32_t));
}

void Node_copy(Node* dest, const Node* src, const size_t t) {
    dest->is_leaf = src->is_leaf;
    dest->num_keys = src->num_keys;
    memcpy(KEYS(dest), KEYS(src), (2 * t - 1) * sizeof(u_int32_t));
    memcpy(CHILDREN(dest, t), CHILDREN(src, t), 2 * t * sizeof(u_int32_t));
}
