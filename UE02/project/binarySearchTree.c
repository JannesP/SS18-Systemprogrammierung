//
// Created by Jannes Peters on 01.04.2018.
//

#include <stdio.h>
#include <stdlib.h>
#include "binarySearchTree.h"

void Node_visit(Node* node) { printf("%s: %d", node->value, node->count); }

/**
 * Creates the start of the tree, just for convenience. Will be NULL in any case.
 * @return NULL
 */
Node* create() {
    return NULL;
}

int insert(Node** node, char* value, int (*compare)(const char *, const char *)) {
    if (*node == NULL) {
        *node = calloc(1, sizeof(Node));
        (*node)->value = value;
        (*node)->count = 1;
        return 0;
    } else {
        int cmp = compare((*node)->value, value);
        if (cmp < 0) {
            return insert(&((*node)->smaller), value, compare);
        } else if (cmp > 0) {
            return insert(&((*node)->bigger), value, compare);
        } else {
            (*node)->count++;
            return 1;
        }
    }
}

void print(Node *node) {
    if (node != NULL) {
        print(node->bigger);
        Node_visit(node);
        printf("\n");
        print(node->smaller);
    }
}



