#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

node_t* newNode(void* value){
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if(!node){
        return NULL;
    }
    node->next = NULL;
    node->data = value;
    return node;
}

static void freeNode(node_t* node){
    if(!node) return;

    printf("node memory freed at %p\n", (void*) node);
    free(node);
}

list_t* initList(void){
    list_t* list = (list_t*)malloc(sizeof(list_t));
    if(!list){
        printf("couldn't allocate memory\n");
        return NULL;
    }

    list->head = list->tail = NULL;
    list->size = 0;

    printf("list memory allocated at %p\n", (void*)list);
    return list;
}

list_status_t freeList(list_t* list){
    if(!list) return LIST_STATUS_INVALID;
    
    node_t* walk = list->head;
    while(walk){
        node_t* next = walk->next;
        
        free(walk);
        printf("element memory freed at %p\n", (void*)walk);
        
        walk = next;
    }

    list->head = list->tail = NULL;
    list->size = 0;

    printf("list freed at %p\n", (void*)list);
    return LIST_STATUS_OK;
}

list_status_t listPushFront(list_t* list, void* value){
    if(!list) return LIST_STATUS_INVALID;

    node_t* nodeToPush = (node_t*)malloc(sizeof(node_t));
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

    node_t* nodeToPush = (node_t*)malloc(sizeof(node_t));
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

list_status_t listInsertAt(list_t* list, size_t location, void* value){
    if(!list) return LIST_STATUS_INVALID;

    if(location > list->size)
        return LIST_STATUS_INVALID;
    if(location == 0)
        return listPushFront(list, value);
    if(location == list->size)
        return listPushBack(list, value);

 
    node_t* nodeToInsert = (node_t*)malloc(sizeof(node_t));
    if(!nodeToInsert) return LIST_STATUS_NO_MEMORY;
    nodeToInsert->data = value;

    node_t* curr = list->head;
    for(size_t i = 0; i < location - 1; ++i){
        curr = curr->next;
    }
    node_t* next = curr->next;

    curr->next = nodeToInsert;
    nodeToInsert->next = next;
    list->size++;

    return LIST_STATUS_OK;
}

list_status_t listPopFront(list_t* list){
    if(!list) return LIST_STATUS_INVALID;
    if(list->head == NULL) return LIST_STATUS_EMPTY;

    node_t* node = list->head;
    list->head = node->next;
    if(list->head == NULL) list->tail = NULL;

    list->size--;

    free(node);
    return LIST_STATUS_OK;
}

list_status_t listPopBack(list_t* list){
    if(!list) LIST_STATUS_INVALID;
    if(list->head == NULL) return LIST_STATUS_EMPTY;

    if(list->head == list->tail){
        node_t* node = list->head;
        list->head = list->head = NULL;
        list->size = 0;

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

    free(walk);
    return LIST_STATUS_OK;
}

list_status_t listRemoveAt(list_t* list, size_t location){
    if(!list) return LIST_STATUS_INVALID;
    if(list->head == NULL) return LIST_STATUS_EMPTY;

    if(location >= list->size)
        return LIST_STATUS_INVALID;
    if(location == 0)
        return listPopFront(list);
    if(location == list->size - 1)
        return listPopBack(list);
    
    node_t* prev = list->head;

    for(size_t i = 0; i < location - 1; ++i){
        prev = prev->next;
    }

    node_t*curr = prev->next;

    free(curr);
    list->size--;
    return LIST_STATUS_OK;
}

list_status_t listRemoveIf(list_t* list, list_predicate_func pred_func, void* user_data, int flags){
    if(!list || !pred_func) return LIST_STATUS_INVALID;
    if(list->head == NULL) return LIST_STATUS_EMPTY; 

    node_t* prev = NULL;
    node_t* curr = list->head;
    while(curr){
        if(pred_func(curr->data, user_data)){
            if(prev)
                prev->next = curr->next;
            else
                list->head = curr->next;

            free(curr);
            list->size--;

            return LIST_STATUS_OK;
        }
        prev = curr;
        curr = curr->next;
    }

    return LIST_STATUS_NOT_FOUND;
}

size_t listSize(const list_t* list){
    if(!list) return 0;
    return list->size;
}

int listIsEmpty(const list_t* list){
    if(!list) return 1;
    return (list->head == NULL);
}

int listIsCircular(const list_t* list){
    if(!list || !list->head) return 0;

    node_t* slowPtr = list->head;
    node_t* fastPtr = list->head->next;

    while(fastPtr && fastPtr->next){
        if(slowPtr == fastPtr) return 1;
        slowPtr = slowPtr->next;
        fastPtr = fastPtr->next->next;
    }
    return 0;
}

list_status_t listForEach(const list_t* list, list_iterate_func func, void* user){
    if(!list || !func) return LIST_STATUS_INVALID;
    if(listIsEmpty(list)) return LIST_STATUS_EMPTY;
    
    node_t* walk = list->head;
    while(walk){
        func(walk->data, user);
        walk = walk->next;
    }
    return LIST_STATUS_OK;
}

list_status_t listReverse(list_t* list){
    if(!list) return LIST_STATUS_INVALID;
    if(list->head == NULL) return LIST_STATUS_EMPTY; 

    node_t* prev = NULL;
    node_t* curr = list->head;
    node_t* next = NULL;

    for(size_t i = 0; i < list->size; i++){
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    list->tail = list->head;
    list->head = prev;
    return LIST_STATUS_OK;
}

list_status_t displayList(list_t* list, list_print_func display){
    if(!list) return LIST_STATUS_INVALID;
    if(!list->head) return LIST_STATUS_EMPTY;

    node_t* walk = list->head;
    printf("\n");
    while(walk){
        display(walk->data);
        printf(" -> ");
        walk = walk->next;
    }
    printf("NULL\n");
}