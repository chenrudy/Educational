/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- Author: Chen Rudy
- Date: 05/19/22
- Reviewed By: May Sohatchevzzki
*heap.c
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <assert.h> /* for assert */
#include <stdlib.h> /* for malloc free */
#include <stdio.h>  /* for printf */

#include "Heap.h" /* heap API */

enum returns
{
    TRUE = 1,
    SUCCESS = 0
};

enum match_status
{
    NOT_MATCH = 0,
    MATCH
};

enum children
{
    TWO_CHILDREN = 2,
    SINGLE_CHILD = 1,
    LEAF = 0
};

#define MIN_CAP (10)
#define ROOT (0)
#define PARENT(i) (i > 1 ? ((i - 1) / 2) : 0)
#define LCHILD(i) (i * 2 + 1)
#define RCHILD(i) (i * 2 + 2)

struct heap
{
    d_vector_t *heap;
    heap_cmp_func_t cmp;
    void *cmp_param;
};

static void InitHeap(heap_t *heap, d_vector_t *vector, heap_cmp_func_t cmp, void *cmp_param);
static void HeapifyUp(heap_t *heap, size_t index);
static void HeapifyDown(heap_t *heap, size_t index);
static int HasChild(size_t index, size_t size);
static size_t GetMaxChild(heap_t *heap, size_t index);
static void Swap(void **i, void **j);
static void SwapNPop(heap_t *heap, size_t i);

heap_t *HeapCreate(heap_cmp_func_t cmp, void *cmp_param)
{
    heap_t *heap = NULL;
    d_vector_t *vector = NULL;

    assert(NULL != cmp);

    heap = (heap_t *)malloc(sizeof(heap_t));
    if (NULL == heap)
    {
        return NULL;
    }

    vector = DVectorCreate(sizeof(size_t), MIN_CAP);
    if (NULL == vector)
    {
        free(heap);
        heap = NULL;
        return NULL;
    }

    InitHeap(heap, vector, cmp, cmp_param);

    return heap;
}

void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);

    DVectorDestroy(heap->heap);
    heap->heap = NULL;

    free(heap);
    heap = NULL;
}

void *HeapPeek(heap_t *heap)
{
    assert(NULL != heap);
    assert(!HeapIsEmpty(heap));

    return *(void **)DVectorGetElement(heap->heap, ROOT);
}

size_t HeapSize(const heap_t *heap)
{
    assert(NULL != heap);

    return DVectorSize(heap->heap);
}

int HeapIsEmpty(const heap_t *heap)
{
    assert(NULL != heap);

    return DVectorIsEmpty(heap->heap);
}

int HeapPush(heap_t *heap, void *new_data)
{
    int status = SUCCESS;

    assert(NULL != heap);

    status = DVectorPushBack(heap->heap, &new_data);
    HeapifyUp(heap, HeapSize(heap) - 1);

    return status;
}

static void HeapifyUp(heap_t *heap, size_t index)
{
    void **current_data = NULL;
    void **parent_data = NULL;

    assert(NULL != heap);

    current_data = DVectorGetElement(heap->heap, index);
    parent_data = DVectorGetElement(heap->heap, PARENT(index));

    if (0 == index || 0 <= heap->cmp(*current_data, *parent_data, heap->cmp_param))
    {
        return;
    }

    Swap(current_data, parent_data);
    HeapifyUp(heap, PARENT(index));
}

void *HeapPop(heap_t *heap)
{
    void **root_data = NULL;
    void *temp = NULL;

    assert(NULL != heap);
    assert(!HeapIsEmpty(heap));

    root_data = DVectorGetElement(heap->heap, ROOT);
    temp = *root_data;

    SwapNPop(heap, ROOT);

    return temp;
}

int HeapReplace(heap_t *heap, void *new_data)
{
    void **root_data = NULL;

    assert(NULL != heap);
    assert(!HeapIsEmpty(heap));

    root_data = DVectorGetElement(heap->heap, ROOT);
    Swap(root_data, &new_data);
    HeapifyDown(heap, ROOT);

    return SUCCESS;
}

static void HeapifyDown(heap_t *heap, size_t index)
{
    void **current_data = NULL;
    void **child_data = NULL;
    int childern = LEAF;
    size_t direction = 0;

    assert(NULL != heap);

    childern = HasChild(index, HeapSize(heap));
    if (LEAF == childern)
    {
        return;
    }

    current_data = DVectorGetElement(heap->heap, index);
    if (TWO_CHILDREN == childern)
    {
        direction = GetMaxChild(heap, index);
    }
    else
    {
        direction = LCHILD(index);
    }

    child_data = DVectorGetElement(heap->heap, direction);

    if (0 <= heap->cmp(*current_data, *child_data, heap->cmp_param))
    {
        Swap(current_data, child_data);
        HeapifyDown(heap, direction);
    }
}

void *HeapRemove(heap_t *heap, void *to_remove, is_match_t match_func, void *match_param)
{
    void *removed_data = NULL;
    void **heap_data = NULL;
    size_t index = ROOT;
    int match_res = NOT_MATCH;

    assert(NULL != heap);
    assert(NULL != match_func);
    assert(!HeapIsEmpty(heap));

    while (index < HeapSize(heap) && MATCH != match_res)
    {
        heap_data = DVectorGetElement(heap->heap, index);
        match_res = match_func(to_remove, *heap_data, match_param);
        ++index;
    }

    if (MATCH == match_res)
    {
        removed_data = *heap_data;
        SwapNPop(heap, index - 1);
        return removed_data;
    }

    return NULL;
}

static void SwapNPop(heap_t *heap, size_t i)
{
    void **end_data = NULL;
    void **ToRemove_data = NULL;

    assert(NULL != heap);

    end_data = DVectorGetElement(heap->heap, HeapSize(heap) - 1);
    ToRemove_data = DVectorGetElement(heap->heap, i);

    Swap(ToRemove_data, end_data);
    DVectorPopBack(heap->heap);

    if (HeapSize(heap))
    {
        HeapifyDown(heap, i);
    }
}

static int HasChild(size_t index, size_t size)
{
    size_t left = LCHILD(index);
    size_t right = RCHILD(index);

    if (right <= (size - 1))
    {
        return TWO_CHILDREN;
    }
    if (left <= (size - 1))
    {
        return SINGLE_CHILD;
    }
    return LEAF;
}

static size_t GetMaxChild(heap_t *heap, size_t index)
{
    void *Left_data = NULL;
    void *right_data = NULL;
    int cmp_res = 0;

    assert(NULL != heap);

    Left_data = *(void **)DVectorGetElement(heap->heap, LCHILD(index));
    right_data = *(void **)DVectorGetElement(heap->heap, RCHILD(index));
    cmp_res = heap->cmp(Left_data, right_data, heap->cmp_param);

    if (0 <= cmp_res)
    {
        return RCHILD(index);
    }

    return LCHILD(index);
}

static void Swap(void **i, void **j)
{
    void *temp = *i;

    assert(NULL != i);
    assert(NULL != j);

    *i = *j;
    *j = temp;
}

static void InitHeap(heap_t *heap, d_vector_t *vector, heap_cmp_func_t cmp, void *cmp_param)
{
    assert(NULL != cmp);
    assert(NULL != heap);
    assert(NULL != vector);

    heap->heap = vector;
    heap->cmp = cmp;
    heap->cmp_param = cmp_param;
}

void Print(heap_t *heap)
{
    size_t i = 0;
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    for (i = 0; i < HeapSize(heap); ++i)
    {
        printf("%d ", **(int **)DVectorGetElement(heap->heap, i));
    }
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
