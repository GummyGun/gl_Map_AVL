#include "glMap.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct testStruct{
    int64_t key;
    glAVL_Anchor anchor;
};

_Bool
emptySet(glAVL_Set set){
    struct testStruct *holder;
    while(!glAVL_getRoot(set, (void**)&holder)){
        printf("%p %ld\n", (void*)holder, holder->key);
        glAVL_deleteNode(set, (void*)holder);
        free(holder);
    }
    return 0;
}

_Bool
deleteNode(glAVL_Set set, int64_t key){
    struct testStruct *holder;
    if(glAVL_getNode(set, &key, (void**)&holder)){
        return 1;
    }
    glAVL_deleteNode(set, (void*)holder);
    free(holder);
    return 0;
}

_Bool
getNode(glAVL_Set set, int64_t key){
    if(glAVL_getNode(set, &key, NULL)){
        return 1;
    }
    return 0;
}

_Bool 
insertNode(glAVL_Set set, int64_t key){
    struct testStruct *test=(struct testStruct*)malloc(sizeof(struct testStruct));
    test->key=key;
    if(!glAVL_insertNode(set, test)){
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
    glAVL_Set set=NULL;
    glAVL_createMap(&set, offsetof(struct testStruct, anchor), offsetof(struct testStruct, key), int64_tCmpFunc);
    
    /*
    int64_t size=0;
    int64_t insert;
    */
    for(int64_t iter=0; iter<7; iter++){
        //insert=rand()%20;
        if(!insertNode(set, iter)){
        /*
            size++;
            printf("%ld se metio %ld\n", size, iter);
        }else{
            printf("----- no se metio %ld\n", insert);
        */
        }
    }
    
    /*
    printf("size: %ld\n", size);
    createNode(&pointer);
    if(glAVL_insertNode(set, pointer)){
        printf("eroror\n");
    }
    */
    
    if(!getNode(set, 4)){
        //printf("si esta\n");
    }else{
        //printf("no esta\n");
    }
    
    
    /*
    for(int64_t iter=0; iter<22; iter++){
        printf("-=-=-=-=-=-=-=-\n");
        printf("%ld\n", glAVL_getSize(set));
        deleteNode(set, iter);
        printf("%ld\n", glAVL_getSize(set));
        printf("-=-=-=-=-=-=-=-\n");
    }
    */
    //test_print(set);
    
    glAVL_Iter iter;
    glAVL_createIter(set, &iter);
    struct testStruct *holder=NULL;
    while(!glAVL_iterNextNode(iter, (void**)&holder)){
        printf("%ld\n", holder->key);
    }
    
    emptySet(set);
    
    glAVL_deleteMap(&set);
    
    return 1;
}
