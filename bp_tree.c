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

    bp_tree.free_nodes[0] = 0;

    return bp_tree;
}

void BPTree_free(const BPTree* bp_tree) {
    munmap(bp_tree->nodes, NODE_SIZE(bp_tree->t));
    munmap(bp_tree->free_nodes, sizeof(int));
    close(bp_tree->node_fd);
    close(bp_tree->free_fd);
}

Node* BPTree_new_node(BPTree* bp_tree, const bool is_leaf) {
    if (bp_tree->free_nodes[0] == 0) {
        bp_tree->num_nodes++;
        ftruncate(bp_tree->node_fd, NODE_SIZE(bp_tree->t) * bp_tree->num_nodes);
        bp_tree->nodes = (char*) mremap(
            bp_tree->nodes, NODE_SIZE(bp_tree->t) * (bp_tree->num_nodes - 1), NODE_SIZE(bp_tree->t) * bp_tree->num_nodes, MREMAP_MAYMOVE);
    }
}

Node* BPTree_get_node(const BPTree* bp_tree, const size_t index) {
    if (index >= bp_tree->num_nodes) {
        return NULL;
    }
    return (Node*) (bp_tree->nodes + index * NODE_SIZE(bp_tree->t));
}

