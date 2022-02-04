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
static inline enum glMap_enum_side s_getSide(struct glMap_NODE *anchor);

static void s_rotateLeft(struct glMap_NODE *pivot);
static void s_rotateRigth(struct glMap_NODE *pivot);

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
    if(!pivot){
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

static inline 
enum glMap_enum_side 
s_getSide(struct glMap_NODE *anchor){
    return (anchor->father->left==anchor)?glME_left:glME_right;
}

static 
void 
s_rotateLeft(struct glMap_NODE *pivot){
/*  _
   |D|           B    
   / \          / \_
  B   E  ===>  A  |D|  
 / \              / \
A   C            C   E
*/
    if(!pivot->left){
        assert("no left pivot" && 0);
    }
    struct glMap_NODE *nodeB=pivot->left;
    if(nodeB->right){
        nodeB->right->father=pivot;
    }
    pivot->left=nodeB->right;
    pivot->lDepth=1+s_getNodeMaxDepth(pivot->left);
    nodeB->father=pivot->father;
    nodeB->right=pivot;
    nodeB->rDepth=1+s_getNodeMaxDepth(pivot);
    if(s_getSide(pivot)==glME_left){
        nodeB->father->left=nodeB;
        nodeB->father->lDepth=1+s_getNodeMaxDepth(nodeB);
    }else{
        nodeB->father->right=nodeB;
        nodeB->father->rDepth=1+s_getNodeMaxDepth(nodeB);
    }
    pivot->father=nodeB;
}

void
test_rotateR(struct glMap_MAP *pivot){
    struct glMap_NODE *test=pivot->head.left;
    s_rotateRigth(test);
}

void
test_rotateL(struct glMap_MAP *pivot){
    struct glMap_NODE *test=pivot->head.left;
    s_rotateLeft(test);
}

static 
void 
s_rotateRigth(struct glMap_NODE *pivot){
/*_
 |B|                D
 / \              _/ \
A   D     ===>   |B|  E
   / \           / \
  C   E         A   C
*/
    struct glMap_NODE *nodeD=pivot->right;
    if(nodeD->left){
        nodeD->left->father=pivot;
    }
    pivot->right=nodeD->left;
    pivot->rDepth=1+s_getNodeMaxDepth(pivot->right);
    nodeD->father=pivot->father;
    nodeD->left=pivot;
    nodeD->lDepth=1+s_getNodeMaxDepth(pivot);
    if(s_getSide(pivot)==glME_left){
        nodeD->father->left=nodeD;
        nodeD->father->lDepth=1+s_getNodeMaxDepth(nodeD);
    }else{
        nodeD->father->right=nodeD;
        nodeD->father->rDepth=1+s_getNodeMaxDepth(nodeD);
    }
    pivot->father=nodeD;
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
    memset(anchor, 0 ,sizeof(struct glMap_NODE));
    if(container->size==0){
        anchor->father=&(container->head);
        container->head.left=anchor;
        container->size++;
        return 0;
    }
    struct glMap_NODE **pivot=&(container->head.left);
    struct glMap_NODE *pivotFather=&(container->head);
    enum glMap_enum_side side=glME_left;
    
    while((*pivot)){
        pivotFather=(*pivot);
        switch(side=container->cmp(s_getKeyFromNode(container, (*pivot)), s_getKeyFromNode(container, anchor))){
            case glME_left:{
                pivot=&((*pivot)->left);
                break;
            }
            case glME_right:{
                pivot=&((*pivot)->right);
                break;
            }
            default:{
                assert(0);
            }
        }
    }
    //insertNode
    *pivot=anchor;
    (*pivot)->father=pivotFather;
    //balance Upwards
    struct glMap_NODE *upwardPtr=(*pivot);
    
    while(upwardPtr->father){
        side=s_getSide(upwardPtr);
        switch(side){
            case glME_left:{
                upwardPtr->father->lDepth=1+s_getNodeMaxDepth(upwardPtr);
                break;
            }
            case glME_right:{
                upwardPtr->father->rDepth=1+s_getNodeMaxDepth(upwardPtr);
                break;
            }
            default:{
                assert(0);
            }
        }
        if(!upwardPtr->father){
            break;
        }
        int8_t balFac=upwardPtr->rDepth-upwardPtr->lDepth;
        if((balFac<-1)){
            if(upwardPtr->left->lDepth>=upwardPtr->left->rDepth){
                s_rotateLeft(upwardPtr);
            }else{
                s_rotateRigth(upwardPtr->left);
                s_rotateLeft(upwardPtr);
            }
            //assert("balancear"&&0);
        }else if((balFac>1)){
            if(upwardPtr->right->rDepth>=upwardPtr->right->lDepth){
                s_rotateRigth(upwardPtr);
            }else{
                s_rotateLeft(upwardPtr->right);
                s_rotateRigth(upwardPtr);
            }
        }
        upwardPtr=upwardPtr->father;
    }
    return 0;
}

//delete


//search


//iterate


