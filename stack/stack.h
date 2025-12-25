#ifndef STACK_H
    #define STACK_H
    
    #include <stddef.h>
    #include <stdio.h>
    #include <stdbool.h>

    typedef enum{
        STACK_OK = 0,
        STACK_ERR_ALLOC,
        STACK_ERR_NULL,
        STACK_EMPTY
    }stack_status_t;

    typedef struct node{
        void** data;
        struct node* next;
    }stk_node_t;

    typedef struct stack{
        stk_node_t* top;
        size_t size;
    }stack_t;

    stack_t* stack_create(void);
    stack_status_t stack_destroy(stack_t* stack);

    stack_status_t stack_push(stack_t* stack, void* data);
    stack_status_t stack_pop(stack_t* stack, void** output);

    stack_status_t stack_peek(const stack_t* stack, void** output);

    bool stack_is_empty(const stack_t* stack);
    bool stack_search(const stack_t* stack, void* data);

    size_t stack_size(const stack_t* stack);
    stack_status_t stack_print(const stack_t* stack);

    stack_status_t stack_clear(stack_t* stack);
    stack_status_t stack_reverse(stack_t* stack);

    stack_status_t stack_clone(const stack_t* src, stack_t** output);
    stack_status_t stack_copy(const stack_t* src, stack_t** output);

    bool stack_compare(stack_t* stack_1, stack_t* stack_2);

#endif