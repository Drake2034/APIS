#ifndef BINARYSEARCHTREE_H
    #define BINARYSREARCHTREE_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdbool.h>

    typedef enum{
        BST_OK = 0,
        BST_ERR_ALLOC,
        BST_ERR_NULL,
        BST_EMPTY
    }bst_status_t;
    
    typedef struct bst_node{
        void* data;
        struct bst_node* lhs;
        struct bst_node* rhs;
    }bst_node_t;

    typedef struct bst{
        bst_node_t* root;
        size_t size;
    }bst_t;

    void free_node(bst_node_t* node);

    bst_t* bst_create(void);
    bst_status_t bst_destroy(bst_t* tree);

    bst_status_t bst_insert(bst_t* tree, void* data);

    bool bst_search(const bst_t* tree, void* data);
#endif