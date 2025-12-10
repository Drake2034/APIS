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
    node->next = NULL;

    if(queue->tail){
        queue->tail->next = node;
    }else{
        queue->head = node;
    }

    queue->tail = node;
    queue->size++;

    return QUEUE_OK;
}

queue_status_t queue_dequeue(queue_t* queue, int* output){
    if(!queue) return QUEUE_ERR_NULL;
    if(!queue->head) return QUEUE_EMPTY;

    node_t* node = queue->head;
    *output = node->data;

    queue->head = node->next;
    if(!queue->head)
        queue->tail = NULL;

    free(node);
    queue->size--;

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

    printf("Queue [Oldest <- Newest]");

    node_t* walk = queue->head;
    int i = 1;
    while(walk){
        printf("%d: %d <- ", i, walk->data);

        i++;
        walk = walk->next;
    }
    printf("NULL\n");

    return QUEUE_OK;
}

queue_status_t queue_clear(queue_t* queue){
    if(!queue) return QUEUE_ERR_NULL;
    if(!queue->head) return QUEUE_OK;

    node_t* walk = queue->head;
    while(walk){
        node_t* next = walk->next;

        if(walk->data) walk->data = NULL;
        free(walk);

        walk = next;
    }

    queue->head = queue->tail = NULL;
    queue->size = 0;

    return QUEUE_OK;
}

queue_status_t queue_reverse(queue_t* queue){
    if(!queue) return QUEUE_ERR_NULL;
    if(!queue->head) return QUEUE_EMPTY;

    node_t* prev = NULL;
    node_t* curr = queue->head;
    node_t* next = NULL;
    while(curr){
        next = curr->next;
        curr->next = prev;

        prev = curr;
        curr = next;
    }

    queue->tail = queue->head;
    queue->head = prev;

    return QUEUE_OK;
}

queue_status_t queue_clone(queue_t* queue, queue_t** output){
    if(!queue || !output) return QUEUE_ERR_NULL;

    if(!queue->head){
        *output = queue_create();
        if(!*output)
            return QUEUE_ERR_ALLOC;
    }

    queue_t* clonedQ = queue_create();
    if(!clonedQ) return QUEUE_ERR_ALLOC;

    node_t* walk = queue->head;
    node_t* walk = queue->head;
    while(walk){
        queue_status_t status = queue_enqueue(clonedQ, walk->data);
        if(status != QUEUE_OK){
            queue_destroy(clonedQ);
            return status;
        }
        walk = walk->next;
    }
    *output = clonedQ;

    return QUEUE_OK;
}

bool queue_compare(queue_t* queue_1, queue_t* queue_2){
    if(!queue_1 || !queue_2) return false;

    node_t* node1 = queue_1->head;
    node_t* node2 = queue_2->head;
    while(node1 && node2){
        if(node1->data != node2->data) return false;
        node1 = node1->next;
        node2 = node2->next;
    }

    return (!node1) && (!node2);
}
