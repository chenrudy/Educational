#include <string.h> /*memmove*/
#include <stdlib.h> /*malloc realloc*/
#include <stdio.h>	/*printf*/
#include <assert.h> /*assert*/

#include "Dynamic_vector.h"

#define DEFAULT 3
#define GROWTH_FACTOR << 1

struct vector
{
	void *data;
	size_t size;
	size_t element_size;
	size_t capacity;
};

static int ResizeDVector(d_vector_t *vector, size_t new_num_of_elements);

d_vector_t *DVectorCreate(size_t element_size, size_t num_of_elements)
{
	d_vector_t *vector = (d_vector_t *)malloc(sizeof(d_vector_t));
	if (NULL == vector)
	{
		return NULL;
	}

	num_of_elements = (0 == num_of_elements ? DEFAULT : num_of_elements);

	vector->data = malloc(num_of_elements * element_size);
	if (NULL == vector->data)
	{
		free(vector);
		return NULL;
	}

	vector->size = 0;
	vector->element_size = element_size;
	vector->capacity = num_of_elements * element_size;

	return vector;
}

void DVectorDestroy(d_vector_t *vector)
{
	assert(NULL != vector);
	free(vector->data);
	vector->data = NULL;
	free(vector);
	vector = NULL;
}

size_t DVectorSize(const d_vector_t *vector)
{
	return vector->size;
}

size_t DVectorCapacity(const d_vector_t *vector)
{
	assert(NULL != vector);
	return vector->capacity;
}

int DVectorPushBack(d_vector_t *vector, const void *data)
{
	size_t position = vector->element_size * vector->size;
	char *pVector = (char *)vector->data;

	assert(NULL != vector);

	memcpy(pVector + position, data, vector->element_size);
	++vector->size;

	if ((vector->size * vector->element_size) == vector->capacity)
	{
		return ResizeDVector(vector, (vector->capacity << 1));
	}
	return 0;
}

int DVectorPopBack(d_vector_t *vector)
{
	assert(NULL != vector);
	--vector->size;
	if ((vector->size * vector->element_size) == (vector->capacity >> 2))
	{
		return ResizeDVector(vector, vector->capacity >> 1);
	}
	return 0;
}

int DVectorReserve(d_vector_t *vector, size_t new_num_of_elements)
{
	size_t byets_to_allocate = new_num_of_elements * vector->element_size;

	assert(NULL != vector);

	if (byets_to_allocate > vector->capacity)
	{
		return ResizeDVector(vector, byets_to_allocate);
	}
	return DVectorShrinkToFit(vector);
}

int DVectorShrinkToFit(d_vector_t *vector)
{
	size_t shrinked_size = vector->size * vector->element_size + vector->element_size;

	assert(NULL != vector);

	return ResizeDVector(vector, shrinked_size);
}

static int ResizeDVector(d_vector_t *vector, size_t new_num_of_elements)
{
	void *backup = vector->data;
	assert(NULL != vector);
	assert(NULL != backup);

	vector->data = realloc(vector->data, new_num_of_elements);
	if (NULL == vector->data)
	{
		vector->data = backup;
		return 1;
	}
	vector->capacity = new_num_of_elements;
	return 0;
}

int DVectorIsEmpty(const d_vector_t *vector)
{
	assert(NULL != vector);
	return 0 == vector->size;
}

void *DVectorGetElement(const d_vector_t *vector, size_t index)
{
	size_t offset = index * vector->element_size;
	char *data = (char *)vector->data;
	assert(NULL != vector);

	return data + offset;
}
