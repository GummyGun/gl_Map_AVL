#include "glMap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* 
 * defined in header file
typedef struct glMap_NODE{
    struct glMap_NODE *father;
    struct glMap_NODE *left;
    struct glMap_NODE *right;
    int8_t lDepth;
    int8_t rDepth;
}glMap_anchor;
*/

struct glMap_MAP{
    struct glMap_NODE head;
    int64_t size;
    ptrdiff_t startOffset;
    ptrdiff_t keyOffset;
    int8_t (*cmp)(void*, void*);
};

enum glMap_enum_side{
    glME_left=-1,
    glME_rep,
    glME_right
};

//general
//insert
//delete
//search
//iterate


//general

//_Bool glMap_print();
static inline int8_t s_getNodeMaxDepth(struct glMap_NODE *pivot);
static inline void* s_getKeyFromNode(struct glMap_MAP *container, void *node);
static inline void* s_getStartFromNode(struct glMap_MAP *container, void *node);

//insert

//_Bool glMap_createMap(struct glMap_MAP **container, ptrdiff_t offset, ptrdiff_t key);

//_Bool glMap_insertNode(struct glMap_MAP *container, void *toInsert);

//delete


//search


//iterate



//general

_Bool
glMap_print(){
    printf("hola\n");
    return 0;
}

static inline
int8_t
s_getNodeMaxDepth(struct glMap_NODE *pivot){
    if(pivot){
        return -1;
    }
    return pivot->lDepth>pivot->rDepth?pivot->lDepth:pivot->rDepth;
}

static inline
void*
s_getKeyFromNode(struct glMap_MAP *container, void *node){
    return ((int8_t*)node+container->keyOffset);
}

static inline
void*
s_getStartFromNode(struct glMap_MAP *container, void *node){
    return ((int8_t*)node+container->startOffset);
}

//insert

_Bool
glMap_createMap(struct glMap_MAP **container, ptrdiff_t offset, ptrdiff_t key, int8_t(*cmp)(void*, void*)){
    if(*container){
        return 1;
    }
    *container=(struct glMap_MAP*)malloc(sizeof(struct glMap_MAP));
    if(!*container){
        return 1;
    }
    (*container)->startOffset=-offset;
    (*container)->keyOffset=key-offset;
    (*container)->size=0;
    (*container)->cmp=cmp;
    memset(&(*container)->head, 0, sizeof(struct glMap_NODE));
    return 0;
}

_Bool
glMap_insertNode(struct glMap_MAP *container, void *toInsert){
    if(!container){
        return 1;
    }
    struct glMap_NODE *anchor=(struct glMap_NODE*)((int8_t*)toInsert-container->startOffset);
    if(container->size==0){
        memset(anchor, 0 ,sizeof(struct glMap_NODE));
        anchor->father=&(container->head);
        container->head.left=anchor;
        container->size++;
        return 0;
    }
    struct glMap_NODE *pivot=container->head.left;
    enum glMap_enum_side side;
    
    switch(side=container->cmp(s_getKeyFromNode(container, pivot), s_getKeyFromNode(container, anchor))){
        case glME_left:{
            assert("left"&& 0);
            break;
        }
        case glME_right:{
            assert("right"&& 0);
            break;
        }
        default:{
            assert(0);
        }
    }
    
    return 1;
}

//delete


//search


//iterate


