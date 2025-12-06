#include <stdio.h>
#include "linkedlist.h"

typedef struct node{
    void* data;
    struct node* next;
}node_t;

node_t* newNode(void* value){
    node_t* node = malloc(sizeof(node_t));
    if(!node){
        fprintf(stderr, "Couldn't allocate memory\n");
        return NULL;
    }

    node->next = NULL;
    node->data = value;

    printf("node memory allocated at %p\n", (void*)node);
    return node;
}

static void freeNode(node_t* node){
    if(!node) return;

    if(node->data){
        free(node->data);
        node->data = NULL;
    }
    node->next = NULL;

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

    printf("list memory allocated at %p\n", (void*)list);
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
    
    if(list->tail == NULL){
        nodeToPush->next = NULL;
        list->head = list->tail = nodeToPush;
        list->size++;
        return LIST_STATUS_OK;
    }

    nodeToPush->next = list->head;
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
        nodeToPush->next = NULL;
        list->head = list->tail = nodeToPush;
        list->size++;
        return LIST_STATUS_OK;
    }
    
    list->tail->next = nodeToPush;
    list->tail = nodeToPush;
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

    node_t* nodeToInsert = malloc(sizeof(node_t));
    if(!nodeToInsert) return LIST_STATUS_NO_MEMORY;

    nodeToInsert->data = value;

    node_t* curr = list->head;
    for(size_t i = 0; i < location - 1; i++, curr = curr->next);
    
    node_t* next = curr->next;

    curr->next = nodeToInsert;
    nodeToInsert->next = next;
    list->size++;

    return LIST_STATUS_OK;
}

list_status_t listPopFront(list_t* list){
    if(!list) return LIST_STATUS_INVALID;
    if(!list->head) return LIST_STATUS_EMPTY;

    node_t* node = list->head;
    list->head = node->next;

    list->size--;

    free(node);
    return LIST_STATUS_OK;
}

list_status_t listPopBack(list_t* list){
    if(!list) LIST_STATUS_INVALID;
    if(!list->head) return LIST_STATUS_EMPTY;

    if(list->head == list->tail){
        node_t* node = list->head;
        list->head = list->head = NULL;
        list->size = 0;

        free(node);
        return LIST_STATUS_OK;
    }

    node_t* prev = NULL;
    node_t* curr = list->head;
    while(curr->next){
        prev = curr;
        curr = curr->next;
    }

    list->tail = prev;
    list->tail->next = NULL;
    list->size--;

    free(curr);

    return LIST_STATUS_OK;
}

list_status_t listRemoveAt(list_t* list, size_t location){
    if(!list) return LIST_STATUS_INVALID;
    if(!list->head) return LIST_STATUS_EMPTY;

    if(location >= list->size)
        return LIST_STATUS_INVALID;
    if(location == 0)
        return listPopFront(list);
    if(location == list->size - 1)
        return listPopBack(list);
    
    node_t* prev = list->head;
    for(size_t i = 0; i < location - 1; i++)
        prev = prev->next;
    node_t* curr = prev->next;

    free(curr);
    list->size--;

    return LIST_STATUS_OK;
}

list_status_t listSelect(list_t* list, list_select_func func, list_t* output){
    if(!list || !output) return LIST_STATUS_INVALID;
    if(!list->head) return LIST_STATUS_EMPTY;

    node_t* walk = list->head;
    size_t i = 0;
    while(walk){
        if(func(walk->data, i, list->size)){
            listPushBack(output, walk->data);
        }
        walk = walk->next;
        i++;
    }

    return LIST_STATUS_OK;
}

list_status_t listRemoveIf(list_t* list, list_predicate_func func, void* user_data){
    if(!list || !func || !user_data) return LIST_STATUS_INVALID;
    if(!list->head) return LIST_STATUS_EMPTY; 

    node_t* prev = NULL;
    node_t* curr = list->head;
    while(curr){
        if(func(curr->data, user_data)){
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

list_status_t listFindIf(list_t* list, list_predicate_func pred_func, list_select_func select_func, void* user_data, list_t* output){
    if(!list || !pred_func || !select_func || !user_data || !output) return LIST_STATUS_INVALID;
    if(!list->head) return LIST_STATUS_EMPTY;

    node_t* walk = list->head;
    while(walk){
        if(pred_func(walk->data, user_data)){
            listSelect(list, select_func, output);
        }
        walk = walk->next;
    }
}

list_status_t listClone(const list_t* list, list_t** output){
    if(!list || !output) return LIST_STATUS_INVALID;

    if(!list->head) {
        *output = initList();
        if(!*output) return LIST_STATUS_NO_MEMORY;
    }

    list_t* clonedList = initList();
    if(!clonedList) return LIST_STATUS_NO_MEMORY;

    node_t* walk = list->head;
    while(walk){
        listPushBack(clonedList, walk->data);
        walk = walk->next;
    }
    *output = clonedList;
    return LIST_STATUS_OK;
}

size_t listSize(const list_t* list){
    if(!list) return 0;
    return list->size;
}

int listIsEmpty(const list_t* list){
    if(!list) return 1;
    return (!list->head);
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
    if(!list->head) return LIST_STATUS_EMPTY;
    
    node_t* walk = list->head;
    while(walk){
        func(walk->data, user);
        walk = walk->next;
    }
    return LIST_STATUS_OK;
}

list_status_t listReverse(list_t* list){
    if(!list) return LIST_STATUS_INVALID;
    if(!list->head) return LIST_STATUS_EMPTY; 

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
    return LIST_STATUS_OK;
}

list_status_t listDisplay(list_t* list, value_type type){
    if(!list) return LIST_STATUS_INVALID;
    if(!list->head) return LIST_STATUS_EMPTY;

    node_t* walk = list->head;
    printf("\n");
    while(walk){
        switch(type){
            case INT:
                printf("%d", *(int*)walk);
                break;
            case FLOAT:
                printf("%f", *(float*)walk);
                break;
            case CHAR:
                printf("%c", *(char*)walk);
                break;
            case STRING:
                printf("%s", (char*)walk);
                break;
            case BOOL:
                printf("%s", *(bool*)walk ? "true" : "false");
                break;
            default:
                printf("invalid type");
                return LIST_STATUS_INVALID;
        }
        
        printf(" -> ");
        walk = walk->next;
    }
    printf("NULL\n");
}

list_status_t listCompare(list_t* list1, list_t* list2){
    if(!list1 || !list2) return LIST_STATUS_INVALID;
    
    node_t* node1 = list1->head;
    node_t* node2 = list2->head;

    while(node1 && node2){
        if(node1->data != node2->data) return LIST_STATUS_NOT_IDENTICAL;
        node1 = node1->next;
        node2 = node2->next;
    }

    if(node1 == NULL && node2 == NULL) return LIST_STATUS_IDENTICAL;
    else return LIST_STATUS_NOT_IDENTICAL;
}

list_status_t listMoveTo(list_t* list, node_t* node, size_t location){
    if(!list || !node || !location) return LIST_STATUS_INVALID;

    node_t* prev = NULL;
    node_t* curr = list->head;

    size_t index = 0;

    while(curr && curr != node){
        prev = curr;
        curr = curr->next;
        index++;
    }
    if(!curr) return LIST_STATUS_NOT_FOUND;

    if(prev == NULL)
        list->head = node->next;
    else
        prev->next = node->next;

    if(index < location)
        location--;
    if(location == 0){
        node->next = list->head;
        list->head = node;
        return LIST_STATUS_OK;
    }
    
    node_t* walk = list->head;
    for(size_t i = 0; i < location - 1; i++){
        walk = walk->next;
    }

    node->next = walk->next;
    walk->next = node;

    return LIST_STATUS_OK;
}

list_status_t merge(list_t* list_1, list_t* list_2){
    if(!list_1 || !list_2) return LIST_STATUS_INVALID;

    if(list_2->head == NULL) return LIST_STATUS_OK;

    if(list_1->head == NULL){
        list_1->head = list_2->head;
        list_1->tail = list_2->tail; 
    }else{
        list_1->tail->next = list_2->head;
        list_1->tail = list_2->tail;
    }

    list_1->size += list_2->size;
    
    list_2->head = NULL;
    list_2->tail = NULL;
    list_2->size = 0;

    return LIST_STATUS_OK;
}

list_status_t merge_alternate(list_t* list_1, list_t* list_2){
    if(!list_1 || !list_2) return LIST_STATUS_INVALID;

    if(list_1->head == NULL) return merge(list_1, list_2);
    if(list_2->head == NULL) return LIST_STATUS_OK;

    node_t* walk_1 = list_1->head;
    node_t* walk_2 = list_2->head;
    while(walk_1 && walk_2){
        walk_1->next = walk_2;
        if(!walk_1) break;
        walk_2->next = walk_1->next;
         
        walk_1 = walk_1->next;
        walk_2 = walk_2->next;
    }

    list_1->size += list_2->size;
    
    list_2->head = NULL;
    list_2->tail = NULL;
    list_2->size = 0;

    return LIST_STATUS_OK;
}

list_status_t listMerge(list_t* list_1, list_t* list_2, list_merge_func func){
    if(!list_1 || !list_2) return LIST_STATUS_EMPTY;
    if(!func) return LIST_STATUS_INVALID;

    func(list_1, list_2);
    return LIST_STATUS_OK;
}