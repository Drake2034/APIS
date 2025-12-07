#ifndef LINKEDLIST_H
    #define LINKEDLIST_H
    #include <stddef.h>
    #include <stdbool.h>

    typedef enum{
        LIST_STATUS_OK = 0,
        LIST_STATUS_ERR,
        LIST_STATUS_NO_MEMORY,
        LIST_STATUS_INVALID,
        LIST_STATUS_EMPTY,
        LIST_STATUS_FOUND,
        LIST_STATUS_NOT_FOUND,
        LIST_STATUS_IDENTICAL,
        LIST_STATUS_NOT_IDENTICAL
    }list_status_t;

    //to fix
    typedef struct list{
        node_t* head;
        node_t* tail;
        size_t size;
    }list_t;

    //callback types
    typedef void (*list_iterate_func)(void* data, void* user);
        list_status_t listForEach(const list_t* list, list_iterate_func func, void* user);

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
        list_status_t listRemoveIf(list_t* list, list_predicate_func pred_func, list_select_func select_func, void* user_data, list_t* output);
        list_status_t listFindIf(list_t* list, list_predicate_func pred_func, list_select_func select_func, void* user_data, list_t* output);

    typedef enum{
        INT,
        FLOAT,
        CHAR,
        STRING,
        BOOL
    }value_type;

    list_status_t listDisplay(list_t* list, value_type type);

    node_t* newNode(void* data);
    void freeNode(node_t* node);

    list_t* initList(void);
    list_status_t freeList(list_t* list);

    list_status_t listPushFront(list_t* list, void* value);
    list_status_t listPushBack(list_t* list, void* value);
    list_status_t listInsertAt(list_t* list, size_t location, void* value);

    list_status_t listPopFront(list_t* list);
    list_status_t listPopBack(list_t* list);
    list_status_t listRemoveAt(list_t* list, size_t location);

    list_status_t listClone(const list_t* list, list_t** output);

    size_t listSize(const list_t* list);
    int listIsEmpty(const list_t* list);
    int listIsCircular(const list_t* list);

    list_status_t listReverse(list_t* list);
    
    typedef void (*list_merge_func)(list_t* list_1, list_t* list_2);
        list_status_t merge(list_t* list_1, list_t* list_2);
        list_status_t merge_alternate(list_t* list_1, list_t* list_2);
    list_status_t listMerge(list_t* list_1, list_t* list_2, list_merge_func func);

    list_status_t listIsSorted();
    typedef void (*list_sort_func)(list_t* list);
        list_status_t listSort_insertion();
        list_status_t listSort_bubble();
        list_status_t listSort_selection();
#endif