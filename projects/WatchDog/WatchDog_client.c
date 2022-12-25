/*****************************************************************************
 *Author: Chen Rudy
 *Date: 18/06/2022
this is the client code, the test for the watchdog. client process will run this code
 ******************************************************************************/
#include <stdio.h>  /* for printf */
#include <stdlib.h> /* for sleep  */
#include <sys/types.h>

#include "Scheduler.h" /*scheduler header*/
#include "Uid.h"       /* unique identifier creator header*/
#include <errno.h>     /* for perror */
#include "WatchDog.h"

#define MAX (15)

int main(int argc, char *argv[])
{
    int a = 0;

    (void)argc;
    if (0 != WDMakeMeImmortal(argv))
    {
        perror("WDMakeMeImmortal function (called by client process) Failed");
    }

    while (MAX > a++) /* give time for watchdog to watch.
                        here should be the code the client wants to protect with our watchdog */
    {
        sleep(1);
    }

    DoNotResuscitate();

    return 0;
}