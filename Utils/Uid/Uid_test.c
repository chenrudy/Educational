/*****************************************************************************
 *Name: Chen Rudy												 			 *
 *Reviewar:	May									 			 *
 *Date: 03/28/22													 			 *
 *uid_test.c													 			 *
 ******************************************************************************/

#include <stdio.h> /* for printf */
#include "Uid.h"

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

int CompereInt(int expected_val, int recieved_val, int line);
size_t CompereSt(size_t expected_val, size_t recieved_val, int line);
int UIDCreateTest();
int UIDIsSameTest();

int main()
{
	RUNTEST(UIDCreateTest());
	RUNTEST(UIDIsSameTest());

	return 0;
}

int UIDCreateTest()
{
	int status = 0;
	ilrd_uid_t uid = UIDCreate();
	ilrd_uid_t uid2 = UIDCreate();
	ilrd_uid_t uid3 = UIDCreate();
	ilrd_uid_t uid4 = UIDCreate();

	status += (0 == uid.pid);
	status += (0 == uid.time);
	status += CompereSt(1, uid.counter, __LINE__);

	printf("first PID     = %d\n", uid.pid);
	printf("first time    = %ld\n", uid.time);
	printf("first counter = %ld\n", uid.counter);

	status += (0 == uid2.pid);
	status += (0 == uid2.time);
	status += CompereSt(2, uid2.counter, __LINE__);

	printf("second PID     = %d\n", uid2.pid);
	printf("second time    = %ld\n", uid2.time);
	printf("second counter = %ld\n", uid2.counter);

	status += (0 == uid3.pid);
	status += (0 == uid3.time);
	status += CompereSt(3, uid3.counter, __LINE__);

	printf("third PID     = %d\n", uid3.pid);
	printf("third time    = %ld\n", uid3.time);
	printf("third counter = %ld\n", uid3.counter);

	status += (0 == uid4.pid);
	status += (0 == uid4.time);
	status += CompereSt(4, uid4.counter, __LINE__);

	printf("forth PID     = %d\n", uid4.pid);
	printf("forth time    = %ld\n", uid4.time);
	printf("forth counter = %ld\n", uid4.counter);

	return status;
}

int UIDIsSameTest()
{
	int status = 0;
	ilrd_uid_t uid = UIDCreate();
	ilrd_uid_t uid2 = UIDCreate();
	ilrd_uid_t uid3 = UIDCreate();
	ilrd_uid_t uid4 = UIDCreate();

	status += CompereInt(0, UIDIsSame(uid, uid2), __LINE__);
	status += CompereInt(0, UIDIsSame(uid, uid3), __LINE__);
	status += CompereInt(0, UIDIsSame(uid, uid4), __LINE__);
	status += CompereInt(1, UIDIsSame(uid, uid), __LINE__);
	status += CompereInt(1, UIDIsSame(uid2, uid2), __LINE__);

	status += CompereInt(0, UIDIsSame(uid, uid2), __LINE__);

	return status;
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
