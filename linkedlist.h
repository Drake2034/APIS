#ifndef LINKEDLIST_H
    #define LINKEDLIST_H
    #include <stddef.h>
    #include <stdbool.h>

    typedef enum{
        LIST_STATUS_OK = 0,
        LIST_STATUS_NO_MEMORY,
        LIST_STATUS_INVALID,
        LIST_STATUS_EMPTY,
        LIST_STATUS_NOT_FOUND
    }list_status_t;
    
    typedef struct singlylist list_t;

    //callback types
    typedef void (*list_free_func)(void* data);
    typedef void (*list_iterate_func)(void* data, void* user);
    typedef bool (*list_predicate_func)(void* element, void* user_data);


    list_t* initList(void);
    void freeNode(node_t* node, list_free_func free_func);
    list_status_t freeList(list_t *list, list_free_func free_func);

    list_status_t listPushFront(list_t* list, void* value);
    list_status_t listPushBack(list_t* list, void* value);
    list_status_t listInsertAt(list_t* list, size_t location, void* value);

    list_status_t listPopFront(list_t* list, void** output);
    list_status_t listPopBack(list_t* list, void** output);

    bool match(void* element, void* data){
        return *(int*)element == *(int*)data;
    }

    list_status_t listRemoveIf(list_t* list, list_predicate_func pred_func, void* user_data, void** output);

    void listRemoveFirst(list_t* list, void* value, void** output){
        listRemoveIf(list, match, &value, output);
    }

    void listRemoveAll(list_t* list, void* value, void** output){
        while(listRemoveIf(list, match, &value, output));
    }

    //to work on
    void listRemoveLast(list_t* list, int value, void** output){
        listRemoveIf(list, match, &value, output);
    }
    list_status_t listRemoveAt(list_t* list, size_t location, void** output);
    
    list_status_t listFindIf(list_t* list, list_predicate_func pred_func, void* user_data);

    size_t listSize(const list_t* list);
    int listIsEmpty(const list_t* list);
    int listIsCircular(const list_t* list);

    list_status_t listReverse(list_t* list);

    list_status_t listForEach(const list_t* list, list_iterate_func func, void* user);


#endif