/*****************************************************************************
 *Author: Chen Rudy
 *Reviewer: Nuri
 *Date: 18/06/2022
 Makefile created a hidden executable of this code, which will be run be the child process of the watchdog
 ******************************************************************************/

#include <stdio.h>  /* for printf */
#include <stdlib.h> /* for sleep  */

#include "WatchDog.h"

#define MAX (15)

int main(int argc, char *argv[])
{
    (void)argc;

    if (0 != WDMakeMeImmortal(argv))
    {
        perror("WDMakeMeImmortal function (called by child process) Failed");
    }

    return 0;
}