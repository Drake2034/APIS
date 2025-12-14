#ifndef BINARYTREE_H
    #define BINARYTREE_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdbool.h>

    typedef enum{
        TREE_OK = 0,
        TREE_ERR_ALLOC,
        TREE_ERR_NULL,
        TREE_EMPTY
    }tree_status_t;
    
    typedef struct bt_node{
        void* data;
        bt_node_t* lhs;
        bt_node_t* rhs;
    }bt_node_t;

    typedef struct bt{
        bt_node_t* root;
        bt_node_t** leaves;
        size_t size;
    }bt_t;
#endif