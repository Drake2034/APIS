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

list_status_t listPushFront(list_t* list, void* value){
    if(!list) return LIST_STATUS_INVALID;

    node_t* nodeToPush = malloc(sizeof(node_t));
    if(!nodeToPush) return LIST_STATUS_NO_MEMORY;

    nodeToPush->data = value;
    nodeToPush->prev = NULL;

    if(list->tail == NULL){
        nodeToPush->next = NULL;

        list->head = list->tail = nodeToPush;
        list->size++;
        return LIST_STATUS_OK;
    }

    nodeToPush->next = list->head;

    list->head->prev = nodeToPush;
    list->head = nodeToPush;
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
        nodeToPush->prev = NULL;

        list->head = list->tail = nodeToPush;
        list->size++;
        return LIST_STATUS_OK;
    }

    nodeToPush->prev = list->tail;

    list->tail->next = nodeToPush;
    list->tail = nodeToPush;
    list->size++;
}

list_status_t listInsertAt(list_t* list, size_t location, void* value){
    if(!list) return LIST_STATUS_INVALID;

    if(location > list->size)
        return LIST_STATUS_INVALID;
    if(location = 0)
        return listPushFront(list, value);
    if(location == list->size)
        return listPushBack(list, value);

    node_t* nodeToInsert = malloc(sizeof(node_t));
    if(!nodeToInsert) return LIST_STATUS_NO_MEMORY;

    nodeToInsert->data = value;

    node_t* curr = list->head;
    for(size_t i = 0; i < location; i++, curr = curr->next);

    node_t* next = curr->next;

    nodeToInsert->next = next;
    nodeToInsert->prev = curr;

    next->prev = nodeToInsert;
    curr->next = nodeToInsert;

    list->size++;
    return LIST_STATUS_OK;
}