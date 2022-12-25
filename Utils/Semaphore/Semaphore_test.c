/*****************************************************************************
 *Author: Chen Rudy															 *
 *Reviewer: Amir Litvak  														 *
 *Date: 16/06/2022
 to view sys v semaphores: ipcs -s command
 to manually remove a sys v semaphore: ipcrm -s + the semid of semaphore
 ******************************************************************************/

#include <stdio.h>  /* for printf and fgets */
#include <stdlib.h> /* for atoi */
#include <string.h> /* for strncmp and strsyt */

#include "Semaphore.h" /* ILR&D funcs */

int main()
{

    semid_t id = 0;
    char buffer[50];
    char *runner = NULL;
    int undo = 0;
    int num = 0;

    id = SemCreate("file", 'S', 1);

    printf("Type undo to add undo flag\nd + number to decrease by\ni + number to increase by\n"
           "v to get value of semaphore.\ntype exit to exit test\n");

    while (1)
    {
        if (!fgets(buffer, 50, stdin))
        {
            return 1;
        }

        if (!strcmp(buffer, "exit\n"))
        {
            break;
        }

        runner = buffer;
        runner++;
        num = atoi(runner);

        if (strstr(buffer, "undo"))
        {
            undo = 1;
            printf("undo  ");
        }

        if (!strncmp(buffer, "d", 1LU))
        {
            SemDecrease(id, num, undo);
            printf("decrease val by %d is: %d\n", num, SemGetVal(id));
        }
        else if (!strncmp(buffer, "i", 1LU))
        {
            SemIncrease(id, num, undo);
            printf("increase val by %d is: %d\n", num, SemGetVal(id));
        }
        else if (!strncmp(buffer, "v", 1LU))
        {
            printf("semaphore value is %d\n", SemGetVal(id));
        }
        undo = 0;
    }

    SemDestroy(id);

    return 0;
}