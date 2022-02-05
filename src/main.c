#include "glMap.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct testStruct{
    int64_t key;
    glMap_Anchor anchor;
};

_Bool
deleteNode(glMap_Map map, int64_t key){
    struct testStruct *holder;
    if(glMap_getNode(map, &key, (void**)&holder)){
        return 1;
    }
    glMap_deleteNode(map, (void*)holder);
    free(holder);
    return 0;
}

_Bool
getNode(glMap_Map map, int64_t key){
    if(glMap_getNode(map, &key, NULL)){
        return 1;
    }
    return 0;
}

_Bool 
insertNode(glMap_Map map, int64_t key){
    struct testStruct *test=(struct testStruct*)malloc(sizeof(struct testStruct));
    test->key=key;
    if(!glMap_insertNode(map, test)){
        return 0;
    }
    free(test);
    return 1;
}

int8_t 
int64_tCmpFunc(void *arg0,void *arg1){
    int64_t *numArg0=(int64_t*)arg0;
    int64_t *numArg1=(int64_t*)arg1;
    if(*numArg0>*numArg1){
        return -1;
    }else if(*numArg1>*numArg0){
        return 1;
    }
    return 0;
}



int
main(){
    glMap_Map map=NULL;
    glMap_createMap(&map, offsetof(struct testStruct, anchor), offsetof(struct testStruct, key), int64_tCmpFunc);
    
    int64_t size=0;
    //int64_t insert;
    for(int64_t iter=0; iter<31; iter++){
        //insert=rand()%20;
        if(!insertNode(map, iter)){
            size++;
            //printf("%ld se metio %ld\n", size, iter);
        }else{
            //printf("----- no se metio %ld\n", insert);
        }
    }
    
    /*
    printf("size: %ld\n", size);
    createNode(&pointer);
    if(glMap_insertNode(map, pointer)){
        printf("eroror\n");
    }
    */
    
    if(!getNode(map, 4)){
        //printf("si esta\n");
    }else{
        //printf("no esta\n");
    }
    
    
    for(int64_t iter=0; iter<22; iter++){
        /*
        printf("-=-=-=-=-=-=-=-\n");
        printf("%ld\n", glMap_getSize(map));
        */
        deleteNode(map, iter);
        /*
        printf("%ld\n", glMap_getSize(map));
        printf("-=-=-=-=-=-=-=-\n");
        */
    }
    //test_print(map);
    
    glMap_Iter iter;
    glMap_createIter(map, &iter);
    struct testStruct *holder=NULL;
    while(!glMap_iterNextNode(iter, (void**)&holder)){
        //printf("%ld\n", holder->key);
    }
    free(map);
    return 1;
}
