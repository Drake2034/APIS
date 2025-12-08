#ifndef STACK_H
    #define STACK_H
    #include <stddef.h>

    typedef enum{
        STACK_OK = 0,
        STACK_ERR_ALLOC,
        STACK_ERR_NULL,
        STACK_EMPTY
    }stack_status_t;

    typedef struct node{
        int* data;
        struct node* next;
    }node_t;

    typedef struct stack{
        node_t* stack_ptr;
        size_t size;
    }stack_t;

    stack_t* stack_create(void);
    stack_status_t stack_destroy(stack_t* stack);

    stack_status_t stack_push(stack_t* stack, int data);
    stack_status_t stack_pop(stack_t* stack, int* output);

    stack_status_t stack_peek(const stack_t* stack, int* output);
    int stack_is_empty(const stack_t* stack);
    size_t stack_size(const stack_t* stack);
    stack_status_t stack_print(const stack_t* stack);

    stack_status_t stack_clear(stack_t* stack);
    stack_status_t stack_reverse(stack_t* stack);

#endif