
#define _GNU_SOURCE

#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>


#include "node.c"

typedef struct BPTree {
    u_int32_t t;
    size_t root;

    size_t num_nodes;
    int node_fd;
    uintptr_t nodes;

    size_t num_free_nodes;
    int free_fd;
    int* free_nodes;

    char filename[32];
} BPTree;

BPTree BPTree_new(const int t, const char* filename) {
    if (strlen(filename) > 31) {
        perror("Filename too long");
        exit(1);
    }
    BPTree bp_tree;
    bp_tree.t = t;
    bp_tree.root = 0;
    bp_tree.num_nodes = 1;
    strcpy(bp_tree.filename, filename);

    // Nodes
    bp_tree.node_fd = open(filename, O_RDWR | O_CREAT);
    if (bp_tree.node_fd == -1) {
        perror("Cant open file: ");
        perror(filename);
        exit(1);
    }
    ftruncate(bp_tree.node_fd, NODE_SIZE(t));
    bp_tree.nodes = (uintptr_t) mmap(NULL, NODE_SIZE(t), PROT_READ | PROT_WRITE, MAP_SHARED, bp_tree.node_fd, 0);

    Node_new(bp_tree.nodes, true, t);

    // Free nodes
    char free_name[strlen(filename) + 5];
    strcpy(free_name, filename);
    strcat(free_name, ".free");
    bp_tree.free_fd = open(free_name, O_RDWR | O_CREAT);
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

void BPTree_save(const BPTree* bp_tree, const char* filename) {
    int fd = open(filename, O_RDWR | O_CREAT);
    if (fd == -1) {
        perror("Cant open file: ");
        perror(filename);
        exit(1);
    }
    write(fd , bp_tree, sizeof(BPTree));
    close(fd);
}

BPTree BPTree_load(const char* filename) {
    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("Cant open file: ");
        perror(filename);
        exit(1);
    }
    BPTree bp_tree;
    read(fd, &bp_tree, sizeof(BPTree));
    close(fd);

    bp_tree.node_fd = open(bp_tree.filename, O_RDWR | O_CREAT);
    if (bp_tree.node_fd == -1) {
        perror("Cant open file: ");
        perror(bp_tree.filename);
        exit(1);
    }
    bp_tree.nodes = (uintptr_t) mmap(NULL, NODE_SIZE(bp_tree.t) * bp_tree.num_nodes, PROT_READ | PROT_WRITE, MAP_SHARED, bp_tree.node_fd, 0);

    char free_name[strlen(bp_tree.filename) + 5];
    strcpy(free_name, bp_tree.filename);
    strcat(free_name, ".free");
    bp_tree.free_fd = open(free_name, O_RDWR | O_CREAT);
    if (bp_tree.free_fd == -1) {
        perror("Cant open file: ");
        perror(free_name);
        exit(1);
    }
    bp_tree.free_nodes = (int*) mmap(NULL, sizeof(int) * bp_tree.num_free_nodes, PROT_READ | PROT_WRITE, MAP_SHARED, bp_tree.free_fd, 0);

    return bp_tree;
}

void BPTree_free(const BPTree* bp_tree) {
    munmap((void*) bp_tree->nodes, NODE_SIZE(bp_tree->t) * bp_tree->num_nodes);
    munmap(bp_tree->free_nodes, sizeof(int) * bp_tree->num_free_nodes);
    close(bp_tree->node_fd);
    close(bp_tree->free_fd);
}

size_t BPTree_get_node_index(const BPTree* bp_tree, const Node* node) {
    return (size_t)((char*)node - bp_tree->nodes) / NODE_SIZE(bp_tree->t);
}

Node* BPTree_new_node(BPTree* bp_tree, const bool is_leaf) {
    const uint32_t t = bp_tree->t;
    if (bp_tree->num_free_nodes == 0) {
        msync(bp_tree->free_nodes, NODE_SIZE(t) * bp_tree->num_nodes, MS_SYNC);
        ftruncate(bp_tree->node_fd, NODE_SIZE(t) * (bp_tree->num_nodes + 1));
        bp_tree->nodes = (uintptr_t) mremap(
            (void*) bp_tree->nodes,
            NODE_SIZE(t) * bp_tree->num_nodes,
            NODE_SIZE(t) * (bp_tree->num_nodes + 1),
            MREMAP_MAYMOVE
        );
        bp_tree->num_nodes++;

        Node* node = (Node*) (bp_tree->nodes + NODE_SIZE(t) * (bp_tree->num_nodes - 1));
        Node_new((uintptr_t) node, is_leaf, t);
        return node;
    }
    // Else: Reuse free node
    bp_tree->num_free_nodes--;
    const size_t free_node_index = bp_tree->free_nodes[bp_tree->num_free_nodes];
    msync(bp_tree->free_nodes, sizeof(int) * bp_tree->num_free_nodes, MS_SYNC);
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
        perror("Index out of bounds");
        exit(1);
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
        while (i >= 0 && key < KEYS(node)[i]) {
            KEYS(node)[i + 1] = KEYS(node)[i];
            i--;
        }
        KEYS(node)[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && key < KEYS(node)[i]) {
            i--;
        }
        i++;

        Node* child = BPTree_get_node(bp_tree, CHILDREN(node, bp_tree->t)[i]);

        if (child->num_keys == 2 * bp_tree->t - 1) {
            BPTree_split_child(node, i, child, bp_tree);

            if (key > KEYS(node)[i]) {
                i++;
            }
        }

        BPTree_insert_non_full(BPTree_get_node(bp_tree, CHILDREN(node, bp_tree->t)[i]), key, bp_tree);
    }
}

void BPTree_split_child(Node* parent, int index, Node* child, BPTree* bp_tree) {
    const u_int32_t t = bp_tree->t;
    Node* new_node = BPTree_new_node(bp_tree, child->is_leaf);

    new_node->num_keys = t - 1;

    for (int j = 0; j < t - 1; j++) {
        u_int32_t pppp = KEYS(child)[j + t];
        KEYS(new_node)[j] = KEYS(child)[j + t];
    }

    if (!child->is_leaf) {
        for (int j = 0; j < t; j++) {
            CHILDREN(new_node, t)[j] = CHILDREN(child, t)[j + t];
        }
    }

    child->num_keys = t - 1;

    for (int j = parent->num_keys; j >= index + 1; j--) {
        CHILDREN(parent, t)[j + 1] = CHILDREN(parent, t)[j];
    }

    CHILDREN(parent, t)[index + 1] = BPTree_get_node_index(bp_tree, new_node);

    for (int j = parent->num_keys - 1; j >= index; j--) {
        KEYS(parent)[j + 1] = KEYS(parent)[j];
    }

    KEYS(parent)[index] = KEYS(child)[t - 1];
    parent->num_keys++;
}


void BPTree_insert(BPTree* bp_tree, const u_int32_t key) {
    Node* root = BPTree_get_node(bp_tree, bp_tree->root);

    if (root->num_keys == 2 * bp_tree->t - 1) {
        Node* new_root = BPTree_new_node(bp_tree, false);

        CHILDREN(new_root, bp_tree->t)[0] = bp_tree->root;
        bp_tree->root = BPTree_get_node_index(bp_tree, new_root);

        BPTree_split_child(new_root, 0, root, bp_tree);
        BPTree_insert_non_full(new_root, key, bp_tree);
    } else {
        BPTree_insert_non_full(root, key, bp_tree);
    }
}

void BPTree_remove(BPTree* bp_tree, const u_int32_t key) {
    Node* root = BPTree_get_node(bp_tree, bp_tree->root);
    BPTree_remove_key(bp_tree, root, key);

    if (root->num_keys == 0 && !root->is_leaf) {
        bp_tree->root = CHILDREN(root, bp_tree->t)[0];
        BPTree_free_node(bp_tree, BPTree_get_node_index(bp_tree, root));
    }
}

void BPTree_remove_key(BPTree* bp_tree, Node* node, u_int32_t key) {
    // Implementação da remoção detalhada.
    // Verifica se a chave está na folha, redistribui ou mescla nós quando necessário.
    // Detalhes omitidos para simplificar, mas deve incluir balanceamento conforme necessário.
}


void BPTree_print_node(const BPTree * bp_tree, Node * node, int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    printf(""
           "Node (%p):"
           "  is_leaf: %s"
           "  num_keys: %d",
           node,
           node->is_leaf ? "true" : "false",
           node->num_keys
    );

    printf("  Keys: [");
    for (int i = 0; i < node->num_keys; i++) {
        printf("%d", KEYS(node)[i]);
        if (i < node->num_keys - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    if (!node->is_leaf) {
        for (int i = 0; i < level; i++) {
            printf("  ");
        }

        printf("children: [\n");
        for (int i = 0; i < node->num_keys + 1; i++) {
            Node* child = BPTree_get_node(bp_tree, CHILDREN(node, bp_tree->t)[i]);
            BPTree_print_node(bp_tree, child, level + 1);
        }

        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("]\n");
    }
}

void BPTree_print(const BPTree* bp_tree) {
    Node* root = BPTree_get_node(bp_tree, bp_tree->root);
    printf(""
           "BPTree\n"
           "  t: %d\n"
           "  root: %zu\n"
           "  num_nodes: %zu\n"
           "  num_free_nodes: %zu\n"
           "\n",
           bp_tree->t, bp_tree->root, bp_tree->num_nodes, bp_tree->num_free_nodes
    );

    BPTree_print_node(bp_tree, root, 0);
}


// Teste
int main() {
    BPTree bp_tree = BPTree_new(2, "bptree_data.db");
    //BPTree bp_tree = BPTree_load("bptree.db");


    BPTree_insert(&bp_tree, 1);
    BPTree_insert(&bp_tree, 2);
    BPTree_insert(&bp_tree, 3);
    BPTree_insert(&bp_tree, 4);
    BPTree_insert(&bp_tree, 5);
    BPTree_insert(&bp_tree, 6);
    BPTree_insert(&bp_tree, 7);
    BPTree_insert(&bp_tree, 8);
    BPTree_insert(&bp_tree, 9);

    //BPTree_print(&bp_tree);

    BPTree_insert(&bp_tree, 10);

    BPTree_print(&bp_tree);

    //BPTree_save(&bp_tree, "bptree.db");

    BPTree_free(&bp_tree);

    return 0
}
