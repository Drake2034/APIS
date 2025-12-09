#ifndef QUEUE_H
    #define QUEUE_H

    #include <stddef.h>
    #include <stdbool.h>
    #include <stdio.h>

    typedef enum{
        QUEUE_OK,
        QUEUE_ERR_ALLOC,
        QUEUE_ERR_NULL,
        QUEUE_EMPTY
    }queue_status_t;

    typedef struct node{
        int* data;
        struct node* next;
    }node_t;

    typedef struct queue{
        node_t* head;
        node_t* tail;
        size_t size;
    }queue_t;

    queue_t* queue_create(void);
    queue_status_t queue_destroy(queue_t* queue);

    queue_status_t queue_enqueue(queue_t* queue, int data);
    queue_status_t queue_dequeue(queue_t* queue, int* output);

    queue_status_t queue_peek_front(const queue_t* queue, int* output);
    queue_status_t queue_peek_back(const queue_t* queue, int* output);

    bool queue_is_empty(const queue_t* queue);
    size_t queue_size(const queue_t* queue);
    queue_status_t queue_print(const queue_t* queue);

    queue_status_t queue_clear(queue_t* queue);
    queue_status_t queue_reverse(queue_t* queue);

    queue_status_t queue_clone(queue_t* queue, queue_t** output);
    bool queue_compare(queue_t* queue_1, queue_t* queue_2);
#endif