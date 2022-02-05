#ifndef __GUM__GL__AVL__MAP__
#define __GUM__GL__AVL__MAP__ 1

#include <stdint.h>
#include <stddef.h>

typedef struct glMap_MAP *glMap_Map;

typedef struct glMap_NODE{
    struct glMap_NODE *father;
    struct glMap_NODE *left;
    struct glMap_NODE *right;
    int8_t lDepth;
    int8_t rDepth;
}glMap_Anchor;

typedef struct glMap_ITER{
    struct glMap_MAP *map;
    struct glMap_NODE *actualNode;
    struct glMap_ITER *autoRef;
    int8_t state;
}glMap_Iter;

/*
struct glMap_MAP{
    struct glMap_NODE head;
    int64_t size;
    ptrdiff_t startOffset;
    ptrdiff_t keyOffset
};
*/

void test_print(struct glMap_MAP *map);

//insert

_Bool glMap_createMap(struct glMap_MAP **container, ptrdiff_t offset, ptrdiff_t key, int8_t(*cmp)(void*,void*));

_Bool glMap_insertNode(struct glMap_MAP *container, void *toInsert);

//delete

_Bool glMap_deleteNode(struct glMap_MAP *container, void *mem);

//get

int64_t glMap_getSize(struct glMap_MAP *map);

_Bool glMap_getNode(struct glMap_MAP *container, void *key, void** pointer);

//iterate

void glMap_createIter(struct glMap_MAP *map, struct glMap_ITER *iter);

_Bool glMap_iterNextNode(struct glMap_ITER iter, void **pointer);

#endif /*__GUM__GL__AVL__MAP__*/
