#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand((unsigned int)time(NULL));
    
    list_t* list = initList();
    for(int i; i < 10; i++){
        //error: passing argument 2 of 'listPushFront' makes pointer from integer without a cast
        //expected 'void *' but argument is of type 'int'
        int random = rand() % 10;
        listPushFront(list, random);
    }
    
    return 0;
}