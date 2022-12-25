/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Author: Chen Rudy
- Date: 03/27/22
- Reviewed By: Amir Topaz
- pq_test.c
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <stdio.h> /* for printf */

#include "PQ_heap.h"

enum
{
	BIGGER = -1,
	EQUAL = 0,
	SMALLER = 1
};

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
			printf("\x1b[1;35m");         \
			printf("\nTest Failed!\n\n"); \
			printf("\x1b[0m");            \
		}                                 \
	}

int CmpFunc(const void *new_data, const void *list_data,
			void *param);
int Compare(const void *data, void *parameter);
int SumInt(void *data, void *parameter);
int CompereInt(int expected_val, int recieved_val, int line);
size_t CompereSt(size_t expected_val, size_t recieved_val, int line);
int IsMatch(const void *current_data, void *param);

int CreatDestroyTest();
int EnqueueTest();
int PQDequeueTest();
int ClearTest();
int EraseTest();

int main()
{
	RUNTEST(CreatDestroyTest());
	RUNTEST(EnqueueTest());
	RUNTEST(PQDequeueTest());
	RUNTEST(ClearTest());
	RUNTEST(EraseTest());

	return 0;
}

int CreatDestroyTest()
{
	int param = 2;
	pq_t *queue = PQCreate(CmpFunc, &param);
	PQDestroy(queue);
	return 0;
}

int EnqueueTest()
{
	int param = 2;
	int status = 0;
	int a = 5;
	int b = 8;
	pq_t *queue = PQCreate(CmpFunc, &param);

	status += CompereInt(0, PQEnqueue(queue, &a), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &b), __LINE__);

	PQDestroy(queue);
	return status;
}

int PQDequeueTest()
{
	int param = 2;
	int status = 0;
	int a = 5;
	int b = 8;
	int c = 1;
	int d = 7;
	int e = -1;
	pq_t *queue = PQCreate(CmpFunc, &param);

	status += CompereInt(1, PQIsEmpty(queue), __LINE__);
	status += CompereSt(0, PQSize(queue), __LINE__);

	status += CompereInt(0, PQEnqueue(queue, &a), __LINE__);
	status += CompereSt(1, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &b), __LINE__);
	status += CompereSt(2, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &c), __LINE__);
	status += CompereSt(3, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &d), __LINE__);
	status += CompereSt(4, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &e), __LINE__);
	status += CompereSt(5, PQSize(queue), __LINE__);
	status += CompereInt(0, PQIsEmpty(queue), __LINE__);

	status += CompereInt(-1, *(int *)PQPeek(queue), __LINE__);

	status += CompereInt(-1, *(int *)PQDequeue(queue), __LINE__);
	status += CompereSt(4, PQSize(queue), __LINE__);
	status += CompereInt(1, *(int *)PQDequeue(queue), __LINE__);
	status += CompereSt(3, PQSize(queue), __LINE__);
	status += CompereInt(5, *(int *)PQDequeue(queue), __LINE__);
	status += CompereSt(2, PQSize(queue), __LINE__);
	status += CompereInt(7, *(int *)PQDequeue(queue), __LINE__);
	status += CompereSt(1, PQSize(queue), __LINE__);
	status += CompereInt(0, PQIsEmpty(queue), __LINE__);
	status += CompereInt(8, *(int *)PQDequeue(queue), __LINE__);
	status += CompereInt(1, PQIsEmpty(queue), __LINE__);

	PQDestroy(queue);
	return status;
}

int ClearTest()
{
	int param = 2;
	int status = 0;
	int a = 5;
	int b = 8;
	int c = 1;
	int d = 7;
	int e = -1;
	pq_t *queue = PQCreate(CmpFunc, &param);

	status += CompereInt(1, PQIsEmpty(queue), __LINE__);
	status += CompereSt(0, PQSize(queue), __LINE__);

	status += CompereInt(0, PQEnqueue(queue, &a), __LINE__);
	status += CompereSt(1, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &b), __LINE__);
	status += CompereSt(2, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &c), __LINE__);
	status += CompereSt(3, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &d), __LINE__);
	status += CompereSt(4, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &e), __LINE__);
	status += CompereSt(5, PQSize(queue), __LINE__);
	status += CompereInt(0, PQIsEmpty(queue), __LINE__);

	status += CompereInt(-1, *(int *)PQPeek(queue), __LINE__);

	PQClear(queue);
	status += CompereInt(1, PQIsEmpty(queue), __LINE__);
	status += CompereSt(0, PQSize(queue), __LINE__);

	status += CompereInt(0, PQEnqueue(queue, &a), __LINE__);
	status += CompereSt(1, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &b), __LINE__);
	status += CompereSt(2, PQSize(queue), __LINE__);

	PQClear(queue);
	status += CompereInt(1, PQIsEmpty(queue), __LINE__);
	status += CompereSt(0, PQSize(queue), __LINE__);

	PQDestroy(queue);
	return status;
}

int EraseTest()
{
	int param = 2;
	int status = 0;
	int a = 5;
	int b = 8;
	int c = 1;
	int d = 7;
	int e = -1;
	pq_t *queue = PQCreate(CmpFunc, &param);

	status += CompereInt(1, PQIsEmpty(queue), __LINE__);
	status += CompereSt(0, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &a), __LINE__);
	status += CompereSt(1, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &b), __LINE__);
	status += CompereSt(2, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &c), __LINE__);
	status += CompereSt(3, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &d), __LINE__);
	status += CompereSt(4, PQSize(queue), __LINE__);
	status += CompereInt(0, PQEnqueue(queue, &e), __LINE__);
	status += CompereSt(5, PQSize(queue), __LINE__);
	status += CompereInt(0, PQIsEmpty(queue), __LINE__);

	status += CompereInt(a, *(int *)PQErase(queue, &a, IsMatch), __LINE__);

	status += CompereSt(4, PQSize(queue), __LINE__);

	status += CompereInt(d, *(int *)PQErase(queue, &d, IsMatch), __LINE__);

	status += CompereSt(3, PQSize(queue), __LINE__);

	PQDestroy(queue);
	return status;
}

/*******************************help funcs***************************************/
int Compare(const void *data, void *parameter)
{
	return (*(int *)data == *(int *)parameter);
}

int SumInt(void *data, void *parameter)
{
	*(int *)data += *(int *)parameter;
	return 0;
}

int CmpFunc(const void *new_data, const void *list_data,
			void *param) /*my cmp_func*/
{
	(void)param;
	return *(int *)new_data - *(int *)list_data;
}

int IsMatch(const void *current_data, void *param)
{
	return *(int *)current_data == *(int *)param;
}

int CompereInt(int expected_val, int recieved_val, int line)
{
	if (recieved_val != expected_val)
	{
		printf("----------------FAIL line %d----------------\n", line);
		printf("expected %d, recieved %d\n", expected_val, recieved_val);
		return 1;
	}

	return 0;
}

size_t CompereSt(size_t expected_val, size_t recieved_val, int line)
{
	if (recieved_val != expected_val)
	{
		printf("----------------FAIL line %d----------------\n", line);
		printf("expected %lu, recieved %lu\n", expected_val, recieved_val);
		return 1;
	}
	return 0;
}
