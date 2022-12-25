/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- Author: Chen Rudy
- Date: Mar 31
- Reviewed By: Noam
- sched_test.c
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <stdio.h>	   /* for printf */
#include "Scheduler.h" /*scheduler header*/
#include "PQ_heap.h"   /*priority queue creator header*/
#include "Heap.h"	   /*heap creator header*/
#include "Uid.h"	   /* unique identifier creator header*/
#include "Task.h"	   /*task creator header*/

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

typedef struct task_remover
{
	sched_t *sched;
	ilrd_uid_t uid;
} task_remover;

typedef struct task_remove_itself
{
	sched_t *sched;
	ilrd_uid_t uid;
} task_remove_itself;

/*normal action function*/
int ActionFunc(void *action_param);
/*loop action function*/
int LoopActionFunc(void *action_param);
/*clean up function*/
void CleanUpFunc(void *clean_up_param);
/*stop functions*/
int StopActionFunc(void *sched);
int StopIfActionFunc(void *sched);
/*task adding task*/
int AddActionFunc(void *sched);
/*task Removing task*/
int RemoveActionFunc(void *to_remove);
/*task Removing itself*/
int RemoveItselfActionFunc(void *to_remove);
/*task Counting itself*/
int CountIsEmptyActionFunc(void *sched);

/*for tests*/
int CompereInt(int expected_val, int recieved_val, int line);
size_t CompereSt(size_t expected_val, size_t recieved_val, int line);

/*tests*/
int CreateDestroyTest();
int EmptyGetsizeTest();
int AddTest();
int RemoveTest();
int RunTest();
int RunUntilFileTest();
int TaskAddTaskTest();
int TaskRemoveTaskTest();
int TaskCountIsEmptyItselfTest();

int main()
{
	RUNTEST(CreateDestroyTest());
	RUNTEST(EmptyGetsizeTest());
	RUNTEST(AddTest());
	RUNTEST(RemoveTest());
	RUNTEST(RunTest());
	RUNTEST(RunUntilFileTest());
	RUNTEST(TaskAddTaskTest());
	RUNTEST(TaskRemoveTaskTest());
	RUNTEST(TaskCountIsEmptyItselfTest());

	return 0;
}

int CreateDestroyTest()
{
	int status = 0;

	sched_t *s = SchedCreate();
	status += (NULL == s);
	SchedDestroy(s);
	return status;
}

int EmptyGetsizeTest()
{
	int status = 0;

	sched_t *s = SchedCreate();
	status += (NULL == s);

	status += CompereInt(1, SchedIsEmpty(s), __LINE__);
	status += CompereSt(0, SchedSize(s), __LINE__);

	SchedDestroy(s);
	return status;
}

int AddTest()
{
	int status = 0;
	ilrd_uid_t uid1 = {0, 0, 0};
	ilrd_uid_t uid2 = {0, 0, 0};
	int action_param = 3;
	int clean_up_param = 5;

	sched_t *s = SchedCreate();
	status += (NULL == s);

	status += CompereInt(1, SchedIsEmpty(s), __LINE__);
	status += CompereSt(0, SchedSize(s), __LINE__);

	uid1 = SchedAddTask(s, ActionFunc, &action_param, CleanUpFunc,
						&clean_up_param, time(0));
	status += (UIDIsSame(uid1, UIDBadUID));

	status += CompereInt(0, SchedIsEmpty(s), __LINE__);
	status += CompereSt(1, SchedSize(s), __LINE__);

	uid2 = SchedAddTask(s, ActionFunc, &action_param, CleanUpFunc,
						&clean_up_param, time(0));
	status += (UIDIsSame(uid2, UIDBadUID));

	status += CompereInt(0, SchedIsEmpty(s), __LINE__);
	status += CompereSt(2, SchedSize(s), __LINE__);

	SchedDestroy(s);
	return status;
}

int RemoveTest()
{
	int status = 0;
	ilrd_uid_t uid1 = {0, 0, 0};
	ilrd_uid_t uid2 = {0, 0, 0};
	int action_param = 3;
	int action_param2 = 6;
	int clean_up_param = 1;
	int clean_up_param2 = 2;

	sched_t *s = SchedCreate();
	status += (NULL == s);

	status += CompereInt(1, SchedIsEmpty(s), __LINE__);
	status += CompereSt(0, SchedSize(s), __LINE__);

	uid1 = SchedAddTask(s, ActionFunc, &action_param,
						CleanUpFunc, &clean_up_param, time(0));
	status += (UIDIsSame(uid1, UIDBadUID));

	status += CompereInt(0, SchedIsEmpty(s), __LINE__);
	status += CompereSt(1, SchedSize(s), __LINE__);

	uid2 = SchedAddTask(s, ActionFunc, &action_param2,
						CleanUpFunc, &clean_up_param2, time(0));
	status += (UIDIsSame(uid2, UIDBadUID));

	status += CompereInt(0, SchedIsEmpty(s), __LINE__);
	status += CompereSt(2, SchedSize(s), __LINE__);

	SchedRemoveTask(s, uid2);
	status += CompereInt(0, SchedIsEmpty(s), __LINE__);
	status += CompereSt(1, SchedSize(s), __LINE__);

	SchedRemoveTask(s, uid1);
	status += CompereInt(1, SchedIsEmpty(s), __LINE__);
	status += CompereSt(0, SchedSize(s), __LINE__);

	SchedDestroy(s);
	return status;
}

int RunTest()
{
	int status = 0;
	ilrd_uid_t uid1 = {0, 0, 0};
	ilrd_uid_t uid2 = {0, 0, 0};
	ilrd_uid_t uid3 = {0, 0, 0};
	ilrd_uid_t uid4 = {0, 0, 0};
	int action_param = 20;
	int action_param2 = 3;
	int action_param3 = 10;

	int clean_up_param = 1;
	int clean_up_param2 = 2;
	int clean_up_param3 = 10;
	int clean_up_param4 = 103;
	time_t t = time(0) + 4;
	time_t t2 = time(0) + 2;
	time_t t3 = time(0) + 1;
	time_t t4 = time(0) + 3;

	sched_t *s = SchedCreate();
	status += (NULL == s);

	status += CompereInt(1, SchedIsEmpty(s), __LINE__);
	status += CompereSt(0, SchedSize(s), __LINE__);

	uid1 = SchedAddTask(s, ActionFunc, &action_param,
						CleanUpFunc, &clean_up_param, t);
	status += (UIDIsSame(uid1, UIDBadUID));

	status += CompereInt(0, SchedIsEmpty(s), __LINE__);
	status += CompereSt(1, SchedSize(s), __LINE__);

	uid2 = SchedAddTask(s, LoopActionFunc, &action_param2,
						CleanUpFunc, &clean_up_param2, t2);
	status += (UIDIsSame(uid2, UIDBadUID));

	status += CompereInt(0, SchedIsEmpty(s), __LINE__);
	status += CompereSt(2, SchedSize(s), __LINE__);

	uid3 = SchedAddTask(s, LoopActionFunc, &action_param3,
						CleanUpFunc, &clean_up_param3, t3);
	status += (UIDIsSame(uid3, UIDBadUID));
	status += CompereSt(3, SchedSize(s), __LINE__);

	uid4 = SchedAddTask(s, StopActionFunc, s,
						CleanUpFunc, &clean_up_param4, t4);
	status += (UIDIsSame(uid4, UIDBadUID));
	status += CompereSt(4, SchedSize(s), __LINE__);
	printf("\n****** Running ********\n\n");

	status += CompereInt(0, SchedRun(s), __LINE__);

	SchedClear(s);
	status += CompereInt(1, SchedIsEmpty(s), __LINE__);
	status += CompereSt(0, SchedSize(s), __LINE__);

	uid1 = SchedAddTask(s, ActionFunc, &action_param,
						CleanUpFunc, &clean_up_param, t);
	status += (UIDIsSame(uid1, UIDBadUID));

	status += CompereInt(0, SchedIsEmpty(s), __LINE__);
	status += CompereSt(1, SchedSize(s), __LINE__);
	printf("\n****** Running ********\n\n");
	status += CompereInt(0, SchedRun(s), __LINE__);

	SchedDestroy(s);
	return status;
}

int RunUntilFileTest()
{
	int status = 0;

	ilrd_uid_t uid4 = {0, 0, 0};
	int clean_up_param4 = 103;
	time_t t4 = time(0);

	sched_t *s = SchedCreate();
	status += (NULL == s);

	status += CompereInt(1, SchedIsEmpty(s), __LINE__);
	status += CompereSt(0, SchedSize(s), __LINE__);

	uid4 = SchedAddTask(s, StopIfActionFunc, s,
						CleanUpFunc, &clean_up_param4, t4);
	status += (UIDIsSame(uid4, UIDBadUID));
	status += CompereSt(1, SchedSize(s), __LINE__);

	printf("\n****** Running ********\n\n");

	status += CompereInt(0, SchedRun(s), __LINE__);
	status += CompereSt(1, SchedSize(s), __LINE__);

	SchedDestroy(s);
	return status;
}

int TaskAddTaskTest()
{
	int status = 0;
	ilrd_uid_t uid1 = {0, 0, 0};
	int clean_up_param = 1;
	time_t t = 3;

	sched_t *s = SchedCreate();
	status += (NULL == s);

	status += CompereInt(1, SchedIsEmpty(s), __LINE__);
	status += CompereSt(0, SchedSize(s), __LINE__);

	uid1 = SchedAddTask(s, AddActionFunc, s, CleanUpFunc, &clean_up_param, t);
	status += (UIDIsSame(uid1, UIDBadUID));

	printf("\n****** Running ********\n\n");
	status += CompereInt(0, SchedRun(s), __LINE__);

	SchedDestroy(s);
	return status;
}

int TaskRemoveTaskTest()
{
	int status = 0;
	ilrd_uid_t uid1 = {0, 0, 0};
	ilrd_uid_t uid2 = {0, 0, 0};
	int action_param = 20;
	int clean_up_param = 1;
	int clean_up_param2 = 2;
	time_t t = time(0);
	time_t t2 = time(0) + 8;

	task_remover remove_uid1 = {NULL, {0, 0, 0}};

	sched_t *s = SchedCreate();
	status += (NULL == s);

	status += CompereInt(1, SchedIsEmpty(s), __LINE__);
	status += CompereSt(0, SchedSize(s), __LINE__);

	uid1 = SchedAddTask(s, LoopActionFunc, &action_param,
						CleanUpFunc, &clean_up_param, t);
	status += (UIDIsSame(uid1, UIDBadUID));

	status += CompereInt(0, SchedIsEmpty(s), __LINE__);
	status += CompereSt(1, SchedSize(s), __LINE__);

	remove_uid1.sched = s;
	remove_uid1.uid = uid1;

	uid2 = SchedAddTask(s, RemoveActionFunc, &remove_uid1,
						CleanUpFunc, &clean_up_param2, t2);
	status += (UIDIsSame(uid2, UIDBadUID));

	printf("\n****** Running ********\n\n");
	status += CompereInt(0, SchedRun(s), __LINE__);

	SchedDestroy(s);
	return status;
}

int TaskCountIsEmptyItselfTest()
{
	int status = 0;
	ilrd_uid_t uid1 = {0, 0, 0};
	ilrd_uid_t uid2 = {0, 0, 0};
	ilrd_uid_t uid3 = {0, 0, 0};
	int clean_up_param = 1;
	int action_param2 = 33;
	time_t t = time(0);
	time_t t2 = time(0) + 3;

	sched_t *s = SchedCreate();
	status += (NULL == s);

	status += CompereInt(1, SchedIsEmpty(s), __LINE__);
	status += CompereSt(0, SchedSize(s), __LINE__);

	uid1 = SchedAddTask(s, CountIsEmptyActionFunc, s,
						CleanUpFunc, &clean_up_param, t);
	status += (UIDIsSame(uid1, UIDBadUID));

	status += CompereInt(0, SchedIsEmpty(s), __LINE__);
	status += CompereSt(1, SchedSize(s), __LINE__);

	uid2 = SchedAddTask(s, ActionFunc, &action_param2,
						CleanUpFunc, &clean_up_param, t2);
	status += (UIDIsSame(uid2, UIDBadUID));

	uid3 = SchedAddTask(s, ActionFunc, &action_param2,
						CleanUpFunc, &clean_up_param, t2);
	status += (UIDIsSame(uid3, UIDBadUID));

	printf("\n****** Running ********\n\n");
	status += CompereInt(0, SchedRun(s), __LINE__);

	SchedDestroy(s);
	return status;
}

int AddActionFunc(void *sched)
{
	static int action_param = 100321;
	static int clean_up_param = 100321;
	time_t t = time(0) + 8;
	sched_t *s = (sched_t *)sched;
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("This Task Is Adding Task\n\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	SchedAddTask(s, ActionFunc, &action_param, CleanUpFunc,
				 &clean_up_param, t);

	return 0;
}

int CountIsEmptyActionFunc(void *sched)
{
	sched_t *s = (sched_t *)sched;
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("Is sched Empty? %d,  ", SchedIsEmpty(s));
	printf("Size of sched: %lu\n", SchedSize(s));
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	return 0;
}

int RemoveActionFunc(void *to_remove)
{
	task_remover *remove = (task_remover *)to_remove;
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("This Task Is Removing task\n\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	SchedRemoveTask(remove->sched, remove->uid);

	return 0;
}

int StopActionFunc(void *sched)
{
	sched_t *s = (sched_t *)sched;
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("This task Initiate Stop: Stopping Run\n\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	SchedStop(s);

	return 5;
}

int StopIfActionFunc(void *sched)
{
	sched_t *s = (sched_t *)sched;
	printf("Serching for file ...\n\n");

	if (0 == access("stop", F_OK))
	{
		remove("stop");
		SchedStop(s);
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("Stop file detected: Stopping Run\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	}
	else
	{
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("No file Found! Continue Run\n\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	}

	return 1;
}

int LoopActionFunc(void *action_param)
{
	printf("loop ~ before action: %d, ", *(int *)action_param);
	*(int *)action_param += 2;
	printf("loop ~ param after action: %d\n", *(int *)action_param);

	return time(0) + 1;
}

int ActionFunc(void *action_param)
{
	printf("Once ~ before action: %d, ", *(int *)action_param);
	*(int *)action_param += 2;
	printf("Once ~ param after action: %d\n", *(int *)action_param);
	return 0;
}

void CleanUpFunc(void *clean_up_param)
{
	printf("cleaned up! param is: %d\n", *(int *)clean_up_param);
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
