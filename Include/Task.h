/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Author: Chen Rudy
- Date: 03/28/22
- Reviewed By: Eyad
- task.h
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __ILRD_TASK_H__
#define __ILRD_TASK_H__

#include <time.h> /* time_t */
#include "Uid.h"  /* ilrd_uid_t */
/*******************************************************************************
								 Typedefs
*******************************************************************************/
typedef struct task task_t;
typedef int (*task_action_func_t)(void *action_param);
typedef void (*task_clean_up_func_t)(void *clean_up_param);
/*******************************************************************************
								Functions
*******************************************************************************/
task_t *TaskCreate(task_action_func_t action_func, void *action_param,
				   task_clean_up_func_t clean_up_func, void *clean_up_param, time_t time);
/******************************************************************************
TaskCreate
Description: creats task
Parameters:
	- action_func
		- recives void pointer parameter
		- returns 0 for sucess or -1 for failure and a positive number to reschedule the task again, the number represents time.
	- action_param: void pointer parameter for action_func
	- clean_up_func
		- recives void pointer parameter
		- void return value
	- clean_up_param: void pointer parameter for clean_up_func
	- time_t time
Return value:
	- for success returns task_t pointer to created Task
	- for failure returns NULL
Complexity: O(1)
Notes:
- NULL action_func will cause undefined behaviour.
- NULL clean_up_func will cause undefined behaviour.
- If time is -1, undefined behavior occurs.
*****************************************************************************/

void TaskDestroy(task_t *task);
/******************************************************************************
TaskDestroy
Description: destroys give task
Parameters:
	- task_t pointer to task
Return value:
	- void
Complexity: undefined. dependes on clean_up_func Complexity
Notes:
- NULL task will cause undefined behaviour.
*****************************************************************************/

int TaskExecute(task_t *task);
/******************************************************************************
TaskExecute
Description: executes tasks action func
Parameters:
	- task_t pointer to task
Return value:
	- 0 for sucess or 1 for failure
Complexity: undefined. dependes on action_func Complexity
Notes:
- NULL task will cause undefined behaviour.
*****************************************************************************/

time_t TaskGetTime(const task_t *task);
/******************************************************************************
TaskGetTime
Description: get time of given task
Parameters:
	- task_t pointer to task
Return value:
	- time_t time
Complexity: O(1)
Notes:
- NULL task will cause undefined behaviour.
*****************************************************************************/

ilrd_uid_t TaskGetUID(const task_t *task);
/******************************************************************************
TaskGetUID
Description: get uid of given task
Parameters:
	- task_t pointer to task
Return value:
	- ilrd_uid_t unique identifier
Complexity: O(1)
Notes:
- NULL task will cause undefined behaviour.
*****************************************************************************/

void TaskSetTime(task_t *task, time_t add_time);
/******************************************************************************
TaskSetTime
Description: added time to task time
Parameters:
	- task_t pointer to task
	- time_t time to add
Return value:
	- void
Complexity: O(1)
Notes:
- NULL task will cause undefined behaviour.
- if added time is negetive and has passed current time, behaviour is undefined
*****************************************************************************/

#endif /* __ILRD_TASK_H__ */
