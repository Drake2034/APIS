#ifndef SORTING_H
    #define SORTING_H

    #include "linkedlist.h"
    #include "stack.h"
    #include "queue.h"

    typedef enum{
        SORT_OK,
        SORT_ERR_NULL,
        SORT_EMPTY
    }sort_status_t;

    //to work on
    typedef void (*list_sort_func)(void** data_structure);
        sort_status_t insertion_sort();
        sort_status_t selection_sort();
        sort_status_t bubble_sort();

#endif