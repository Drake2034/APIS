#ifndef LINKEDLIST_H
    #define LINKEDLIST_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdbool.h>

    typedef enum{
        LIST_OK = 0,
        LIST_ERR_ALLOC,
        LIST_ERR_NULL,
        LIST_EMPTY
    }list_status_t;
    
    typedef struct dll_node{
        void* data;
        struct dll_node* next;
        struct dll_node* prev;
    }dll_node_t;

    typedef struct dll{
        dll_node_t* head;
        dll_node_t* tail;
        size_t size;
    }dll_t;

    //callback types
    typedef list_status_t (*list_iterate_func)(void* data, void* user);
        list_status_t listForEach(const dll_t* list, list_iterate_func func, void* user);

    typedef bool (*list_predicate_func)(void* element, void* user_data);
        bool match(void* element, void* data){return *(int*)element == *(int*)data;}
        bool not_match(void* element, void* data){return !match(element, data);}

    typedef bool (*list_select_func)(void*data, size_t index, size_t size);
        bool select_first(void* data, size_t index, size_t size){return index == 0;}
        bool select_last(void* data, size_t index, size_t size){return index == size - 1;}
        bool select_mid(void* data, size_t index, size_t size){return index == size / 2;}

        bool select_first_half(void* data, size_t index, size_t size){return index < size / 2;}
        bool select_second_half(void* data, size_t index, size_t size){return index >= size/2;}

        bool select_all(void* data, size_t index, size_t size){return true;}

        //rewrok
        list_status_t listRemoveIf(dll_t* list, list_predicate_func pred_func, list_select_func select_func, void* user_data, dll_t* output);
        list_status_t listFindIf(dll_t* list, list_predicate_func pred_func, list_select_func select_func, void* user_data, dll_t* output);

    typedef enum{
        INT,
        FLOAT,
        CHAR,
        STRING,
        BOOL
    }value_type;

    list_status_t listDisplay(dll_t* list, value_type type);

    void freeNode(dll_node_t* node);

    dll_t* initList(void);
    list_status_t freeList(dll_t* list);

    list_status_t listPushFront(dll_t* list, void* value);
    list_status_t listPushBack(dll_t* list, void* value);
    list_status_t listInsertAt(dll_t* list, size_t location, void* value);

    list_status_t listPopFront(dll_t* list);
    list_status_t listPopBack(dll_t* list);
    list_status_t listRemoveAt(dll_t* list, size_t location);

    list_status_t listClone(const dll_t* list, dll_t** output);

    size_t listSize(const dll_t* list);
    bool listIsEmpty(const dll_t* list);
    bool listIsCircular(const dll_t* list);
    bool listIsSorted(const dll_t* list);

    list_status_t listReverse(dll_t* list);
    
    typedef list_status_t (*list_merge_func)(dll_t* list_1, dll_t* list_2);
        list_status_t merge(dll_t* list_1, dll_t* list_2);
        list_status_t merge_alternate(dll_t* list_1, dll_t* list_2);
    list_status_t listMerge(dll_t* list_1, dll_t* list_2, list_merge_func func);

#endif