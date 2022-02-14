#include "glMap.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <stdio.h>
/* 
 * defined in header file
typedef struct glAVL_NODE{
    struct glAVL_NODE *father;
    struct glAVL_NODE *left;
    struct glAVL_NODE *right;
    int8_t lDepth;
    int8_t rDepth;
}glAVL_anchor;
*/

struct glAVL_MAP{
    struct glAVL_NODE head;
    int64_t size;
    ptrdiff_t startOffset;
    ptrdiff_t keyOffset;
    int8_t (*cmp)(void*, void*);
};

enum glAVL_enum_side{
    glME_left=0,
    glME_right,
    glME_else
};

enum glAVL_enum_iterState{
    glME_down=0,
    glME_turn
};

//general
//insert
//delete
//get
//iterate


//general

//_Bool glAVL_print();
static inline int8_t s_getNodeMaxDepth(struct glAVL_NODE *pivot);
static inline void* s_getKeyFromNode(struct glAVL_MAP *container, struct glAVL_NODE *node);
static inline void* s_getStartFromNode(struct glAVL_MAP *container, struct glAVL_NODE *node);
static inline struct glAVL_NODE* s_getNodeFromStart(struct glAVL_MAP * container, void *start);
static inline enum glAVL_enum_side s_getSide(struct glAVL_NODE *anchor);
static inline void s_balanceFromBottom(struct glAVL_NODE *pivot);

static void s_rotateLeft(struct glAVL_NODE *pivot);
static void s_rotateRigth(struct glAVL_NODE *pivot);

//insert

//_Bool glAVL_createMap(struct glAVL_MAP **container, ptrdiff_t offset, ptrdiff_t key);

//_Bool glAVL_insertNode(struct glAVL_MAP *container, void *toInsert);

//delete

//_Bool glAVL_deleteNode(struct glAVL_MAP *container, void *mem);

//void glAVL_restartMap(struct glAVL_MAP *container);

//void glAVL_deleteMap(struct glAVL_MAP **container);

//get

//int64_t glAVL_getSize(struct glAVL_MAP *container);

//iterate

//void glAVL_createIter(struct glAVL_MAP *container, struct glAVL_ITER *iter);
//_Bool glAVL_iterNextNode(struct glAVL_ITER *iter, void **pointer);


//debug
void test_printNode(struct glAVL_NODE *node, int over);

void
test_print(struct glAVL_MAP *container){
    test_printNode(container->head.left, 5);
}

void
test_printNode(struct glAVL_NODE *node, int over){
    if(node->left){
        test_printNode(node->left, over-1);
    }
    int indent=over;
    while(indent--)printf("\t");
    printf("anchor\n");
    if(node->right){
        test_printNode(node->right, over-1);
    }
}

//general

static inline
int8_t
s_getNodeMaxDepth(struct glAVL_NODE *pivot){
    if(!pivot){
        return -1;
    }
    return pivot->lDepth>pivot->rDepth?pivot->lDepth:pivot->rDepth;
}

static inline
void*
s_getKeyFromNode(struct glAVL_MAP *container, struct glAVL_NODE *node){
    return ((int8_t*)node+container->keyOffset);
}

static inline
void*
s_getStartFromNode(struct glAVL_MAP *container, struct glAVL_NODE *node){
    return ((int8_t*)node+container->startOffset);
}

static inline 
struct glAVL_NODE* 
s_getNodeFromStart(struct glAVL_MAP * container, void *start){
    return (struct glAVL_NODE*)((int8_t*)start-container->startOffset);
}

static inline 
enum glAVL_enum_side 
s_getSide(struct glAVL_NODE *anchor){
    return (anchor->father->left==anchor)?glME_left:(anchor->father->right==anchor)?glME_right:glME_else;
}

static inline
void
s_balanceFromBottom(struct glAVL_NODE *pivot){
    enum glAVL_enum_side side=glME_left;
    struct glAVL_NODE *nodePtr=pivot;
    if(pivot->left){
        nodePtr=pivot->left;
        pivot->lDepth=1+s_getNodeMaxDepth(pivot->left);
    }
    if(pivot->right){
        nodePtr=pivot->right;
        pivot->rDepth=1+s_getNodeMaxDepth(pivot->right);
    }
    pivot=nodePtr;
    while(pivot->father){
        side=s_getSide(pivot);
        switch(side){
            case glME_left:{
                pivot->father->lDepth=1+s_getNodeMaxDepth(pivot);
                break;
            }
            case glME_right:{
                pivot->father->rDepth=1+s_getNodeMaxDepth(pivot);
                break;
            }
            default:{
                return;
                assert(0);
            } 
        }
        if(!pivot->father){
            break;
        }
        int8_t balFac=pivot->rDepth-pivot->lDepth;
        if((balFac<-1)){
            if(pivot->left->lDepth>=pivot->left->rDepth){
                s_rotateLeft(pivot);
            }else{
                s_rotateRigth(pivot->left);
                s_rotateLeft(pivot);
            }
        }else if((balFac>1)){
            if(pivot->right->rDepth>=pivot->right->lDepth){
                s_rotateRigth(pivot);
            }else{
                s_rotateLeft(pivot->right);
                s_rotateRigth(pivot);
            }
        }
        pivot=pivot->father;
    } 
    pivot->lDepth=1+s_getNodeMaxDepth(pivot->left);
}

static 
void 
s_rotateLeft(struct glAVL_NODE *pivot){
/*  _
   |D|           B    
   / \          / \_
  B   E  ===>  A  |D|  
 / \              / \
A   C            C   E
    if(!pivot->left){
        assert("no left pivot" && 0);
    }
*/
    struct glAVL_NODE *nodeB=pivot->left;
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

static 
void 
s_rotateRigth(struct glAVL_NODE *pivot){
/*_
 |B|                D
 / \              _/ \
A   D     ===>   |B|  E
   / \           / \
  C   E         A   C
    if(!pivot->right){
        assert("no right pivot" && 0);
    }
*/
    struct glAVL_NODE *nodeD=pivot->right;
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
glAVL_createMap(struct glAVL_MAP **container, ptrdiff_t offset, ptrdiff_t key, int8_t(*cmp)(void*, void*)){
    if(*container){
        return 1;
    }
    *container=(struct glAVL_MAP*)malloc(sizeof(struct glAVL_MAP));
    if(!*container){
        return 1;
    }
    (*container)->startOffset=-offset;
    (*container)->keyOffset=key-offset;
    (*container)->size=0;
    (*container)->cmp=cmp;
    memset(&(*container)->head, 0, sizeof(struct glAVL_NODE));
    return 0;
}

_Bool
glAVL_insertNode(struct glAVL_MAP *container, void *toInsert){
    if(!container){
        return 1;
    }
    struct glAVL_NODE *anchor=s_getNodeFromStart(container, toInsert);
    memset(anchor, 0 ,sizeof(struct glAVL_NODE));
    if(container->size==0){
        anchor->father=&(container->head);
        container->head.left=anchor;
        container->size++;
        return 0;
    }
    struct glAVL_NODE **pivot=&(container->head.left);
    struct glAVL_NODE *pivotFather=&(container->head);
    enum glAVL_enum_side side=glME_left;
    
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
                return 1;
                assert(0);
            }
        }
    }
    //insertNode
    container->size++;
    *pivot=anchor;
    (*pivot)->father=pivotFather;
    //balance Upwards
    s_balanceFromBottom(*pivot);
    return 0;
}

//delete


_Bool 
testDel(struct glAVL_MAP *container, void *mem){
    if(!container | !mem){
        return 1;
    }
    struct glAVL_NODE *anchor=s_getNodeFromStart(container, mem);
    struct glAVL_NODE *pivot=anchor;
    if(!pivot->right){
        if(!pivot->left){
            if(s_getSide(pivot)==glME_left){
                pivot->father->left=NULL;
                pivot->father->lDepth=0;
            }else{
                pivot->father->right=NULL;
                pivot->father->rDepth=0;
            }
        }else{
            if(s_getSide(pivot)==glME_left){
                pivot->father->left=pivot->left;
                pivot->father->lDepth=1;
            }else{
                pivot->father->right=pivot->left;
                pivot->father->rDepth=1;
            }
            //it is one level from leafs
        }
    }else{
        pivot=pivot->right;
        while(pivot->left){
            pivot=pivot->left;
            //go to leaf or one level away
        }
        if(!pivot->right){
            if(s_getSide(pivot)==glME_left){
                pivot->father->left=NULL;
                pivot->father->lDepth=0;
            }else{
                pivot->father->right=NULL;
                pivot->father->rDepth=0;
            }
        }else{
            pivot->father->right=pivot->right;
            pivot->father->rDepth=1;
            //it is one level from leaf
        }
        //*pivotFather=;
        memcpy(pivot, anchor, sizeof(struct glAVL_NODE));
        if(s_getSide(anchor)==glME_left){
            anchor->father->left=pivot;
        }else{
            anchor->father->right=pivot;
        }
        if(pivot->left){
            pivot->left->father=pivot;
        }
        if(pivot->right){
            pivot->right->father=pivot;
        }
    }
    
    s_balanceFromBottom(pivot);
    container->size--;
    return 0;
}

_Bool 
glAVL_deleteNode(struct glAVL_MAP *container, void *mem){
    if(!container | !mem){
        return 1;
    }
    struct glAVL_NODE *target=s_getNodeFromStart(container, mem);
    struct glAVL_NODE *pivot=target, *substitute, *depth;
    struct glAVL_NODE **targetAutoRef;
    int8_t *pivotFatherDepth;
    
    
    if(s_getSide(pivot)==glME_left){
        targetAutoRef=&(pivot->father->left);
        pivotFatherDepth=&(pivot->father->lDepth);
    }else{
        targetAutoRef=&(pivot->father->right);
        pivotFatherDepth=&(pivot->father->rDepth);
    }
    
    if(pivot->right){
        if(pivot->left){
            pivot=pivot->right;
            //struct glAVL_NODE *hold=pivot;
            while(pivot->left){
                pivot=pivot->left;
            }
            substitute=pivot;
            if(pivot->right){
                if(s_getSide(pivot)==glME_left){
                    pivot->father->left=pivot->right;
                    pivot->father->lDepth=1;
                    depth=pivot->father->left;
                }else{
                    pivot->father->right=pivot->right;
                    pivot->father->rDepth=1;
                    depth=pivot->father->left;
                    depth=pivot->father->right;
                }
            }else{
                if(s_getSide(pivot)==glME_left){
                    pivot->father->left=NULL;
                    pivot->father->lDepth=0;
                    depth=pivot->father;
                }else{
                    pivot->father->right=NULL;
                    pivot->father->rDepth=0;
                    depth=pivot->father;
                }
            }
            memcpy(substitute, target, sizeof(struct glAVL_NODE));
            *targetAutoRef=substitute;
            if(substitute->right){
                substitute->right->father=substitute;
            }
            substitute->left->father=substitute;
            //here we substitute the node
        }else{
            *targetAutoRef=pivot->right;
            *pivotFatherDepth=1;
            depth=pivot->right;
        }
    }else{
        if(pivot->left){
            pivot->left->father=pivot->father;
            *targetAutoRef=pivot->left;
            *pivotFatherDepth=1;
            depth=pivot->left;
        }else{
            *targetAutoRef=NULL;
            *pivotFatherDepth=0;
            depth=pivot;
        }
    }
    container->size--;
    
    s_balanceFromBottom(depth);
    return 0;
}

void 
glAVL_restartMap(struct glAVL_MAP *container){
    memset(&(container->head), 0, sizeof(struct glAVL_NODE));
    container->size=0;
}

void
glAVL_deleteMap(struct glAVL_MAP **container){
    free(*container);
    *container=NULL;
}

//get

int64_t 
glAVL_getSize(struct glAVL_MAP *container){
    return container->size;
}

_Bool 
glAVL_getNode(struct glAVL_MAP *container, void *key, void** pointer){
    if(!container){
        return 1;
    }
    struct glAVL_NODE *pivot=container->head.left;
    enum glAVL_enum_side side; 
    while(pivot){
        side=container->cmp(s_getKeyFromNode(container, pivot), key);
        switch(side){
            case glME_left:{
                pivot=pivot->left;
                break;
            }
            case glME_right:{
                pivot=pivot->right;
                break;
            }
            default:{
                if(pointer){
                    *pointer=s_getStartFromNode(container, pivot);
                }
                return 0;
            }
        }
        
    }
    return 1;
}

_Bool 
glAVL_getRoot(struct glAVL_MAP *container, void** pointer){
    if(!container){
        return 1;
    }
    if(container->size==0){
        return 1;
    }
    *pointer=s_getStartFromNode(container, container->head.left);
    return 0;
}

//iterate

void
glAVL_createIter(struct glAVL_MAP *container, struct glAVL_ITER *iter){
    iter->autoRef=iter;
    iter->map=container;
    if(container->size==0){
        iter->actualNode=&(container->head);
        return;
    }
    iter->actualNode=container->head.left;
    while(iter->actualNode->left){
        iter->actualNode=iter->actualNode->left;
    }
    iter->state=glME_down;
}

_Bool 
glAVL_iterNextNode(struct glAVL_ITER iterCpy, void **pointer){
    struct glAVL_ITER *iter=iterCpy.autoRef;
    if(!iter->actualNode->father){
        *pointer=NULL;
        return 1;
    }
    *pointer=s_getStartFromNode(iter->map, iter->actualNode);
    switch(iter->state){
        case glME_down:{
            if(iter->actualNode->right){ //<--------------1
                iter->actualNode=iter->actualNode->right;
                return 0;
            }
            break;
        }
        case glME_turn:{                                    
            if(!iter->actualNode->right){ //<-------------2 
                break;                                      
            }                                               
            iter->actualNode=iter->actualNode->right;       
            while(iter->actualNode->left){                  
                iter->actualNode=iter->actualNode->left;    
            }
            iter->state=glME_down;
            return 0;
        }
    }
    while(s_getSide(iter->actualNode)==glME_right){
        iter->actualNode=iter->actualNode->father;
    }
    iter->actualNode=iter->actualNode->father;
    iter->state=glME_turn;
    return 0;
}
/* Apendix
1.- case there is right son on A going down
   C
 _/
|A|
  \
   B

2.- case there is no right node on C going up
 A                                      
  \_
  |C|
  /
 B

*/
