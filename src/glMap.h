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

/*
struct glMap_MAP{
    struct glMap_NODE head;
    int64_t size;
    ptrdiff_t startOffset;
    ptrdiff_t keyOffset
};
*/

_Bool glMap_print();

void test_rotateR(struct glMap_MAP *pivot);
void test_rotateL(struct glMap_MAP *pivot);

//insert

_Bool glMap_createMap(struct glMap_MAP **container, ptrdiff_t offset, ptrdiff_t key, int8_t(*cmp)(void*,void*));

_Bool glMap_insertNode(struct glMap_MAP *container, void *toInsert);

//delete
//search
//iterate

#endif /*__GUM__GL__AVL__MAP__*/
