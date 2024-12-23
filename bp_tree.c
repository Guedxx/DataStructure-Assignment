//
// Created by nathan on 12/23/24.
//

#define _GNU_SOURCE

#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>


#include "node.c"

typedef struct BPTree {
    u_int32_t t;
    size_t root;

    size_t num_nodes;
    int node_fd;
    char* nodes;

    size_t num_free_nodes;
    int free_fd;
    int* free_nodes;
} BPTree;

BPTree BPTree_new(const int t, const char* filename) {
    BPTree bp_tree;
    bp_tree.t = t;
    bp_tree.root = 0;
    bp_tree.num_nodes = 1;

    // Nodes
    bp_tree.node_fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (bp_tree.node_fd == -1) {
        perror("Cant open file: ");
        perror(filename);
        exit(1);
    }
    ftruncate(bp_tree.node_fd, NODE_SIZE(t));
    bp_tree.nodes = (char*) mmap(NULL, NODE_SIZE(t), PROT_READ | PROT_WRITE, MAP_SHARED, bp_tree.node_fd, 0);

    Node_new(bp_tree.nodes, true, t);

    // Free nodes
    char free_name[strlen(filename) + 5];
    strcpy(free_name, filename);
    strcat(free_name, ".free");
    bp_tree.free_fd = open(free_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (bp_tree.free_fd == -1) {
        perror("Cant open file: ");
        perror(free_name);
        exit(1);
    }
    ftruncate(bp_tree.free_fd, sizeof(int));
    bp_tree.free_nodes = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, bp_tree.free_fd, 0);

    bp_tree.num_free_nodes = 0;

    return bp_tree;
}

void BPTree_free(const BPTree* bp_tree) {
    munmap(bp_tree->nodes, NODE_SIZE(bp_tree->t));
    munmap(bp_tree->free_nodes, sizeof(int));
    close(bp_tree->node_fd);
    close(bp_tree->free_fd);
}

Node* BPTree_new_node(BPTree* bp_tree, const bool is_leaf) {
    if (bp_tree->num_free_nodes == 0) {
        bp_tree->num_nodes++;

        ftruncate(bp_tree->node_fd, NODE_SIZE(bp_tree->t) * bp_tree->num_nodes);
        bp_tree->nodes = (char*) mremap(
            bp_tree->nodes,
            NODE_SIZE(bp_tree->t) * (bp_tree->num_nodes - 1),
            NODE_SIZE(bp_tree->t) * bp_tree->num_nodes,
            MREMAP_MAYMOVE
        );

        Node_new(bp_tree->nodes + NODE_SIZE(bp_tree->t) * (bp_tree->num_nodes - 1), is_leaf, bp_tree->t);
        return (Node*) (bp_tree->nodes + NODE_SIZE(bp_tree->t) * (bp_tree->num_nodes - 1));
    }

    bp_tree->num_free_nodes--;
    size_t free_node_index = bp_tree->free_nodes[bp_tree->num_free_nodes];
    ftruncate(bp_tree->free_fd, sizeof(int) * bp_tree->num_free_nodes);
    bp_tree->free_nodes = (int*) mremap(
        bp_tree->free_nodes,
        sizeof(int) * (bp_tree->num_free_nodes + 1),
        sizeof(int) * bp_tree->num_free_nodes,
        MREMAP_MAYMOVE
    );

    Node_new(bp_tree->nodes + NODE_SIZE(bp_tree->t) * free_node_index, is_leaf, bp_tree->t);
    return (Node*) (bp_tree->nodes + NODE_SIZE(bp_tree->t) * free_node_index);
}

Node* BPTree_get_node(const BPTree* bp_tree, const size_t index) {
    if (index >= bp_tree->num_nodes) {
        return NULL;
    }
    return (Node*) (bp_tree->nodes + index * NODE_SIZE(bp_tree->t));
}

void BPTree_free_node(BPTree* bp_tree, const size_t index) {
    bp_tree->num_free_nodes++;
    ftruncate(bp_tree->free_fd, sizeof(int) * (bp_tree->num_free_nodes + 1));
    bp_tree->free_nodes = (int*) mremap(
        bp_tree->free_nodes,
        sizeof(int) * bp_tree->num_free_nodes,
        sizeof(int) * (bp_tree->num_free_nodes + 1),
        MREMAP_MAYMOVE
    );

    bp_tree->free_nodes[bp_tree->num_free_nodes - 1] = (int) index;
}



// Chat: 23/12/2024

void BPTree_split_child(Node* parent, int index, Node* child, BPTree* bp_tree);
void BPTree_remove_key(BPTree* bp_tree, Node* node, u_int32_t key);

void BPTree_insert_non_full(Node* node, u_int32_t key, BPTree* bp_tree) {
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;

        Node* child = BPTree_get_node(bp_tree, node->children[i]);

        if (child->num_keys == 2 * bp_tree->t - 1) {
            BPTree_split_child(node, i, child, bp_tree);

            if (key > node->keys[i]) {
                i++;
            }
        }

        BPTree_insert_non_full(BPTree_get_node(bp_tree, node->children[i]), key, bp_tree);
    }
}

void BPTree_split_child(Node* parent, int index, Node* child, BPTree* bp_tree) {
    int t = bp_tree->t;
    Node* new_node = BPTree_new_node(bp_tree, child->is_leaf);

    new_node->num_keys = t - 1;

    for (int j = 0; j < t - 1; j++) {
        new_node->keys[j] = child->keys[j + t];
    }

    if (!child->is_leaf) {
        for (int j = 0; j < t; j++) {
            new_node->children[j] = child->children[j + t];
        }
    }

    child->num_keys = t - 1;

    for (int j = parent->num_keys; j >= index + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[index + 1] = (u_int32_t)((char*)new_node - bp_tree->nodes) / NODE_SIZE(bp_tree->t);

    for (int j = parent->num_keys - 1; j >= index; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    parent->keys[index] = child->keys[t - 1];
    parent->num_keys++;
}

void BPTree_insert(BPTree* bp_tree, u_int32_t key) {
    Node* root = BPTree_get_node(bp_tree, bp_tree->root);

    if (root->num_keys == 2 * bp_tree->t - 1) {
        Node* new_root = BPTree_new_node(bp_tree, false);
        new_root->children[0] = bp_tree->root;
        bp_tree->root = (u_int32_t)((char*)new_root - bp_tree->nodes) / NODE_SIZE(bp_tree->t);

        BPTree_split_child(new_root, 0, root, bp_tree);
        BPTree_insert_non_full(new_root, key, bp_tree);
    } else {
        BPTree_insert_non_full(root, key, bp_tree);
    }
}

void BPTree_remove(BPTree* bp_tree, u_int32_t key) {
    Node* root = BPTree_get_node(bp_tree, bp_tree->root);
    BPTree_remove_key(bp_tree, root, key);

    if (root->num_keys == 0 && !root->is_leaf) {
        bp_tree->root = root->children[0];
        BPTree_free_node(bp_tree, (size_t)((char*)root - bp_tree->nodes) / NODE_SIZE(bp_tree->t));
    }
}

void BPTree_remove_key(BPTree* bp_tree, Node* node, u_int32_t key) {
    // Implementação da remoção detalhada.
    // Verifica se a chave está na folha, redistribui ou mescla nós quando necessário.
    // Detalhes omitidos para simplificar, mas deve incluir balanceamento conforme necessário.
}



