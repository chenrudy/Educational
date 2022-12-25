/*****************************************************************************
 *Author: Chen Rudy														 *
 *Reviewer:  														 *
 *Date: 16/06/2022															 *
 *semaphore.h																		 *
 ******************************************************************************/

/*******************************************************************************

********************************************************************************
Description:
Attributes and policy:
	- FIFO
	...

-------------------------------------------------------------------------------

Operations on DS:
	- Create
	- Destroy
	...

*******************************************************************************/
#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <stddef.h> /* for size_t */

/*******************************************************************************
									Typedefs
*******************************************************************************/
typedef int semid_t;

/******************************************************************************
Foo
Description: Creates ...
Parameters:
	- None
	...
Return value:
	- None
Time complexity: O(1).
Note:
*****************************************************************************/
semid_t SemCreate(const char *name, int proj_id, size_t sem_value);
int SemDestroy(semid_t id);
int SemIncrease(semid_t id, int num, int set_undo);
int SemDecrease(semid_t id, int num, int set_undo);
int SemGetVal(semid_t id);

#endif /* __SEMAPHORE_H__ */
