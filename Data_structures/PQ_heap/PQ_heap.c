/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Author: Chen Rudy
- Date: 03/27/22
- Reviewed By: Amir Topaz
- pq.c
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <assert.h> /* for assert */
#include <stdlib.h> /*for malloc and free*/

#include "PQ_heap.h"

enum return_val
{
	SUCCESS = 0
};

struct pq
{
	heap_t *queue;
};

typedef struct param_ismatch
{
	pq_is_match_t *is_match;
	void *param;
} param_ismatch_t;

static int IsMatchRapper(const void *data, const void *heap_data, void *param_ismatch);

static int IsMatch(const void *current_data, void *param);

pq_t *PQCreate(pq_cmp_func_t cmp_func, void *param)
{
	pq_t *pq = NULL;

	assert(NULL != cmp_func);

	pq = (pq_t *)malloc(sizeof(pq_t));

	if (NULL == pq)
	{
		return NULL;
	}

	pq->queue = HeapCreate(cmp_func, param);

	if (NULL == pq->queue)
	{
		free(pq);
		pq = NULL;
		return NULL;
	}

	return pq;
}

void PQDestroy(pq_t *pq)
{
	assert(NULL != pq);

	HeapDestroy(pq->queue);
	pq->queue = NULL;
	free(pq);
	pq = NULL;
}

int PQEnqueue(pq_t *pq, void *data)
{
	int status = 0;

	assert(NULL != pq);
	assert(NULL != data);

	status = HeapPush(pq->queue, data);
	if (SUCCESS != status)
	{
		PQErase(pq, data, IsMatch);
	}

	return status;
}

void *PQDequeue(pq_t *pq)
{
	assert(NULL != pq);

	return HeapPop(pq->queue);
}

int PQIsEmpty(const pq_t *pq)
{
	assert(NULL != pq);

	return HeapIsEmpty(pq->queue);
}

void *PQPeek(const pq_t *pq)
{
	assert(NULL != pq);

	return HeapPeek(pq->queue);
}

size_t PQSize(const pq_t *pq)
{
	assert(NULL != pq);

	return HeapSize(pq->queue);
}

void PQClear(pq_t *pq)
{
	assert(NULL != pq);

	while (!PQIsEmpty(pq))
	{
		PQDequeue(pq);
	}
}

void *PQErase(pq_t *pq, void *param, pq_is_match_t is_match)
{
	param_ismatch_t param_ismatch;

	assert(NULL != pq);
	assert(NULL != is_match);

	param_ismatch.is_match = &is_match;
	param_ismatch.param = param;

	return HeapRemove(pq->queue, NULL, &IsMatchRapper, &param_ismatch);
}

static int IsMatchRapper(const void *data, const void *heap_data, void *param_ismatch)
{
	param_ismatch_t param_ismatch_stract = *(param_ismatch_t *)param_ismatch;
	pq_is_match_t *is_match = param_ismatch_stract.is_match;
	void *param = param_ismatch_stract.param;

	(void)data;
	return (*is_match)(heap_data, param);
}

int IsMatch(const void *current_data, void *param)
{
	return current_data == param;
}