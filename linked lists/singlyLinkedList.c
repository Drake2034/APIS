#include "sll.h"

static void freeNode(sll_node_t* node){
    if(!node) return;

    if(node->data){
        free(node->data);
        node->data = NULL;
    }
    node->next = NULL;

    printf("node memory freed at %p\n", (void*)node);
    free(node);
}

sll_t* initList(void){
    sll_t* list = malloc(sizeof(*list));
    if(!list){
        fprintf(stderr, "couldn't allocate memory\n");
        return NULL;
    }

    list->head = list->tail = NULL;
    list->size = 0;

    printf("list memory allocated at %p\n", (void*)list);
    return list;
}

list_status_t listDelete(sll_t* list){
    if(!list) return LIST_ERR_NULL;
    
    sll_node_t* walk = list->head;
    while(walk){
        sll_node_t* next = walk->next;
        
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

list_status_t listPushFront(sll_t* list, void* value){
    if(!list) return LIST_ERR_NULL;

    sll_node_t* node = malloc(sizeof(*node));
    if(!node) return LIST_ERR_ALLOC;

    node->data = value;
    
    if(list->tail == NULL){
        node->next = NULL;

        list->head = list->tail = node;
        list->size++;

        return LIST_OK;
    }

    node->next = list->head;

    list->head = node;
    list->size++;

    return LIST_OK;
}

list_status_t listPushBack(sll_t* list, void* value){
    if(!list) return LIST_ERR_NULL;

    sll_node_t* node = malloc(sizeof(*node));
    if(!node) return LIST_ERR_ALLOC;

    node->data = value;
    node->next = NULL;

    if(list->tail == NULL){
        node->next = NULL;
        list->head = list->tail = node;
        list->size++;
        return LIST_OK;
    }
    
    list->tail->next = node;
    list->tail = node;
    list->size++;

    return LIST_OK;
}

list_status_t listInsertAt(sll_t* list, size_t location, void* value){
    if(!list) return LIST_ERR_NULL;

    if(location > list->size)
        return LIST_ERR_NULL;
    if(location == 0)
        return listPushFront(list, value);
    if(location == list->size)
        return listPushBack(list, value);

    sll_node_t* node = malloc(sizeof(*node));
    if(!node) return LIST_ERR_ALLOC;

    node->data = value;

    sll_node_t* curr = list->head;
    for(size_t i = 0; i < location - 1; i++, curr = curr->next);
    
    sll_node_t* next = curr->next;

    curr->next = node;
    node->next = next;
    list->size++;

    return LIST_OK;
}

list_status_t listPopFront(sll_t* list){
    if(!list) return LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY;

    sll_node_t* node = list->head;
    list->head = node->next;

    list->size--;

    free(node);
    return LIST_OK;
}

list_status_t listPopBack(sll_t* list){
    if(!list) LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY;

    if(list->head == list->tail){
        sll_node_t* node = list->head;
        list->head = list->head = NULL;
        list->size = 0;

        free(node);
        return LIST_OK;
    }

    sll_node_t* prev = NULL;
    sll_node_t* curr = list->head;
    while(curr->next){
        prev = curr;
        curr = curr->next;
    }

    list->tail = prev;
    list->tail->next = NULL;
    list->size--;

    free(curr);

    return LIST_OK;
}

list_status_t listRemoveAt(sll_t* list, size_t location){
    if(!list) return LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY;

    if(location >= list->size)
        return LIST_ERR_NULL;
    if(location == 0)
        return listPopFront(list);
    if(location == list->size - 1)
        return listPopBack(list);
    
    sll_node_t* prev = list->head;
    for(size_t i = 0; i < location - 1; i++)
        prev = prev->next;
    sll_node_t* curr = prev->next;

    free(curr);
    list->size--;

    return LIST_OK;
}

list_status_t listSelect(sll_t* list, list_select_func func, sll_t* output){
    if(!list || !output) return LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY;

    sll_node_t* walk = list->head;
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

//rework

// list_status_t listRemoveIf(
//     sll_t* list,
//     list_predicate_func pred_func,
//     list_select_func select_func,
//     void* user_data,
//     sll_t** output){

//     if(!list || !pred_func || !select_func || !user_data) return LIST_ERR_NULL;
//     if(!list->head) return LIST_EMPTY; 

//     sll_node_t* prev = NULL;
//     sll_node_t* curr = list->head;
//     while(curr){
//         if(pred_func(curr->data, user_data)){
//             listSelect(list, select_func, output);
//         }

//         prev = curr;
//         curr = curr->next;
//     }
    
//     return LIST_STATUS_NOT_FOUND;
// }

list_status_t listFindIf(sll_t* list, list_predicate_func pred_func, list_select_func select_func, void* user_data, sll_t* output){
    if(!list || !pred_func || !select_func || !user_data || !output) return LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY;

    sll_node_t* walk = list->head;
    while(walk){
        if(pred_func(walk->data, user_data)){
            listSelect(list, select_func, output);
        }
        walk = walk->next;
    }
}

list_status_t listClone(const sll_t* list, sll_t** output){
    if(!list || !output) return LIST_ERR_NULL;

    if(!list->head) {
        *output = initList();
        if(!*output) return LIST_ERR_ALLOC;
    }

    sll_t* clonedList = initList();
    if(!clonedList) return LIST_ERR_ALLOC;

    sll_node_t* walk = list->head;
    while(walk){
        listPushBack(clonedList, walk->data);
        walk = walk->next;
    }
    *output = clonedList;
    return LIST_OK;
}

size_t listSize(const sll_t* list){
    return list ? list->size : 0;
}

bool listIsEmpty(const sll_t* list){
    return (!list) || (!list->head);
}

bool listIsCircular(const sll_t* list){
    if(!list || !list->head) return false;

    sll_node_t* slowPtr = list->head;
    sll_node_t* fastPtr = list->head->next;

    while(fastPtr && fastPtr->next){
        if(slowPtr == fastPtr) return true;

        slowPtr = slowPtr->next;
        fastPtr = fastPtr->next->next;
    }
    return false;
}

list_status_t listForEach(const sll_t* list, list_iterate_func func, void* user){
    if(!list || !func) return LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY;
    
    sll_node_t* walk = list->head;
    while(walk){
        func(walk->data, user);
        walk = walk->next;
    }
    return LIST_OK;
}

list_status_t listReverse(sll_t* list){
    if(!list) return LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY; 

    sll_node_t* prev = NULL;
    sll_node_t* curr = list->head;
    sll_node_t* next = NULL;

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

list_status_t listDisplay(sll_t* list, value_type type){
    if(!list) return LIST_ERR_NULL;
    if(!list->head) return LIST_EMPTY;

    sll_node_t* walk = list->head;
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
                return LIST_ERR_NULL;
        }
        
        printf(" -> ");
        walk = walk->next;
    }
    printf("NULL\n");
}

int listCompare(sll_t* list1, sll_t* list2){
    if(!list1 || !list2) return 0;
    
    sll_node_t* node1 = list1->head;
    sll_node_t* node2 = list2->head;

    while(node1 && node2){
        if(node1->data != node2->data) return 0;
        node1 = node1->next;
        node2 = node2->next;
    }

    return (node1 == NULL && node2 == NULL) ? 1 : 0;
}

list_status_t listMoveTo(sll_t* list, sll_node_t* node, size_t location){
    if(!list || !node || !location) return LIST_ERR_NULL;

    sll_node_t* prev = NULL;
    sll_node_t* curr = list->head;

    size_t index = 0;

    while(curr && curr != node){
        prev = curr;
        curr = curr->next;
        index++;
    }
    if(!curr) return LIST_ERR_NULL;

    if(prev == NULL)
        list->head = node->next;
    else
        prev->next = node->next;

    if(index < location)
        location--;
    if(location == 0){
        node->next = list->head;
        list->head = node;
        return LIST_OK;
    }
    
    sll_node_t* walk = list->head;
    for(size_t i = 0; i < location - 1; i++){
        walk = walk->next;
    }

    node->next = walk->next;
    walk->next = node;

    return LIST_OK;
}

list_status_t merge(sll_t* list_1, sll_t* list_2){
    if(!list_1 || !list_2) return LIST_ERR_NULL;

    if(!list_2->head) return LIST_OK;

    if(!list_1->head){
        list_1->head = list_2->head;
        list_1->tail = list_2->tail; 
    }else{
        list_1->tail->next = list_2->head;
        list_1->tail = list_2->tail;
    }

    list_1->size += list_2->size;
    
    list_2->head = list_2->tail = NULL;
    list_2->size = 0;

    return LIST_OK;
}

list_status_t merge_alternate(sll_t* list_1, sll_t* list_2){
    if(!list_1 || !list_2) return LIST_ERR_NULL;

    if(!list_1->head) return merge(list_1, list_2);
    if(!list_2->head) return LIST_OK;

    sll_node_t* node1 = list_1->head;
    sll_node_t* node2 = list_2->head;

    sll_node_t* next1;
    sll_node_t* next2;
    while(node1 && node2){
        next1 = node1->next;
        next2 = node2->next;

        node1->next = node2;

        if(!next1) break;
        node2->next = next1;

        node1 = next1;
        node2 = next2;
    }

    list_1->size += list_2->size;
    list_1->tail = list_2->tail;

    list_2->head = list_2->tail = NULL;
    list_2->size = 0;

    return LIST_OK;
}

list_status_t listMerge(sll_t* list_1, sll_t* list_2, list_merge_func func){
    if(!list_1 || !list_2 || !func) return LIST_ERR_NULL;
    return func(list_1, list_2);
}

bool listIsSorted(const sll_t* list){
    if(!list || !list->head) return false;

    sll_node_t* walk = list->head;
    while(walk){
        if(walk->data >= walk->next->data) return false;
        walk = walk->next;
    }
    return true;
}