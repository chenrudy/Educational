/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- Author: Chen Rudy
- Date: Mar 31
- Reviewed By: Noam
- sched.h
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
***************************************************************************
								Scheduler
****************************************************************************
Scheduler is an entity whose purpose is to schedule via assigning resources to perform tasks.

attributes and policy:
-	Sequential accsess
-   Holds elements by reference
- 	Sorted according to time priority
- 	FIFO within elements of the same priority
-	After initiationg Run, scheduler gains control.
-	In order regain control and stop scheduler, one should insert a stop task in scheduler in advance.

-------------------------------------------------------------------------------

Operations on DS:
- creates
- destroys
- AddTask
- RemoveTask
- Run
- Stop
- Clear
- Size
- IsEmpty

*******************************************************************************/

#ifndef __SCHED__
#define __SCHED__

#include <time.h> /* size_t */
#include "Uid.h"  /* unique identifier creator header*/

/*******************************************************************************
								 Typedefs
*******************************************************************************/
typedef struct sched sched_t;
typedef int (*sc_action_func_t)(void *action_param);
typedef void (*sc_clean_up_func_t)(void *clean_up_param);

/*******************************************************************************
								Functions
*******************************************************************************/

sched_t *SchedCreate(void);
/******************************************************************************
SchedCreate
Description: creates a scheduler
Parameters:
	- void
Return value:
	- in case of success, will return new sched_t scheduler
	- in case of failure, will return
Complexity: O(1)
Notes:

*****************************************************************************/

void SchedDestroy(sched_t *sched);
/******************************************************************************
SchedDestroy
Description: Destroys given Scheduler.
Parameters:
	- sched_t pointer to a Scheduler.
Return value:
	- void
Complexity: O(n)
Notes:
- If Scheduler is NULL, undefined behavior occurs.
*****************************************************************************/

ilrd_uid_t SchedAddTask(sched_t *sched,
						sc_action_func_t action_func, void *action_param,
						sc_clean_up_func_t clean_up_func, void *clean_up_param, time_t time);
/******************************************************************************
SchedAddTask
Description: Creats and add new task to Scheduler
Parameters:
	- sched_t pointer to a Scheduler.
	- action_func
		- recives void pointer parameter
		- returns 0 for sucess , -1 for failure and a positive number to
		  reschedule the task again, the number represents time.
	- action_param: void pointer parameter for action_func
	- clean_up_func
		- recives void pointer parameter
		- void return value
	- clean_up_param: void pointer parameter for clean_up_func
	- time_t absolute time.
Return value:
	- for success returns ilrd_uid_t unique identifier of added task
	- for failure returns UIDBadUID
Complexity: O(1)
Notes:
- If Scheduler is NULL, undefined behavior occurs.
- If action_func is NULL, undefined behavior occurs.
- If clean_up_func is NULL, undefined behavior occurs.
- If time is -1, undefined behavior occurs.
- given time is absolute

*****************************************************************************/

void SchedRemoveTask(sched_t *sched, ilrd_uid_t uid);
/******************************************************************************
SchedRemoveTask
Description: Removes given task from the Scheduler.
Parameters:
	- sched_t pointer to a Scheduler.
	- ilrd_uid_t unique identifier of added task
Return value:
	- void
Complexity: O(n)
Notes:
- If Scheduler is NULL, undefined behavior occurs.
- If uid is UIDBadUID , undefined behavior occurs.

*****************************************************************************/

int SchedRun(sched_t *sched);
/******************************************************************************
SchedRun
Description: Runs given Scheduler
Parameters:
	- sched_t pointer to a Scheduler.
Return value:
	- In case of success in all tasks, returns 0.
	- In case of failure in at least 1 task , returns 1.
Complexity: undefined
Notes:
- If Scheduler is NULL, undefined behavior occurs.
- After initiationg Run, scheduler gains control.
- In order regain control and stop scheduler, one should insert a stop task in scheduler.
- keeps running until done even if tasks failed
*******************************************************************************/

void SchedStop(sched_t *sched);
/******************************************************************************
SchedStop
Description: Stops the Scheduler Run.
Parameters:
	- sched_t pointer to a Scheduler.
Return value:
	- void
Complexity: O(1)
Notes:
- If Scheduler is NULL, undefined behavior occurs.
*****************************************************************************/

void SchedClear(sched_t *sched);
/******************************************************************************
SchedClear
Description: clear all tasks from given Scheduler
Parameters:
	- sched_t pointer to a Scheduler.
Return value:
	- void
Complexity: O(n)
Notes:
- If Scheduler is NULL, undefined behavior occurs.
*****************************************************************************/

size_t SchedSize(const sched_t *sched);
/******************************************************************************
SchedSize
Description: Counts Number of Tasks in given Scheduler.
Parameters:
	- sched_t pointer to a Scheduler.
Return value:
	- size_t Number of Tasks
Complexity: O(n)
Notes:
- If Scheduler is NULL, undefined behavior occurs.
*****************************************************************************/

int SchedIsEmpty(const sched_t *sched);
/******************************************************************************
SchedIsEmpty
Description: Check whether given Scheduler is empty.
Parameters:
	- sched_t pointer to a Scheduler.
Return value:
	- true (1) in case Scheduler is empty
	- false (0)  in case Scheduler is not empty
Complexity: O(1)
Notes:
- If Scheduler is NULL, undefined behavior occurs.
*****************************************************************************/

#endif /* __SCHED_H__ */
