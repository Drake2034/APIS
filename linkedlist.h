#ifndef LINKEDLIST_H
    #define LINKEDLIST_H
    #include <stddef.h>

    typedef enum{
        LIST_STATUS_OK = 0,
        LIST_STATUS_NO_MEMORY,
        LIST_STATUS_INVALID,
        LIST_STATUS_EMPTY
    }list_status_t;
    
    typedef struct list list_t;

    //callback types
    typedef void (*list_free_func)(void* data);
    typedef void (*list_iterate_func)(void* data, void* user);

    list_t *initList(void);
    list_status_t freeList(list_t *list, list_free_func free_func);

    list_status_t listPushFront(list_t* list, void* value);
    list_status_t listPushBack(list_t* list, void* value);

    list_status_t listPopFront(list_t* list, void** output);
    list_status_t listPopBack(list_t* list, void** output);
    
    size_t listSize(const list_t* list);
#endif