/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Author: Chen Rudy
- Date: 03/27/22
- Reviewed By: Amir Topaz
- pq.h
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifndef __h_PQ__
#define __h_PQ__

#include <stddef.h> /* size_t */

#include "Heap.h"

/***************************************************************************
									DS
****************************************************************************
priority queue is an abstract data-type similar to a regular queue in which each element additionally has a priority associated with it. In a priority queue, an element with high priority is served before an element with low priority.

attributes and policy:
-	order of elements is based on priority
-   Linear data structure
-   Sequential access
-	access only to top element
-   Holds elements by reference
-	Size change acorrding to insert/delete.

-------------------------------------------------------------------------------

Operations on DS:
- creates queue
- destroys queue
- Enqueue
- Dequeue
- cheak if queue is empty
- peek at first element
- size of queue
- clear queue of all elements
- erase an element from the queue by parameter


*******************************************************************************
								 Typedefs
*******************************************************************************/
typedef struct pq pq_t;
typedef int (*pq_cmp_func_t)(const void *new_data, const void *current_data, void *param);
typedef int (*pq_is_match_t)(const void *current_data, void *param);
/*******************************************************************************
								Functions
*******************************************************************************/

pq_t *PQCreate(pq_cmp_func_t cmp_func, void *param);
/******************************************************************************
PQCreate
Description: creates an empty queue
Parameters:
	- cmp_func - function by which priority is determined
	- param - void pointer to parameter for sorting
Return value:
	- pq_t queue
Complexity: O(1)
Notes:
- NULL cmp_func will cause undefined behaviour.
-cmp_func returns 0 if equal, -1 / 1 if not equal.

*****************************************************************************/
void PQDestroy(pq_t *pq);
/******************************************************************************
PQDestroy
Description: destroys given queue
Parameters:
	- pq_t queue
Return value:
	- void
Complexity: O(n)
Notes:
- NULL queue will cause undefined behaviour.

*****************************************************************************/

int PQEnqueue(pq_t *pq, void *data);
/******************************************************************************
PQEnqueue
Description: Inserts element by priority to queue.
Parameters:
	- pq_t queue
	- void pointer to data
Return value:
	- returns success (0) or failure (1)
Complexity:  O(n)
Notes:
- in case of cmp_func equality, insert will occur after the equal node/s.
- NULL queue will cause undefined behaviour.
- NULL data will cause undefined behaviour.
- priority is determined via cmp_func, PQEnqueue inserts while cmp_func returns > 0.
- if 2 elements has same priority, FIFO will occur.
*****************************************************************************/

void *PQDequeue(pq_t *pq);
/******************************************************************************
PQDequeue
Description: Removes element with highest priority
Parameters:
	- pq_t queue
Return value:
	- void pointer to data of removed element
Complexity: O(1)
Notes:
- NULL queue will cause undefined behaviour.

*****************************************************************************/

int PQIsEmpty(const pq_t *pq);
/******************************************************************************
PQIsEmpty
Description: Cheaks if queue is empty
Parameters:
	- pq_t queue
Return value:
	- returns true (1) if queue is empty
	- returns false (0) if queue is not empty
Complexity: O(1)
Notes:
- NULL queue will cause undefined behaviour.
*****************************************************************************/

void *PQPeek(const pq_t *pq);
/******************************************************************************
PQPeek
Description: peeks at top element
Parameters:
	- pq_t queue
Return value:
	- void pointer to data of top element
Complexity: O(1)
Notes:
- NULL queue will cause undefined behaviour.
*****************************************************************************/
size_t PQSize(const pq_t *pq);
/******************************************************************************
PQSize
Description: Counts elements in queue
Parameters:
	- pq_t queue
Return value:
	- size_t number of elements in given queue
Complexity: O(n)
Notes:
- NULL queue will cause undefined behaviour.
*****************************************************************************/
void PQClear(pq_t *pq);

/******************************************************************************
PQClear
Description: cleans queue of all elements
Parameters:
	- pq_t queue
Return value:
	- pq_t empty queue
Complexity: O(n)
Notes:
- NULL queue will cause undefined behaviour.
*****************************************************************************/

void *PQErase(pq_t *pq, void *param, pq_is_match_t is_match);

/******************************************************************************
PQErase
Description: removes one element by given parameter
Parameters:
	- pq_t queue
	- void pointer parameter
	- pq_is_match_t function
Return value:
	- returns void pointer to data of removed element
	- returns NULL if element isnt found
Complexity: O(n)
Notes:
- NULL queue will cause undefined behaviour.
- if void pointer parameter is NULL, behaviour is undefined.
- if pointer to function is NULL, behaviour is undefined.
- is_match; returns 1 for true, 0 for false.
therefore, PQErase erases when is_match returns 1.

*****************************************************************************/

#endif /* __PQ__ */
