#ifndef SORTING_H
    #define SORTING_H

    #include "linkedlist.h"

    typedef void (*list_sort_func)(list_t* list);
        list_status_t listSort_insertion();
        list_status_t listSort_bubble();
        list_status_t listSort_selection();
#endif