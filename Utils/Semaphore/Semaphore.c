/*****************************************************************************
 *Author: Chen Rudy															 *
 *Reviewer: Amir Litvak  														 *
 *Date: 16/06/2022															 *
 *semaphore.c																		 *
 ******************************************************************************/

#include <assert.h>    /* for assert */
#include <string.h>    /* for memset */
#include <stdio.h>     /* for peror */
#include <errno.h>     /* for error handlling */
#include <sys/types.h> /* for semaphores */
#include <sys/ipc.h>   /* for semaphores */
#include <sys/sem.h>   /* for semaphores */
#include <stdlib.h>

#include "Semaphore.h"

#define INVALID_SEMAPHORE (-1)

static int SemIsValid(semid_t semaphore);

static int SemOp(semid_t id, int num, int set_undo, int op_flag);

enum ret
{
    FAIL = -1,
    SUCCESS = 0
};

enum Op
{
    INCREASE = 1,
    DECREASE = -1
};

union semun
{
    int val;
    struct semid_ds *buf;
};

semid_t SemCreate(const char *name, int proj_id, size_t sem_value)
{
    semid_t semid = 0;
    key_t semkey = 0;
    struct sembuf sb = {0, 0, 0};
    union semun value = {0};
    value.val = sem_value;

    memset(&sb, 0, sizeof(sb));

    if (FAIL == (semkey = ftok(name, proj_id)))
    {
        perror("IPC error: ftok");
        return FAIL;
    }

    /* Get semaphore ID associated with this key. */
    if ((semid = semget(semkey, 0, 0)) == -1)
    {
        /* Semaphore does not exist - Create. */
        if ((semid = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0666)) != -1)
        {
            /* Initialize the semaphore. */
            if (FAIL == semctl(semid, 0, SETVAL, value))
            {
                SemDestroy(semid);
                perror("IPC error: semctl");
                return FAIL;
            }
        }
        else if (errno == EEXIST)
        {
            if ((semid = semget(semkey, 0, 0)) == -1)
            {
                perror("IPC error 1: semget");
                exit(1);
            }
        }
        else
        {
            perror("IPC error 2: semget");
            exit(1);
        }
    }

    return semid;
}

int SemDestroy(semid_t id)
{
    return semctl(id, 0, IPC_RMID);
}

int SemIncrease(semid_t id, int num, int set_undo)
{
    assert(SemIsValid(id));

    return SemOp(id, num, set_undo, INCREASE);
}

int SemDecrease(semid_t id, int num, int set_undo)
{
    assert(SemIsValid(id));

    return SemOp(id, num, set_undo, DECREASE);
}

static int SemOp(semid_t id, int num, int set_undo, int op_flag)
{
    struct sembuf signal = {0, 0, 0};
    num *= op_flag;
    signal.sem_op = num;

    if (1 == set_undo)
    {
        signal.sem_flg = SEM_UNDO;
    }

    if (0 == num)
    {
        return 0;
    }

    return semop(id, &signal, 1);
}

static int SemIsValid(semid_t semaphore)
{
    return semaphore > INVALID_SEMAPHORE;
}

int SemGetVal(semid_t id)
{
    return semctl(id, 0, GETVAL);
}
