#include "linkedlist.h"

typedef struct node{
    void* data;
    struct node* next;
    struct node* prev;
}node_t;

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

list_t* initList(void){
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
    if(!list) return LIST_ERR_NULL;

    node_t* walk = list->head;
    while(walk){
        node_t* next = walk->next;

        printf("element memory freed at %p\n", (void*)walk);
        freeNode(walk);

        walk = next;
    }

    list->head = list->tail = NULL;
    list->size = 0;

    printf("list memory freed at %p\n", (void*)list);
    free(list);

    return LIST_OK;
}

list_status_t listPushFront(list_t* list, void* value){
    if(!list) return LIST_ERR_NULL;

    node_t* nodeToPush = malloc(sizeof(node_t));
    if(!nodeToPush) return LIST_ERR_ALLOC;

    nodeToPush->data = value;
    nodeToPush->prev = NULL;

    if(list->tail == NULL){
        nodeToPush->next = NULL;

        list->head = list->tail = nodeToPush;
        list->size++;

        return LIST_OK;
    }

    nodeToPush->next = list->head;

    list->head->prev = nodeToPush;
    list->head = nodeToPush;
    list->size++;
    return LIST_OK;
}

list_status_t listPushBack(list_t* list, void* value){
    if(!list) return LIST_ERR_NULL;

    node_t* nodeToPush = malloc(sizeof(node_t));
    if(!nodeToPush) return LIST_ERR_ALLOC;

    nodeToPush->data = value;
    nodeToPush->next = NULL;

    if(list->tail == NULL){
        nodeToPush->prev = NULL;

        list->head = list->tail = nodeToPush;
        list->size++;
        
        return LIST_OK;
    }

    nodeToPush->prev = list->tail;

    list->tail->next = nodeToPush;
    list->tail = nodeToPush;
    list->size++;

    return LIST_OK;
}

list_status_t listInsertAt(list_t* list, size_t location, void* value){
    if(!list) return LIST_ERR_NULL;

    if(location > list->size)
        return LIST_ERR_NULL;
    if(location = 0)
        return listPushFront(list, value);
    if(location == list->size)
        return listPushBack(list, value);

    node_t* nodeToInsert = malloc(sizeof(node_t));
    if(!nodeToInsert) return LIST_ERR_ALLOC;

    nodeToInsert->data = value;

    node_t* curr = list->head;
    for(size_t i = 0; i < location; i++, curr = curr->next);

    node_t* next = curr->next;

    nodeToInsert->next = next;
    nodeToInsert->prev = curr;

    next->prev = nodeToInsert;
    curr->next = nodeToInsert;

    list->size++;
    return LIST_OK;
}

list_status_t listPopFront(list_t* list){
    if(!list) return LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY;

    node_t* node = list->head;

    list->head = node->next;
    list->head->prev = NULL;

    list->size--;

    free(node);
    return LIST_OK;
}

list_status_t listPopBack(list_t* list){
    if(!list) return LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY;

    if(list->head == list->tail){
        node_t* node = list->head;
        list->head = list->tail = NULL;

        list->size = 0;
        free(node);
        return LIST_OK;
    }

    node_t* curr = list->head;
    while(curr->next){
        curr = curr->next;
    }

    list->tail = curr->prev;
    list->tail->next = NULL;
    list->tail->prev = curr->prev;
    list->size--;

    curr->prev = NULL;
    free(curr);

    return LIST_OK;
}

list_status_t listRemoveAt(list_t* list, size_t location){
    if(!list) return LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY;

    if(location >= list->size)
        return LIST_ERR_NULL;
    if(location == 0)
        return listPopFront(list);
    if(location == list->size - 1)
        return listPopBack(list);

    node_t* walk = list->head;
    for(size_t i = 0; i < location; i++)
        walk = walk->next;

    walk->prev->next = walk->next;
    walk->next->prev = walk->prev;

    free(walk);
    list->size--;

    return LIST_OK;
}

list_status_t listSelect(list_t* list, list_select_func func, list_t* output){
    if(!list || !output) return LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY;

    node_t* walk = list->head;
    size_t i = 0;
    while(walk){
        if(func(walk->data, i, list->size)){
            listPushBack(output, walk->data);
        }
        walk = walk->next;
        i++;
    }

    return LIST_OK;
}

list_status_t listClone(const list_t* list, list_t** output){
    if(!list || !output) return LIST_ERR_NULL;

    if(!list->head){
        *output = initList();
        if(!*output) return LIST_ERR_ALLOC;
    }

    list_t* clonedList = initList();
    if(!clonedList) return LIST_ERR_ALLOC;

    node_t* walk = list->head;
    while(walk){
        listPushBack(clonedList, walk);
        walk = walk->next;
    }
    *output = clonedList;
    return LIST_OK;
}

size_t listSize(const list_t* list){
    return list ? list->size : 0;
}

bool listIsEmpty(const list_t* list){
    return (!list) || (!list->head);
}

bool listIsCircular(const list_t* list){
    if(!list || !list->head) return false;

    node_t* slowPtr = list->head;
    node_t* fastPtr = list->head;

    while(slowPtr && fastPtr->next){
        if(slowPtr == fastPtr) return true;

        slowPtr = slowPtr->next;
        fastPtr = fastPtr->next->next;
    }
    return false;
}

list_status_t listReverse(list_t* list){
    if(!list) return LIST_ERR_NULL;
    
    node_t* prev = NULL;
    node_t* curr = list->head;
    node_t* next = NULL;
    while(curr){
        next = curr->next;
        curr->next = prev;

        prev = curr;
        curr = next;
    }

    list->tail = list->head;
    list->head = prev;

    return LIST_OK;
}

list_status_t merge(list_t* list_1, list_t* list_2){
    if(!list_1 || !list_2) return LIST_ERR_NULL;

    if(!list_2->head) return LIST_OK;
    if(!list_1->head){
        list_1->head = list_2->head;
        list_1->tail = list_2->head;
    }else{
        list_1->tail->next = list_2->head;
        list_1->tail = list_2->tail;
    }

    list_1->size += list_2->size;

    list_2->head = list_2->tail = NULL;
    list_2->size = 0;

    return LIST_OK;
}

list_status_t merge_alternate(list_t* list_1, list_t* list_2){
    if(!list_1 || !list_2) return LIST_ERR_NULL;

    if(!list_1->head) return merge(list_1, list_2);
    if(!list_2->head) return LIST_OK;
    
    node_t* node1 = list_1->head;
    node_t* node2 = list_2->head;

    node_t* next1;
    node_t* next2;
    while(node1 && node2){
        next1 = node1->next;
        next2 = node2->next;

        node1->next = node2;
        node2->prev = node1;

        if(!next1) break;
        node2->next = next1;
        next1->prev = node2;

        node1 = next1;
        node2 = next2;
    }

    list_1->size += list_2->size;
    list_1->tail = list_2->tail;

    list_2->head = list_2->tail = NULL;
    list_2->size = 0;

    return LIST_OK;
}

list_status_t listMerge(list_t* list_1, list_t* list_2, list_merge_func func){
    if(!list_1 || !list_2 || !func) return LIST_ERR_NULL;
    return func(list_1, list_2);
}
