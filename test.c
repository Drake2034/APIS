#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL));
    
    list_t* list = initList();
    for(int i; i < 10; i++){
        //error: passing argument 2 of 'listPushFront' makes pointer from integer without a cast
        //expected 'void *' but argument is of type 'int'
        int* number = (int*)malloc(sizeof(int));
        *number = 4;
        listPushFront(list, (int*) number);
        free(number);
    }
    freeList(list);
    
    return 0;
}