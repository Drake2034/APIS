#ifndef LINKEDLIST_H
    #define LINKEDLIST_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdbool.h>

    typedef enum{
        DLL_OK = 0,
        DLL_ERR_ALLOC,
        DLL_ERR_NULL,
        DLL_EMPTY
    }dll_status_t;
    
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
    typedef dll_status_t (*list_iterate_func)(void* data, void* user);
        dll_status_t listForEach(const dll_t* list, list_iterate_func func, void* user);

    typedef bool (*list_predicate_func)(const dll_node_t* node, void* ctx);
        bool match_value(const dll_node_t* node, void* ctx){
            int target = *(int*)ctx;
            return node->data == target;
        }

        bool is_even(const dll_node_t* node, void* ctx){
            (void)ctx;
            return (int)node->data % 2 == 0;
        }

    typedef bool (*list_select_func)(void* data, size_t index, size_t size);
        bool select_first(void* data, size_t index, size_t size){return index == 0;}
        bool select_last(void* data, size_t index, size_t size){return index == size - 1;}
        bool select_mid(void* data, size_t index, size_t size){return index == size / 2;}

        bool select_first_half(void* data, size_t index, size_t size){return index < size / 2;}
        bool select_second_half(void* data, size_t index, size_t size){return index >= size/2;}

        bool select_all(void* data, size_t index, size_t size){return true;}

        //rewrok
        dll_status_t listRemoveIf(dll_t* list, list_predicate_func pred_func, list_select_func select_func, void* user_data, dll_t* output);
        dll_status_t listFindIf(dll_t* list, list_predicate_func pred_func, list_select_func select_func, void* user_data, dll_t* output);

    typedef enum{
        INT,
        FLOAT,
        CHAR,
        STRING,
        BOOL
    }value_type;

    dll_status_t listDisplay(dll_t* list, value_type type);

    void freeNode(dll_node_t* node);

    dll_t* initList(void);
    dll_status_t freeList(dll_t* list);

    dll_status_t listPushFront(dll_t* list, void* value);
    dll_status_t listPushBack(dll_t* list, void* value);
    dll_status_t listInsertAt(dll_t* list, size_t location, void* value);

    dll_status_t listPopFront(dll_t* list);
    dll_status_t listPopBack(dll_t* list);
    dll_status_t listRemoveAt(dll_t* list, size_t location);

    dll_status_t listClone(const dll_t* list, dll_t** output);

    size_t listSize(const dll_t* list);
    bool listIsEmpty(const dll_t* list);
    bool listIsCircular(const dll_t* list);
    bool listIsSorted(const dll_t* list);
    bool listSearch(const dll_t* list, void* data, size_t location);

    dll_status_t listReverse(dll_t* list);
    
    typedef dll_status_t (*list_merge_func)(dll_t* list_1, dll_t* list_2);
        dll_status_t merge(dll_t* list_1, dll_t* list_2);
        dll_status_t merge_alternate(dll_t* list_1, dll_t* list_2);
    dll_status_t listMerge(dll_t* list_1, dll_t* list_2, list_merge_func func);

#endif