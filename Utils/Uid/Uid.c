/*****************************************************************************
 *Name: Chen Rudy											 			 *
 *Reviewar: May											 			 *
 *Date: 03/28/22													 			 *
 *uid.c														 			 *
 ******************************************************************************/
#include <assert.h> /* for assert */
#include "Uid.h"

const ilrd_uid_t UIDBadUID = {0, 0, 0};

ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t uid = {0, 0, 0};
	static size_t counter = 0;

	uid.pid = getpid();
	uid.time = time(0);

	uid.counter = __sync_add_and_fetch(&counter, 1);

	if (-1 == uid.time)
	{
		return UIDBadUID;
	}

	return uid;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	size_t status = 0;

	status += uid1.pid == uid2.pid;
	status += uid1.time == uid2.time;
	status += uid1.counter == uid2.counter;

	return status == 3;
}
