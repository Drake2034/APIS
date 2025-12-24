#include "bst.h"
#include "stack.h"
#include "queue.h"

bst_node_t* create_node(void* data){
    bst_node_t* node = malloc(sizeof *node);
    if(!node) return NULL;

    node->data = data;
    node->rhs = node->lhs = NULL;

    return node;
}

void free_node(bst_node_t* node){
    if(!node) return;

    node->data = NULL;
    node->lhs = node->rhs = NULL;

    free(node);
}

bst_t* bst_create(void){
    bst_t* tree = malloc(sizeof *tree);
    if(!tree) return NULL;

    tree->root = NULL;
    tree->size = 0;

    return tree;
}

bst_status_t bst_destroy(bst_t* tree){
    if(!tree) return BST_ERR_NULL;

    if(!tree->root) return BST_OK;

    bt_destroy(tree->root->lhs);
    bt_destroy(tree->root->rhs);

    free(tree->root->data);
    free(tree->root);
}

bst_status_t bst_insert(bst_t* tree, void* data){
    if(!tree) return BST_ERR_NULL;


}