#include "dll.h"

void freeNode(dll_node_t* node){
    if(!node) return;

    if(node->data){
        free(node->data);
        node->data = NULL;
    }
    node->next = node->prev = NULL;

    printf("node memory freed at %p\n", (void*)node);
    free(node);
}

dll_t* initList(void){
    dll_t* list = malloc(sizeof(*list));
    if(!list){
        fprintf(stderr, "couldn't allocate memory\n");
        return NULL;
    }

    list->head = list->tail = NULL;
    list->size = 0;

    printf("list memory allocated at %p", (void*)list);
    return list;
}

dll_status_t freeList(dll_t* list){
    if(!list) return DLL_ERR_NULL;

    dll_node_t* walk = list->head;
    while(walk){
        dll_node_t* next = walk->next;

        printf("element memory freed at %p\n", (void*)walk);
        freeNode(walk);

        walk = next;
    }

    list->head = list->tail = NULL;
    list->size = 0;

    printf("list memory freed at %p\n", (void*)list);
    free(list);

    return DLL_OK;
}

dll_status_t listPushFront(dll_t* list, void* value){
    if(!list) return DLL_ERR_NULL;

    dll_node_t* node = malloc(sizeof(*node));
    if(!node) return DLL_ERR_ALLOC;

    node->data = value;
    node->prev = NULL;

    if(list->tail == NULL){
        node->next = NULL;

        list->head = list->tail = node;
        list->size++;

        return DLL_OK;
    }

    node->next = list->head;

    list->head->prev = node;
    list->head = node;
    list->size++;

    return DLL_OK;
}

dll_status_t listPushBack(dll_t* list, void* value){
    if(!list) return DLL_ERR_NULL;

    dll_node_t* node = malloc(sizeof(*node));
    if(!node) return DLL_ERR_ALLOC;

    node->data = value;
    node->next = NULL;

    if(list->tail == NULL){
        node->prev = NULL;

        list->head = list->tail = node;
        list->size++;
        
        return DLL_OK;
    }

    node->prev = list->tail;

    list->tail->next = node;
    list->tail = node;
    list->size++;

    return DLL_OK;
}

dll_status_t listInsertAt(dll_t* list, size_t location, void* value){
    if(!list) return DLL_ERR_NULL;

    if(location > list->size)
        return DLL_ERR_NULL;
    if(location = 0)
        return listPushFront(list, value);
    if(location == list->size)
        return listPushBack(list, value);

    dll_node_t* node = malloc(sizeof(*node));
    if(!node) return DLL_ERR_ALLOC;

    node->data = value;

    dll_node_t* curr = list->head;
    for(size_t i = 0; i < location; i++, curr = curr->next);

    dll_node_t* next = curr->next;

    node->next = next;
    node->prev = curr;

    next->prev = node;
    curr->next = node;

    list->size++;
    return DLL_OK;
}

dll_status_t listPopFront(dll_t* list){
    if(!list) return DLL_ERR_NULL;
    if(!list->head) return DLL_EMPTY;

    dll_node_t* node = list->head;

    list->head = node->next;
    list->head->prev = NULL;

    list->size--;

    free(node);
    return DLL_OK;
}

dll_status_t listPopBack(dll_t* list){
    if(!list) return DLL_ERR_NULL;
    if(!list->head) return DLL_EMPTY;

    if(list->head == list->tail){
        dll_node_t* node = list->head;
        list->head = list->tail = NULL;

        list->size = 0;
        free(node);
        return DLL_OK;
    }

    dll_node_t* curr = list->head;
    while(curr->next){
        curr = curr->next;
    }

    list->tail = curr->prev;
    list->tail->next = NULL;
    list->tail->prev = curr->prev;
    list->size--;

    curr->prev = NULL;
    free(curr);

    return DLL_OK;
}

dll_status_t listRemoveAt(dll_t* list, size_t location){
    if(!list) return DLL_ERR_NULL;
    if(!list->head) return DLL_EMPTY;

    if(location >= list->size)
        return DLL_ERR_NULL;
    if(location == 0)
        return listPopFront(list);
    if(location == list->size - 1)
        return listPopBack(list);

    dll_node_t* walk = list->head;
    for(size_t i = 0; i < location; i++)
        walk = walk->next;

    walk->prev->next = walk->next;
    walk->next->prev = walk->prev;

    free(walk);
    list->size--;

    return DLL_OK;
}

dll_status_t listSelect(dll_t* list, list_select_func func, dll_t* output){
    if(!list || !output) return DLL_ERR_NULL;
    if(!list->head) return DLL_EMPTY;

    dll_node_t* walk = list->head;
    size_t i = 0;
    while(walk){
        if(func(walk->data, i, list->size)){
            listPushBack(output, walk->data);
        }
        walk = walk->next;
        i++;
    }

    return DLL_OK;
}

dll_status_t listClone(const dll_t* list, dll_t** output){
    if(!list || !output) return DLL_ERR_NULL;

    if(!list->head){
        *output = initList();
        if(!*output) return DLL_ERR_ALLOC;
    }

    dll_t* clonedList = initList();
    if(!clonedList) return DLL_ERR_ALLOC;

    dll_node_t* walk = list->head;
    while(walk){
        listPushBack(clonedList, walk);
        walk = walk->next;
    }
    *output = clonedList;

    return DLL_OK;
}

size_t listSize(const dll_t* list){
    return list ? list->size : 0;
}

bool listIsEmpty(const dll_t* list){
    return (!list) || (!list->head);
}

bool listIsCircular(const dll_t* list){
    if(!list || !list->head) return false;

    dll_node_t* slowPtr = list->head;
    dll_node_t* fastPtr = list->head;

    while(slowPtr && fastPtr->next){
        if(slowPtr == fastPtr) return true;

        slowPtr = slowPtr->next;
        fastPtr = fastPtr->next->next;
    }
    return false;
}

dll_status_t listReverse(dll_t* list){
    if(!list) return DLL_ERR_NULL;
    
    dll_node_t* prev = NULL;
    dll_node_t* curr = list->head;
    dll_node_t* next = NULL;
    while(curr){
        next = curr->next;
        curr->next = prev;

        prev = curr;
        curr = next;
    }

    list->tail = list->head;
    list->head = prev;

    return DLL_OK;
}

dll_status_t merge(dll_t* list_1, dll_t* list_2){
    if(!list_1 || !list_2) return DLL_ERR_NULL;

    if(!list_2->head) return DLL_OK;
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

    return DLL_OK;
}

dll_status_t merge_alternate(dll_t* list_1, dll_t* list_2){
    if(!list_1 || !list_2) return DLL_ERR_NULL;

    if(!list_1->head) return merge(list_1, list_2);
    if(!list_2->head) return DLL_OK;
    
    dll_node_t* node1 = list_1->head;
    dll_node_t* node2 = list_2->head;

    dll_node_t* next1;
    dll_node_t* next2;
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

    return DLL_OK;
}

dll_status_t listMerge(dll_t* list_1, dll_t* list_2, list_merge_func func){
    if(!list_1 || !list_2 || !func) return DLL_ERR_NULL;
    return func(list_1, list_2);
}

bool listIsSorted(const dll_t* list){
    if(!list || !list->head) return false;

    dll_node_t* walk = list->head;
    while(walk){
        if(walk->data >= walk->next->data) return false;
        walk = walk->next;
    }
    return true;
}

dll_status_t listDisplay(dll_t* list, value_type type){
    if(!list) return DLL_ERR_NULL;
    if(!list->head) return DLL_EMPTY;

    dll_node_t** walk = &list->head;
    printf("\nNULL");
    while(walk){
        switch(type){
            case INT:
                printf("%d", *(int*)*walk);
                break;
            case FLOAT:
                printf("%f", *(float*)*walk);
                break;
            case CHAR:
                printf("%c", *(char*)*walk);
                break;
            case STRING:
                printf("%s", (char*)*walk);
                break;
            case BOOL:
                printf("%s", *(bool*)*walk ? "true" : "false");
                break;
            default:
                printf("invalid type");
                return DLL_ERR_NULL;
        }
        
        printf(" <-> ");
        walk = &(*walk)->next;
    }
    printf("NULL\n");
}

bool listSearch(const dll_t* list, void* data, size_t location){
    if(!list) return false;

    dll_node_t** walk = &list->head;
    size_t i = 0;

    while(walk){
        if((*walk)->data == data){
            location = i;
            return true;
        }
        walk = &(*walk)->next;
        i++;
    }
    return false;
}