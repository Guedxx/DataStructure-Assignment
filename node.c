//
// Created by nathan on 12/23/24.
//


#include <stdlib.h>
#include <stdbool.h>



#define NODE_SIZE(t) (sizeof(Node) + (2 * t - 1) * sizeof(int) + 2 * t * sizeof(int))
typedef struct Node {
    bool is_leaf;
    u_int32_t num_keys;
    u_int32_t* keys;
    u_int32_t* children; // Posição dos filhos no arquivo
} Node;

void Node_new(char* data, const bool is_leaf, const int t) {
    Node* node = (Node*) data;
    node->is_leaf = is_leaf;
    node->num_keys = 0;
    node->keys = (u_int32_t*) (data + sizeof(Node));
    node->children = (u_int32_t*) (data + sizeof(Node) + (2 * t - 1) * sizeof(u_int32_t));
}

