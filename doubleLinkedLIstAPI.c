#include <stdio.h>
#include "linkedlist.h"

typedef struct node{
    void* data;
    struct node* next;
    struct node* prev;
}node_t;

node_t* newNode(void* data){
    node_t* node = malloc(sizeof(node_t));
    if(!node){
        fprintf(stderr, "Couldn't allocate memory\n");
        return NULL;
    }
 
    node->next = node->prev = NULL;
    node->data = data;

    printf("node memory allocated at %p\n", (void*)node);
    return node;
}

static void freeNode(node_t* node){
    if(!node) return;

    if(node->data){
        free(node->data);
        node->data = NULL;
    }
    node->next = node->prev = NULL;

    printf("node memory freed at %p\n", (void*)node);
    free(node);
}

list_t* initList(){
    list_t* list = malloc(sizeof(list_t));
    if(!list){
        fprintf(stderr, "couldn't allocate memory\n");
        return NULL;
    }

    list->head = list->tail = NULL;
    list->size = 0;

    printf("list memory allocated at %p", (void*)list);
    return list;
}

list_status_t freeList(list_t* list){
    if(!list) return LIST_STATUS_INVALID;

    node_t* walk = list->head;
    while(walk){
        node_t* next = walk->next;

        printf("element memory freed at %p\n", (void*)walk);
        freeNode(walk);

        walk = next;
    }

    list->head = list->tail = NULL;
    list->size = 0;

    printf("list freed at %p\n", (void*)list);
    free(list);

    return LIST_STATUS_OK;
}