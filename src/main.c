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
    glMap_print();
    
    struct testStruct *pointer=NULL;
    
    int64_t array[]={1,3,2};
    
    for(int32_t iter=0; iter<3; iter++){
        createNode(&pointer, 1000-array[iter]);
        if(glMap_insertNode(map, pointer)){
            printf("eroror\n");
        }
    }
    
    
    /*
    createNode(&pointer);
    if(glMap_insertNode(map, pointer)){
        printf("eroror\n");
    }
    */
    
    //test_rotateR(map);
    
    return 1;
}
