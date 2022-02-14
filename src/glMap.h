#ifndef __GUM__GL__AVL__MAP__
#define __GUM__GL__AVL__MAP__ 1

#include <stdint.h>
#include <stddef.h>

typedef struct glAVL_MAP *glAVL_Map;
typedef struct glAVL_MAP *glAVL_Set;

typedef struct glAVL_NODE{
    struct glAVL_NODE *father;
    struct glAVL_NODE *left;
    struct glAVL_NODE *right;
    int8_t lDepth;
    int8_t rDepth;
}glAVL_Anchor;

typedef struct glAVL_ITER{
    struct glAVL_MAP *map;
    struct glAVL_NODE *actualNode;
    struct glAVL_ITER *autoRef;
    int8_t state;
}glAVL_Iter;

/*
struct glAVL_MAP{
    struct glAVL_NODE head;
    int64_t size;
    ptrdiff_t startOffset;
    ptrdiff_t keyOffset
};
*/

//void test_print(struct glAVL_MAP *map);

//insert

_Bool glAVL_createMap(struct glAVL_MAP **container, ptrdiff_t offset, ptrdiff_t key, int8_t(*cmp)(void*,void*));

_Bool glAVL_insertNode(struct glAVL_MAP *container, void *toInsert);

//delete

_Bool glAVL_deleteNode(struct glAVL_MAP *container, void *mem);

void glAVL_restartMap(struct glAVL_MAP *container);

void glAVL_deleteMap(struct glAVL_MAP **container);

//get

int64_t glAVL_getSize(struct glAVL_MAP *map);

_Bool glAVL_getNode(struct glAVL_MAP *container, void *key, void** pointer);

_Bool glAVL_getRoot(struct glAVL_MAP *container, void** pointer);

//iterate

void glAVL_createIter(struct glAVL_MAP *map, struct glAVL_ITER *iter);

_Bool glAVL_iterNextNode(struct glAVL_ITER iter, void **pointer);

#endif /*__GUM__GL__AVL__MAP__*/
