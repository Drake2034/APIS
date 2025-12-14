#include "binaryTree.h"
#include "stack.h"
#include "queue.h"

bt_node_t* createNode(void* data){
    bt_node_t* node = malloc(sizeof(*node));
    if(!node) return NULL;

    node->data = 0;
    node->rhs = node->lhs = NULL;

    return node;
}

bt_t* bt_create(void){
    bt_t* tree = malloc(sizeof(*tree));
    if(!tree) return NULL;

    tree->root = NULL;
    tree->size = 0;

    return tree;
}

tree_status_t bt_destroy(bt_t* tree){
    if(!tree) return TREE_ERR_NULL;

    if(!tree->root) return TREE_OK;
    bt_destroy(tree->root->lhs);
    bt_destroy(tree->root->rhs);
    free(tree->root->data);
    free(tree->root);
}
