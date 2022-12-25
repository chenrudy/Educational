 /*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Author: Chen Rudy     
- Date: 03/28/22	
- Reviewed By: May
- uid.h
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

***************************************************************************
                                    DS
****************************************************************************/ 

#ifndef __UID__
#define __UID__

#include <unistd.h>
#include <time.h>

/*******************************************************************************
                                 Typedefs
*******************************************************************************/
typedef int pid_t;

typedef struct
{
  pid_t pid;
  time_t time;
  size_t counter;
} ilrd_uid_t;


/*******************************************************************************
                                Functions
*******************************************************************************/
ilrd_uid_t UIDCreate(void);
/******************************************************************************
UIDCreate
Description: create a unique identifier
Parameters: 
	- void
Return value:
	- ilrd_uid_t uid
	- if failed returns UIDBadUID
Complexity: O(1)
Notes:

*****************************************************************************/
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2);
/******************************************************************************
UIDIsSame
Description: comperes between uid1 and uid2
Parameters: 
	- ilrd_uid_t uid1
	- ilrd_uid_t uid2
Return value:
	- returns true (1) if uids are identicle 
	- returns false (0) if uids are diffrent 
Complexity: O(1)
Notes:

*****************************************************************************/
extern const ilrd_uid_t UIDBadUID;
/******************************************************************************
a non-valid unique identifier
*****************************************************************************/


#endif /* __UID__ */
