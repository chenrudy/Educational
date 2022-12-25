/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Author:
- Date:
- Reviewed By:
- heap.h
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

***************************************************************************
                                   DS
****************************************************************************
heap is a specialized tree-based data structure which is essentially an
almost complete tree that satisfies the heap property: in a max heap, for
any given node C, if P is a parent node of C, then the key (the value) of P
is greater than or equal to the key of C.
In a min heap, the key of P is less than or equal to the key of C.
The node at the "top" of the heap (with no parents) is called the root node.

attributes and policy:
- Random Access.
- Dynamic Size.
- growth factor: when reaches last element, doubles capacity.
  when number of elements is 25% of capacity, capacety shrinks by 50%.

-------------------------------------------------------------------------------

Operations on DS:
-

*******************************************************************************/

#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h> /* size_t */

#include "Dynamic_vector.h"
/*******************************************************************************
                                 Typedefs
*******************************************************************************/
typedef int (*heap_cmp_func_t)(const void *new_data, const void *current_data, void *cmp_param);
/*
return 0 if equal
returns > 1 if new_data > current_data
returns < 1 if new_data < current_data
*/

typedef int (*is_match_t)(const void *data, const void *heap_data, void *param);
/*
bool return: 1 if match, 0 if not a match
*/
typedef struct heap heap_t;

/*******************************************************************************
                                Functions
*******************************************************************************/

heap_t *HeapCreate(heap_cmp_func_t cmp, void *cmp_param);
/*O(1) */

void HeapDestroy(heap_t *heap);
/*O(1)*/

int HeapPush(heap_t *heap, void *new_data);
/*O(lon n) */

void *HeapPop(heap_t *heap);
/*O(lon n) */

void *HeapRemove(heap_t *heap, void *to_remove, is_match_t match_func, void *match_param);
/*O(n) */

void *HeapPeek(heap_t *heap);
/*O(1)*/

int HeapReplace(heap_t *heap, void *new_data);
/* O(lon n)*/

size_t HeapSize(const heap_t *heap);
/*O(1) */

int HeapIsEmpty(const heap_t *heap);
/*O(1)*/

void Print(heap_t *heap);

/******************************************************************************
-----
Description: ----
Parameters:
    -
Return value:
    -
Complexity:
Notes:

*****************************************************************************/

#endif /* __HEAP_H__ */
