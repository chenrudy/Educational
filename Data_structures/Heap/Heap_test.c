/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- Author: Chen Rudy
- Date: 05/19/22
- Reviewed By:
*heap_test.c
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <stdio.h>  /* for printf */
#include <assert.h> /* for asserts */

#include "Heap.h"

#define RUNTEST(test)                     \
    {                                     \
        if (!(test))                      \
        {                                 \
            printf("\x1b[0;34m");         \
            printf("OK!\n");              \
            printf("\x1b[0m");            \
        }                                 \
        else                              \
        {                                 \
            printf("\x1b[1;31m");         \
            printf("\nTest Failed!\n\n"); \
            printf("\x1b[0m");            \
        }                                 \
    }

enum return_val
{
    SUCCESS = 0,
    FALSE = 0,
    EQUAL = 0,
    FAILURE = 1,
    TRUE = 1
};

static int CompereInt(int expected_val, int recieved_val, int line);
int CmpFunc(const void *new_data, const void *current_data, void *cmp_param);
int IsMatch(const void *data, const void *heap_data, void *param);
int CreatDestroy();
int InsertTest();
int PopTest();
int ReplaceTest();
int RemoveTest();

int main()
{
    RUNTEST(CreatDestroy());
    RUNTEST(InsertTest());
    RUNTEST(PopTest());
    RUNTEST(ReplaceTest());
    RUNTEST(RemoveTest());

    return 0;
}

int CreatDestroy()
{
    int status = 0;
    int param = 2;
    heap_t *heap = HeapCreate(CmpFunc, &param);
    status += CompereInt(TRUE, NULL != heap, __LINE__);
    status += CompereInt(TRUE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(0, HeapSize(heap), __LINE__);

    HeapDestroy(heap);
    return status;
}

int InsertTest()
{
    int status = 0;
    int param = 2;
    int a = 4;
    int b = 5;
    int c = 8;
    int d = 7;
    int e = 3;
    int f = 50;
    int g = 10;
    int h = 0;

    heap_t *heap = HeapCreate(CmpFunc, &param);
    status += CompereInt(TRUE, NULL != heap, __LINE__);
    status += CompereInt(TRUE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(0, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &d), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(1, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &b), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(2, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &c), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(3, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &a), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(4, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &e), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(5, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &f), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(6, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &g), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(7, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &h), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(8, HeapSize(heap), __LINE__);

    Print(heap);

    HeapDestroy(heap);
    return status;
}

int PopTest()
{
    int status = 0;
    int param = 2;
    int a = 4;
    int b = 5;
    int c = 8;
    int d = 7;
    int e = 3;
    int f = 50;
    int g = 10;
    int h = 0;

    heap_t *heap = HeapCreate(CmpFunc, &param);
    status += CompereInt(TRUE, NULL != heap, __LINE__);
    status += CompereInt(TRUE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(0, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &b), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(1, HeapSize(heap), __LINE__);
    Print(heap);
    status += CompereInt(SUCCESS, HeapPush(heap, &d), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(2, HeapSize(heap), __LINE__);
    Print(heap);
    status += CompereInt(SUCCESS, HeapPush(heap, &c), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(3, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &a), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(4, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &e), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(5, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &f), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(6, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &g), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(7, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &h), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(8, HeapSize(heap), __LINE__);

    Print(heap);

    status += CompereInt(0, *(int *)HeapPop(heap), __LINE__);
    status += CompereInt(3, *(int *)HeapPeek(heap), __LINE__);
    status += CompereInt(7, HeapSize(heap), __LINE__);
    Print(heap);

    status += CompereInt(3, *(int *)HeapPop(heap), __LINE__);
    status += CompereInt(4, *(int *)HeapPeek(heap), __LINE__);
    status += CompereInt(6, HeapSize(heap), __LINE__);
    Print(heap);

    status += CompereInt(4, *(int *)HeapPop(heap), __LINE__);
    status += CompereInt(5, *(int *)HeapPeek(heap), __LINE__);
    status += CompereInt(5, HeapSize(heap), __LINE__);
    Print(heap);

    status += CompereInt(5, *(int *)HeapPop(heap), __LINE__);
    status += CompereInt(7, *(int *)HeapPeek(heap), __LINE__);
    status += CompereInt(4, HeapSize(heap), __LINE__);
    Print(heap);

    status += CompereInt(7, *(int *)HeapPop(heap), __LINE__);
    status += CompereInt(8, *(int *)HeapPeek(heap), __LINE__);
    status += CompereInt(3, HeapSize(heap), __LINE__);
    Print(heap);

    status += CompereInt(8, *(int *)HeapPop(heap), __LINE__);
    status += CompereInt(10, *(int *)HeapPeek(heap), __LINE__);
    status += CompereInt(2, HeapSize(heap), __LINE__);
    Print(heap);

    status += CompereInt(10, *(int *)HeapPop(heap), __LINE__);
    status += CompereInt(50, *(int *)HeapPeek(heap), __LINE__);
    status += CompereInt(1, HeapSize(heap), __LINE__);
    Print(heap);

    status += CompereInt(50, *(int *)HeapPop(heap), __LINE__);
    status += CompereInt(0, HeapSize(heap), __LINE__);
    Print(heap);

    HeapDestroy(heap);
    return status;
}

int ReplaceTest()
{
    int status = 0;
    int param = 2;
    int a = 4;
    int b = 5;
    int c = 8;
    int d = 7;
    int e = 3;
    int f = 50;
    int g = 10;
    int h = 0;
    int i = 2;

    heap_t *heap = HeapCreate(CmpFunc, &param);
    status += CompereInt(TRUE, NULL != heap, __LINE__);
    status += CompereInt(TRUE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(0, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &d), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(1, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &b), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(2, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &c), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(3, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &a), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(4, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &e), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(5, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &f), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(6, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &g), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(7, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &h), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(8, HeapSize(heap), __LINE__);

    Print(heap);

    Print(heap);

    HeapReplace(heap, &i);
    status += CompereInt(8, HeapSize(heap), __LINE__);

    Print(heap);

    HeapDestroy(heap);
    return status;
}

int RemoveTest()
{
    int status = 0;
    int param = 2;
    int a = 4;
    int b = 5;
    int c = 8;
    int d = 7;
    int e = 3;
    int f = 50;
    int g = 10;
    int h = 0;

    heap_t *heap = HeapCreate(CmpFunc, &param);
    status += CompereInt(TRUE, NULL != heap, __LINE__);
    status += CompereInt(TRUE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(0, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &d), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(1, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &b), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(2, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &c), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(3, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &a), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(4, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &e), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(5, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &f), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(6, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &g), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(7, HeapSize(heap), __LINE__);

    status += CompereInt(SUCCESS, HeapPush(heap, &h), __LINE__);
    status += CompereInt(FALSE, HeapIsEmpty(heap), __LINE__);
    status += CompereInt(8, HeapSize(heap), __LINE__);

    Print(heap);
    status += CompereInt(c, *(int *)HeapRemove(heap, &c, IsMatch, NULL), __LINE__);

    Print(heap);

    status += CompereInt(g, *(int *)HeapRemove(heap, &g, IsMatch, NULL), __LINE__);

    Print(heap);

    HeapDestroy(heap);
    return status;
}

int CmpFunc(const void *new_data, const void *current_data, void *cmp_param)
{
    assert(NULL != new_data);
    assert(NULL != current_data);
    (void)cmp_param;

    return (*(int *)new_data - *(int *)current_data);
}

int IsMatch(const void *data, const void *heap_data, void *param)
{
    assert(NULL != data);
    assert(NULL != heap_data);
    (void)param;
    return (*(int *)data == *(int *)heap_data);
}
/*************************************************************************/

static int CompereInt(int expected_val, int recieved_val, int line)
{
    if (recieved_val != expected_val)
    {
        printf("\x1b[1;33m");
        printf("----------------FAIL line %d----------------\n", line);
        printf("expected %d, recieved %d\n", expected_val, recieved_val);
        printf("\x1b[0m");
        return 1;
    }

    return 0;
}
