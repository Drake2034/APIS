#ifndef LINKEDLIST_H
    #define LINKEDLIST_H
    #include <stddef.h>
    #include <stdbool.h>

    typedef enum{
        LIST_STATUS_OK = 0,
        LIST_STATUS_NO_MEMORY,
        LIST_STATUS_INVALID,
        LIST_STATUS_EMPTY,
        LIST_STATUS_FOUND,
        LIST_STATUS_NOT_FOUND,
        LIST_STATUS_IDENTICAL,
        LIST_STATUS_NOT_IDENTICAL
    }list_status_t;

    enum flags{
        LIST_FIRST = 1,
        LIST_MIDDLE = 2,
        LIST_LAST = 3,
        LIST_FIRST_HALF = 4,
        LIST_SECOND_HALF = 5,
        LIST_ALL = 6
    };
    
    typedef struct node{
        void* data;
        struct node* next;
    }node_t;

    typedef struct singlylist{
        node_t* head;
        node_t* tail;
        size_t size;
    }list_t;

    //callback types
    typedef void (*list_iterate_func)(void* data, void* user);
        list_status_t listForEach(const list_t* list, list_iterate_func func, void* user);

    typedef bool (*list_predicate_func)(void* element, void* user_data);
        bool match(void* element, void* data){
            return *(int*)element == *(int*)data;
        }
        bool not_match(void* element, void* data){
            return !match(element, data);
        }

        list_status_t listRemoveIf(list_t* list, list_predicate_func pred_func, void* user_data, int flags);
        list_status_t listFindIf(list_t* list, list_predicate_func pred_func, void* user_data, int flags);

    typedef void (*list_print_func)(void* data);
        //to work on
        void printer(void* data){}

        list_status_t displayList(list_t* list, list_print_func func);

    node_t* newNode(void* data);
    void freeNode(node_t* node);

    list_t* initList(void);
    list_status_t freeList(list_t *list);

    list_status_t listPushFront(list_t* list, void* value);
    list_status_t listPushBack(list_t* list, void* value);
    list_status_t listInsertAt(list_t* list, size_t location, void* value);

    list_status_t listPopFront(list_t* list);
    list_status_t listPopBack(list_t* list);

    list_status_t listRemoveAt(list_t* list, size_t location);

    size_t listSize(const list_t* list);
    int listIsEmpty(const list_t* list);
    int listIsCircular(const list_t* list);

    list_status_t listReverse(list_t* list);
#endif