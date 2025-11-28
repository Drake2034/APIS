#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

typedef struct node{
    void* data;
    struct node* next;
}node_t;

struct list{
    node_t* head;
    node_t* tail;
    int size;
};

list_t* initList(void){
    list_t* list = malloc(sizeof(list_t));
    if(!list){
        printf("couldn't allocate memory\n");
        return NULL;
    }

    list->head = list->tail = NULL;
    list->size = 0;

    printf("list memory allocated at %p\n", (void*)list);
    return list;
}

static void free_node(node_t* node, list_free_func free_func){
    if(!node) return;
    if(free_func && node->data){
        free_func(node->data);
        printf("node memory freed at %p\n", (void*) node);
    }
        
    free(node);
}

list_status_t freeList(list_t* list, list_free_func free_func){
    if(!list) return LIST_STATUS_INVALID;
    
    node_t* walk = list->head;
    while(walk){
        node_t* next = walk->next;
        if (free_func && walk->data) freefunc(walk->data);
        free(walk);
        walk = next;
    }
    list->head = list->tail = NULL;
    list->size = 0;
    printf("list freed at %p\n", (void*)list);
    return LIST_STATUS_OK;
}

list_status_t listPushFront(list_t* list, void* value){
    if(!list) return LIST_STATUS_INVALID;

    node_t* nodeToPush = newNode(sizeof(node_t));
    if(!nodeToPush) return LIST_STATUS_NO_MEMORY;

    nodeToPush->data = value;
    nodeToPush->next = list->head;

    list->head = nodeToPush;
    
    if(list->tail == NULL){
        list->tail = nodeToPush;
    }
    list->size++;
    return LIST_STATUS_OK;
}

list_status_t listPushBack(list_t* list, void* value){
    if(!list) return LIST_STATUS_INVALID;

    node_t* nodeToPush = malloc(sizeof(node_t));
    if(!nodeToPush) return LIST_STATUS_NO_MEMORY;
    nodeToPush->data = value;
    nodeToPush->next = NULL;

    if(list->tail == NULL){
        list->head = list->tail = nodeToPush;
    }else{
        list->tail->next = nodeToPush;
        list->tail = nodeToPush;
    }
    list->size++;
    return LIST_STATUS_OK;
}

list_status_t listPopFront(list_t* list, void** output){
    if(!list) return LIST_STATUS_INVALID;
    if(list->head == NULL) return LIST_STATUS_EMPTY;

    node_t* node = list->head;
    list->head = node->next;
    if(list->head == NULL) list->tail = NULL;

    list->size--;
    if(output)
        *output = node->data;
    free(node);
    return LIST_STATUS_OK;
}

list_status_t listPopBack(list_t* list, void** output){
    if(!list) LIST_STATUS_INVALID;
    if(list->head == NULL) return LIST_STATUS_EMPTY;

    if(list->head == list->tail){
        node_t* node = list->head;
        list->head = list->head = NULL;
        list->size = 0;
        
        if(output)
            *output = node->data;

        free(node);
        return LIST_STATUS_OK;
    }

    node_t* walk = list->head;
    node_t* prev = NULL;
    while(walk->next){
        prev = walk;
        walk = walk->next;
    }

    list->tail = prev;
    list->tail->next = NULL;
    list->size--;

    if(output)
        *output = walk->data;

    free(walk);
    return LIST_STATUS_OK;
}

size_t listSize(const list_t* list){
    if(!list) return 0;
    return list->size;
}

int listIsEmpty(const list_t* list){
    if(!list) return 1;
    return (list->head == NULL);
}

