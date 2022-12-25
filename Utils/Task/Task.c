/*****************************************************************************
 *Name: Chen Rudy											 			 *
 *Reviewar: Eyad								 			 *
 *Date: 03/28/22													 			 *
 *task.c														 			 *
 ******************************************************************************/

#include <assert.h> /* for assert */
#include <stdlib.h> /*for malloc and free*/

#include "Task.h"

struct task
{
	time_t time;
	task_action_func_t action_func;
	void *action_param;
	task_clean_up_func_t clean_up_func;
	void *clean_up_param;
	ilrd_uid_t uid;
};

static task_t *TaskInit(task_t *task, task_action_func_t action_func,
						void *action_param, task_clean_up_func_t clean_up_func, void *clean_up_param,
						time_t time);

task_t *TaskCreate(task_action_func_t action_func, void *action_param,
				   task_clean_up_func_t clean_up_func, void *clean_up_param, time_t time)
{
	task_t *task = NULL;

	assert(NULL != action_func);
	assert(NULL != clean_up_func);
	assert(-1 != time);

	task = (task_t *)malloc(sizeof(task_t));
	if (NULL == task)
	{
		return NULL;
	}

	return TaskInit(task, action_func, action_param,
					clean_up_func, clean_up_param, time);
}

static task_t *TaskInit(task_t *task, task_action_func_t action_func,
						void *action_param, task_clean_up_func_t clean_up_func, void *clean_up_param,
						time_t time)
{
	task->uid = UIDCreate();

	if (UIDIsSame(UIDBadUID, task->uid))
	{
		free(task);
		task = NULL;

		return NULL;
	}

	task->time = time;
	task->action_func = action_func;
	task->action_param = action_param;
	task->clean_up_func = clean_up_func;
	task->clean_up_param = clean_up_param;
	return task;
}

void TaskDestroy(task_t *task)
{
	assert(NULL != task);
	assert(NULL != task->clean_up_func);

	task->clean_up_func(task->clean_up_param);
	free(task);
	task = NULL;
}

int TaskExecute(task_t *task)
{
	assert(NULL != task);
	assert(NULL != task->action_func);

	return task->action_func(task->action_param);
}

time_t TaskGetTime(const task_t *task)
{
	assert(NULL != task);

	return task->time;
}

ilrd_uid_t TaskGetUID(const task_t *task)
{
	assert(NULL != task);

	return task->uid;
}

void TaskSetTime(task_t *task, time_t add_time)
{
	assert(NULL != task);

	task->time += add_time;
}
