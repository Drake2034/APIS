#ifndef SLL_H
    #define SLL_H
    
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stddef.h>

    typedef enum{
        SLL_OK = 0,
        SLL_ERR_ALLOC,
        SLL_ERR_NULL,
        SLL_EMPTY
    }sll_status_t;

    typedef struct sll_node{
        void* data;
        struct sll_node* next;
    }sll_node_t;

    typedef struct sll{
        sll_node_t* head;
        sll_node_t* tail;
        size_t size;
    }sll_t;

    typedef sll_status_t (*list_iterate_func)(void* data, void* user);
        sll_status_t listForEach(const sll_t* list, list_iterate_func func, void* user);

    typedef bool (*list_predicate_func)(const sll_node_t* node, void* ctx);
        bool match_value(const sll_node_t* node, void* ctx){
            int target = *(int*)ctx;
            return node->data == target;
        }

        bool is_even(const sll_node_t* node, void* ctx){
            (void)ctx;
            return (int)node->data % 2 == 0;
        }

    typedef bool (*list_select_func)(void*data, size_t index, size_t size);
        bool select_first(void* data, size_t index, size_t size){return index == 0;}
        bool select_last(void* data, size_t index, size_t size){return index == size - 1;}
        bool select_mid(void* data, size_t index, size_t size){return index == size / 2;}

        bool select_first_half(void* data, size_t index, size_t size){return index < size / 2;}
        bool select_second_half(void* data, size_t index, size_t size){return index >= size/2;}

        bool select_all(void* data, size_t index, size_t size){return true;}

        //rewrok
        sll_status_t listRemoveIf(sll_t* list, list_predicate_func pred_func, void* ctx);
        sll_status_t listFindIf(sll_t* list, list_predicate_func pred_func, list_select_func select_func, void* user_data, sll_t* output);

    typedef enum{
        INT,
        FLOAT,
        CHAR,
        STRING,
        BOOL
    }value_type;

    sll_status_t listDisplay(sll_t* list, value_type type);

    void freeNode(sll_node_t* node);

    sll_t* initList(void);
    sll_status_t listDelete(sll_t* list);

    sll_status_t listPushFront(sll_t* list, void* value);
    sll_status_t listPushBack(sll_t* list, void* value);
    sll_status_t listInsertAt(sll_t* list, size_t location, void* value);

    sll_status_t listPopFront(sll_t* list);
    sll_status_t listPopBack(sll_t* list);
    sll_status_t listRemoveAt(sll_t* list, size_t location);

    sll_status_t listClone(const sll_t* list, sll_t** output);

    size_t listSize(const sll_t* list);
    
    bool listIsEmpty(const sll_t* list);
    bool listIsCircular(const sll_t* list);
    bool listIsSorted(const sll_t* list);
    bool listSearch(const sll_t* list, void* data, size_t location);

    sll_status_t listReverse(sll_t* list);
    
    typedef sll_status_t (*list_merge_func)(sll_t* list_1, sll_t* list_2);
        sll_status_t merge(sll_t* list_1, sll_t* list_2);
        sll_status_t merge_alternate(sll_t* list_1, sll_t* list_2);
    sll_status_t listMerge(sll_t* list_1, sll_t* list_2, list_merge_func func);

#endif