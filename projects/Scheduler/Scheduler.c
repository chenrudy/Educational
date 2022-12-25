/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- Author: Chen Rudy
- Date: Mar 31
- Reviewed By: Noam
- sched.c
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <assert.h> /* for assert */
#include <stdlib.h> /* for malloc and free */

#include "Scheduler.h" /*scheduler header*/
#include "PQ_heap.h"   /*priority queue creator header*/
#include "Heap.h"	   /*heap creator header*/
#include "Uid.h"	   /* unique identifier creator header*/
#include "Task.h"	   /*task creator header*/

struct sched
{
	pq_t *pq;
	int flag;
	task_t *current_task;
};

enum value
{
	EXECUTE_FAILURE = -1,
	SUCCESS = 0,
	FAILURE = 1
};

enum flag
{
	STOP = 1,
	RUN = 0
};

int SchedMatchUID(const void *current_task, void *uid2);
static int SchedReschedule(sched_t *sched, task_t *task, time_t add_time);
static void SchedSleepyTime(time_t to_sleep);
int SchedPQCmp(const void *new_task, const void *current_task, void *param);
static int HandleStatus(sched_t *sched, int status, time_t return_value);

sched_t *SchedCreate(void)
{
	sched_t *scheduler = (sched_t *)malloc(sizeof(sched_t));
	if (NULL == scheduler)
	{
		return NULL;
	}

	scheduler->pq = PQCreate(SchedPQCmp, NULL);
	if (NULL == scheduler->pq)
	{
		free(scheduler);
		scheduler = NULL;
		return NULL;
	}

	scheduler->flag = RUN;
	scheduler->current_task = NULL;

	return scheduler;
}

int SchedPQCmp(const void *new_task, const void *current_task, void *param)
{
	time_t current_time = TaskGetTime(current_task);
	time_t new_time = TaskGetTime(new_task);
	(void)param;
	return new_time - current_time;
}

void SchedDestroy(sched_t *sched)
{
	assert(NULL != sched);

	SchedClear(sched);
	PQDestroy(sched->pq);
	free(sched);
	sched = NULL;
}

void SchedClear(sched_t *sched)
{
	task_t *task = NULL;

	assert(NULL != sched);

	while (!PQIsEmpty(sched->pq))
	{
		task = (task_t *)PQDequeue(sched->pq);
		TaskDestroy(task);
		task = NULL;
	}
}

ilrd_uid_t SchedAddTask(sched_t *sched,
						sc_action_func_t action_func, void *action_param,
						sc_clean_up_func_t clean_up_func, void *clean_up_param, time_t time)
{
	task_t *task = NULL;

	assert(NULL != sched);
	assert(NULL != action_func);
	assert(NULL != clean_up_func);
	assert(FAILURE != time);

	task = TaskCreate(action_func, action_param,
					  clean_up_func, clean_up_param, time);
	if (NULL == task)
	{
		return UIDBadUID;
	}

	if (FAILURE == PQEnqueue(sched->pq, task))
	{
		TaskDestroy(task);
		task = NULL;
		return UIDBadUID;
	}

	return TaskGetUID(task);
}

void SchedRemoveTask(sched_t *sched, ilrd_uid_t uid)
{
	task_t *task = NULL;
	assert(NULL != sched);
	assert(!UIDIsSame(uid, UIDBadUID));

	task = PQErase(sched->pq, &uid, SchedMatchUID);
	if (NULL != task)
	{
		TaskDestroy(task);
		task = NULL;
	}

	else if (sched->current_task != NULL && UIDIsSame(TaskGetUID(sched->current_task), uid))
	{
		TaskDestroy(sched->current_task);
		sched->current_task = NULL;
	}
}

int SchedMatchUID(const void *current_task, void *uid2)
{
	ilrd_uid_t curr_uid = TaskGetUID(current_task);
	ilrd_uid_t to_remove = *(ilrd_uid_t *)uid2;

	return UIDIsSame(curr_uid, to_remove);
}

int SchedRun(sched_t *sched)
{
	time_t return_value = 0;
	int status = SUCCESS;
	assert(NULL != sched);

	sched->flag = RUN;

	while (!SchedIsEmpty(sched) && RUN == sched->flag)
	{
		sched->current_task = (task_t *)PQDequeue(sched->pq);

		SchedSleepyTime(TaskGetTime(sched->current_task) - time(0));

		return_value = TaskExecute(sched->current_task);

		status = HandleStatus(sched, status, return_value);
	}

	return status;
}

static int HandleStatus(sched_t *sched, int status, time_t return_value)
{
	if (sched->current_task == NULL)
	{
		return 0;
	}
	if (SUCCESS < return_value)
	{
		SchedReschedule(sched, sched->current_task, return_value - time(0));
		sched->current_task = NULL;
		return 0;
	}

	TaskDestroy(sched->current_task);
	sched->current_task = NULL;

	if (EXECUTE_FAILURE == return_value)
	{
		status = FAILURE;
	}
	return status;
}

static void SchedSleepyTime(time_t to_sleep)
{
	if (0 < to_sleep)
	{
		while (sleep(to_sleep))
			;
	}
}

static int SchedReschedule(sched_t *sched, task_t *task, time_t add_time)
{
	TaskSetTime(task, add_time);

	return PQEnqueue(sched->pq, task);
}

void SchedStop(sched_t *sched)
{
	assert(NULL != sched);

	sched->flag = STOP;
}

size_t SchedSize(const sched_t *sched)
{
	size_t size = 0;

	assert(NULL != sched);

	size = PQSize(sched->pq);

	if (NULL != sched->current_task)
	{
		++size;
	}

	return size;
}

int SchedIsEmpty(const sched_t *sched)
{
	assert(NULL != sched);

	if (NULL != sched->current_task)
	{
		return 0;
	}

	return PQIsEmpty(sched->pq);
}
