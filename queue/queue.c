#include "queue.h"

queue_t* queue_create(void){
    queue_t* queue = malloc(sizeof(queue_t));
    if(!queue){
        fprintf(stderr, "couldn't allocate memory\n");
        return NULL;
    }

    queue->head = queue->tail = NULL;
    queue->size = 0;
    
    printf("queue memory allocated at %p", (void*)queue);
    return queue;
}

queue_status_t queue_destroy(queue_t* queue){
    if(!queue) return QUEUE_ERR_NULL;

    node_t* walk = queue->head;
    while(walk){
        node_t* next = walk->next;

        printf("element memory freed at %p\n", (void*)walk);
        freeNode(walk);

        walk = next;
    }

    queue->head = queue->tail = NULL;
    queue->size = 0;

    printf("queue memory freed at %p\n", (void*)queue);
    free(queue);

    return QUEUE_OK;
}

queue_status_t queue_enqueue(queue_t* queue, int data){
    if(!queue) return QUEUE_ERR_NULL;

    node_t* node = malloc(sizeof(node_t));
    if(!node) return QUEUE_ERR_ALLOC;

    node->data = data;
    if(queue->tail == NULL){
        node->next = NULL;

        queue->tail = queue->head = node;
        queue->size++;

        return QUEUE_OK;
    }
    node->next = queue->head;

    queue->head = node;
    queue->size++;

    return QUEUE_OK;
}

queue_status_t queue_dequeue(queue_t* queue, int* output){
    if(!queue) return QUEUE_ERR_NULL;
    if(!queue->head) return QUEUE_EMPTY;

    if(queue->head == queue->tail){
        node_t* node = queue->head;

        queue->head = queue->tail = NULL;
        queue->size = 0;

        free(node);
        return QUEUE_OK;
    }

    node_t* prev = NULL;
    node_t* curr = queue->head;
    while(curr->next){
        prev = curr;
        curr = curr->next;
    }

    queue->tail = prev;
    queue->tail->next = NULL;
    queue->size--;

    free(curr);
    return QUEUE_OK;
}

queue_status_t queue_peek_front(const queue_t* queue, int* output){
    if(!queue) return QUEUE_ERR_NULL;
    if(!queue->head) return QUEUE_EMPTY;

    *output = queue->head->data;

    return QUEUE_OK;
}

queue_status_t queue_peek_back(const queue_t* queue, int* output){
    if(!queue) return QUEUE_ERR_NULL;
    if(!queue->head) return QUEUE_EMPTY;

    *output = queue->tail->data;

    return QUEUE_OK;
}

bool queue_is_empty(const queue_t* queue){
    return (!queue) || (!queue->head);
}

size_t queue_size(const queue_t* queue){
    return queue ? queue->size : 0; 
}

queue_status_t queue_print(const queue_t* queue){
    if(!queue) return QUEUE_ERR_NULL;

    printf("Queue [Last -> First]");

    node_t* walk = queue->head;
    int i = queue->size;
    while(walk){
        printf("%d: %d -> ", i, walk->data);

        i--;
        walk = walk->next;
    }
    printf("NULL\n");

    return QUEUE_OK;
}