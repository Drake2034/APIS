#include "bst.h"
#include "stack.h"
#include "queue.h"

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

bst_node_t* bst_search_node(bst_node_t* root, void* data){
    while(root){
        if(root->data == data) return root;
        root = root->data > data ? root->lhs : root->rhs;
    }
    return NULL;
}

bool bst_search(const bst_t* tree, void* data){
    return bst_search_node(tree->root, data) ? true : false;
}

bst_node_t* bst_insert_node(bst_node_t* root, void* data){
    if(!root){
        bst_node_t* new = malloc(sizeof *new);
        if(!new) return NULL;

        new->data = data;
        new->lhs = new->rhs = NULL;

        return new;
    }

    if(root->data < data){
        bst_insert(root->lhs, data);
    }else if(root->data > data){
        bst_insert(root->rhs, data);
    }

    return root;
}

bst_status_t bst_insert(bst_t* tree, void* data){
    if(!tree) return BST_ERR_NULL;

    tree->root = bst_insert_node(tree->root, data);
    if(!tree->root) return BST_ERR_ALLOC;

    tree->size++;

    return BST_OK;
}

bst_node_t* bst_remove_node(bst_node_t* root, void* data){
    if(!root) return NULL;

    if(root->data > data){
        bst_remove_node(root->lhs, data);
    }else if(root->data < data){
        bst_remove_node(root->rhs, data);
    }else{
        if(!root->lhs){
            bst_node_t* temp = root->rhs;
            free(root);
            return temp;
        }else if(!root->lhs){
            bst_node_t* temp = root->lhs;
            free(root);
            return temp;
        }else{
            bst_node_t* successor = root->rhs;
            while(successor->lhs){
                successor = successor->lhs;
            }
            root->data = successor->data;
            root->rhs = bst_remove_node(root->rhs, successor->data);
        }
    }
    return root;
}

bst_status_t bst_remove(bst_t* tree, void* data){
    if(!tree) return BST_ERR_NULL;

    tree->root = bst_remove_node(tree->root, data);
    tree->size--;

    return BST_OK;
}

bst_node_t* depth_first(bst_node_t* root){
       
}

bst_status_t depth_first(const bst_t* tree, queue_t* output){
    
}

bst_node_t* breadth_first(bst_node_t* root){

}

bst_status_t breadth_first(const bst_t* tree, queue_t* output){

}

bst_node_t* pre_order(bst_node_t* root){

}

bst_status_t pre_order(const bst_t* tree, queue_t* output){

}

bst_node_t* in_order(bst_node_t* root){

}

bst_status_t in_order(const bst_t* tree, queue_t* output){

}

bst_node_t* post_order(bst_node_t* root){

}

bst_status_t post_order(const bst_t* tree, queue_t* output){

}

bst_node_t* level_order(bst_node_t* root){

}

bst_status_t level_order(const bst_t* tree, queue_t* output){

}