/*****************************************************************************
 *Name: Chen Rudy												 			 *
 *Reviewar: Eyad							 			 *
 *Date: 03/28/22													 			 *
 *task_test.c													 			 *
 ******************************************************************************/

#include <stdio.h> /* for printf */
#include "Task.h"

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

int Test();
int action_func(void *action_param);
void clean_up_func(void *clean_up_param);

int main()
{
	RUNTEST(Test());
	return 0;
}

int Test()
{
	int action_param = 3;
	int clean_up_param = 5;
	task_t *task = TaskCreate(action_func, &action_param, clean_up_func, &clean_up_param, time(0));
	TaskExecute(task);
	printf("time is: %lu\n", TaskGetTime(task));
	TaskSetTime(task, 15);
	printf("time is: %lu\n", TaskGetTime(task));
	printf("PID: %u\n", TaskGetUID(task).pid);
	printf("counter: %lu\n", TaskGetUID(task).counter);
	TaskDestroy(task);

	return 0;
}

int action_func(void *action_param)
{
	*(int *)action_param *= 2;
	printf("%d\n", *(int *)action_param);
	return 0;
}

void clean_up_func(void *clean_up_param)
{
	printf("cleaned up! %d\n", *(int *)clean_up_param);
}
