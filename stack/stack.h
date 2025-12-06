#ifndef STACK_H
    #define STACK_H
    #include <stddef.h>

    typedef enum{
        STACK_OK = 0,
        STACK_ERROR_NULL,
        STACK_ERROR_EMPTY,
        STACK_ERROR_ALLOC
    }stack_status_t;

    typedef struct node{
        int* data;
        struct node* next;
    }node_t;

    typedef struct stack{
        node_t* stack_ptr;
        size_t size;
    }stack_t;

#endif