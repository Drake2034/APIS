#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    list_t* list = initList();
    for(int i; i < 10; i++){
        int random = rand() % 10;
        listPushFront(list, random);
    }
    
    return 0;
}