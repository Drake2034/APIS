#ifndef SORTING_H
    #define SORTING_H

    #include "sll.h"
    #include "dll.h"
    #include "stack.h"
    #include "queue.h"

    typedef enum{
        SORT_OK,
        SORT_ERR,
        SORT_EMPTY
    }sort_status_t;

    typedef enum{
        SLL,
        DLL,
        Q,
        STK,
        BT,
    }structures_t;

    //to work on
    typedef void (*list_sort_func)(int flag, void** data);
        sort_status_t insertion_sort();
        sort_status_t selection_sort();
        sort_status_t bubble_sort();

#endif