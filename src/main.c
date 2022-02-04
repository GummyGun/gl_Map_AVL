#include "glMap.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


struct testStruct{
    int64_t key;
    glMap_Anchor anchor;
};

_Bool 
createNode(struct testStruct **test, int64_t key){
    *test=(struct testStruct*)malloc(sizeof(struct testStruct));
    (*test)->key=key;
    return 0;
}

int8_t 
cmp(void *arg0,void *arg1){
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
    glMap_createMap(&map, offsetof(struct testStruct, anchor), offsetof(struct testStruct, key), cmp);
    
    struct testStruct *pointer=NULL;
    
    
    int64_t size=0;
    for(int32_t iter=0; iter<60; iter++){
        createNode(&pointer, rand()%10);
        //createNode(&pointer, iter);
        
        if(!glMap_insertNode(map, pointer)){
            size++;
        }else{
            free(pointer);
        }
    }
    printf("size: %ld\n", size);
    printf("size in map: %ld\n", glMap_getSize(map));
    /*
    */
    
    
    /*
    createNode(&pointer);
    if(glMap_insertNode(map, pointer)){
        printf("eroror\n");
    }
    */
    //test_rotateR(map);
    
    //test_print(map);
    
    glMap_Iter iter;
    glMap_createIter(map, &iter);
    struct testStruct *holder=NULL;
    while(!glMap_iterNextVal(iter, (void**)&holder)){
        printf("%ld\n", holder->key);
    }
    return 1;
}
