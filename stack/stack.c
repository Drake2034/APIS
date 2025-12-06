#include <stdio.h>
#include <stdbool.h>
#include "stack.h"

stack_t* stack_create(void){
    stack_t* stack = malloc(sizeof(stack_t));
    if(!stack) return NULL;

    stack->stack_ptr = NULL;
    stack->size = 0;

    return stack;
}

stack_status_t stack_destroy(stack_t* stack){
    if(!stack) return STACK_ERROR_NULL;

    node_t* walk = stack->stack_ptr;
    while(walk){
        node_t* next = walk->next;
        freeNode(walk);
        walk = next;
    }

    stack->size = 0;
    stack->stack_ptr = NULL;

    free(stack);

    return STACK_OK;
}

stack_status_t stack_push(stack_t* stack, int data){
    if(!stack) return STACK_ERROR_NULL;

    node_t* nodeToPush = malloc(sizeof(node_t));
    if(!nodeToPush) return STACK_ERROR_ALLOC;

    nodeToPush->data = data;
    nodeToPush->next = stack->stack_ptr;

    stack->stack_ptr = nodeToPush;
    stack->size++;

    return STACK_OK;
}

stack_status_t stack_pop(stack_t* stack, int* output){
    if(!stack) return STACK_ERROR_NULL;
    if(!stack->stack_ptr) return STACK_ERROR_EMPTY;

    node_t* toDelete = stack->stack_ptr;
    *output = toDelete->data;

    stack->stack_ptr = toDelete->next;
    stack->size--;

    if(toDelete->data) toDelete->data = NULL;
    free(toDelete);

    return STACK_OK;
}

stack_status_t stack_peek(const stack_t* stack, int* output){
    if(!stack) return STACK_ERROR_NULL;
    if(stack->stack_ptr == NULL) return STACK_ERROR_EMPTY;

    *output = stack->stack_ptr->data;

    return STACK_OK;
}

bool stack_is_empty(const stack_t* stack){
    return !stack || stack->size == 0;
}

size_t stack_size(const stack_t* stack){
    return stack ? stack->size : 0;
}

stack_status_t stack_print(const stack_t* stack){
    if(!stack) return STACK_ERROR_NULL;

    node_t* walk = stack->stack_ptr;
    int i = 0;

    printf("stack [top -> bottom]\n");
    while(walk){
        printf("%d: %d ", i, walk->data);
        i++;
        walk = walk->next;
    }
    printf("NULL\n");

    return STACK_OK;
}

stack_status_t stack_clear(stack_t* stack){
    if(!stack) return STACK_ERROR_NULL;
    if(stack->stack_ptr == NULL) return STACK_OK;
    
    node_t* walk = stack->stack_ptr;
    while(walk){
        node_t* next = walk->next;
        if(walk->data) walk->data = NULL;
        free(walk);
        walk = next;
    }

    stack->size = 0;
    stack->stack_ptr = NULL;

    return STACK_OK;
}

stack_status_t stack_reverse(stack_t* stack){
    if(!stack) return STACK_ERROR_NULL;
    if(!stack->stack_ptr) return STACK_ERROR_EMPTY;

    node_t* prev = NULL;
    node_t* curr = stack->stack_ptr;
    node_t* next = NULL;
    while(curr){
        next = curr->next;
        curr->next = prev;

        prev = curr;
        curr = next;
    }
    stack->stack_ptr = prev;

    return STACK_OK;
}