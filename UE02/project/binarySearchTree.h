//
// Created by Jannes Peters on 01.04.2018.
//

#ifndef PROJECT_BINARYSEARCHTREE_H
#define PROJECT_BINARYSEARCHTREE_H

typedef struct Node {
    char* value;
    int count;
    struct Node* smaller;
    struct Node* bigger;
} Node;

void Node_visit(Node* node);

/**
 * Creates a new node in the heap.
 * @return the pointer to the node
 */
Node* create();
/**
 * Inserts the value into the tree.
 * @param start the pointer to the tree structure
 * @param value the value to be inserted into the tree
 * @param compare the function to copmpare the strings with
 * @return 0 if it was a new value and !=0 if not
 */
int insert(Node** node, char* value, int(*compare)(const char*, const char*));
/**
 * Traverses the tree and visits (prints) every node.
 * @param start the pointer to the tree structure or a subtree
 */
void print(Node* node);

#endif //PROJECT_BINARYSEARCHTREE_H
