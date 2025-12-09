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
    if(!stack) return STACK_ERR_NULL;
    if(!stack->stack_ptr) return STACK_EMPTY;

    node_t* walk = stack->stack_ptr;
    while(walk){
        node_t* next = walk->next;
        
        printf("element memory freed at %p\n", (void*)walk);
        freeNode(walk);

        walk = next;
    }

    stack->size = 0;
    stack->stack_ptr = NULL;

    printf("stack memory freed at %p\n", (void*)stack);
    free(stack);

    return STACK_OK;
}

stack_status_t stack_push(stack_t* stack, int data){
    if(!stack) return STACK_ERR_NULL;

    node_t* nodeToPush = malloc(sizeof(node_t));
    if(!nodeToPush) return STACK_ERR_ALLOC;

    nodeToPush->data = data;
    nodeToPush->next = stack->stack_ptr;

    stack->stack_ptr = nodeToPush;
    stack->size++;

    return STACK_OK;
}

stack_status_t stack_pop(stack_t* stack, int* output){
    if(!stack) return STACK_ERR_NULL;
    if(!stack->stack_ptr) return STACK_EMPTY;

    node_t* toDelete = stack->stack_ptr;
    *output = toDelete->data;

    stack->stack_ptr = toDelete->next;
    stack->size--;

    if(toDelete->data) toDelete->data = NULL;
    free(toDelete);

    return STACK_OK;
}

stack_status_t stack_peek(const stack_t* stack, int* output){
    if(!stack) return STACK_ERR_NULL;
    if(!stack->stack_ptr) return STACK_EMPTY;

    *output = stack->stack_ptr->data;

    return STACK_OK;
}

bool stack_is_empty(const stack_t* stack){
    return (!stack) || (stack->size == 0);
}

size_t stack_size(const stack_t* stack){
    return stack ? stack->size : 0;
}

stack_status_t stack_print(const stack_t* stack){
    if(!stack) return STACK_ERR_NULL;

    printf("stack [top -> bottom]\n");

    node_t* walk = stack->stack_ptr;
    int i = 0;
    while(walk){
        printf("%d: %d \n", i, walk->data);
        
        i++;
        walk = walk->next;
    }
    printf("NULL\n");

    return STACK_OK;
}

stack_status_t stack_clear(stack_t* stack){
    if(!stack) return STACK_ERR_NULL;
    if(!stack->stack_ptr) return STACK_OK;
    
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
    if(!stack) return STACK_ERR_NULL;
    if(!stack->stack_ptr) return STACK_EMPTY;

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

stack_status_t stack_clone(stack_t* stack, stack_t** output){
    if(!stack || !output) return STACK_ERR_NULL;

    if(!stack->stack_ptr){
        *output = stack_create();
        if(!*output)
            return STACK_ERR_ALLOC;
    }

    stack_t* clonedStack = stack_create();
    if(!clonedStack) return STACK_ERR_ALLOC;

    node_t* walk = stack->stack_ptr;
    while(walk){
        stack_status_t status = stack_push(clonedStack, walk->data);
        if(status != STACK_OK){
            stack_destroy(clonedStack);
            return status;
        }
        walk = walk->next;
    }

    stack_reverse(clonedStack);
    *output = clonedStack;

    return STACK_OK;
}

bool stack_compare(stack_t* stack_1, stack_t* stack_2){
    if(!stack_1 || !stack_2) return false;

    node_t* node1 = stack_1->stack_ptr;
    node_t* node2 = stack_2->stack_ptr;
    while(node1 && node2){
        if(node1->data != node2->data) return false;
        node1 = node1->next;
        node2 = node2->next;
    }

    return (!node1) && (!node2); 
}