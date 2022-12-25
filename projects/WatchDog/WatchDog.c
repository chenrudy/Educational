/*****************************************************************************
 *Author: Chen Rudy
 *Date: 18/06/2022
 API for the watchdog.
 this code is shared by the watchdog thread and watchdog process
 ******************************************************************************/
#define _POSIX_C_SOURCE (200112L) /* for enviorment var */
#include <stdio.h>                /* for printf */
#include <sys/types.h>            /* for pid_t , sig_atomic_t*/
#include <stdatomic.h>            /* for atomic_int type */
#include <signal.h>               /* for sigaction */
#include <string.h>               /* for memset */
#include <stdlib.h>               /* for get and set env */
#include <time.h>                 /* for time */
#include <pthread.h>              /* for pthread functions */
#include <string.h>               /* for strtok and  strcspn */
#include <unistd.h>               /* for fork and execpv */
#include <errno.h>                /* for perror */

#include "Scheduler.h" /*scheduler header*/
#include "Uid.h"       /* unique identifier creator header*/
#include "Semaphore.h" /* semaphore implementation */
#include "WatchDog.h"

#define CHILD (0)
#define STR (15)
#define DONT_RESCHEDUAL (0)
#define WD_EXE_NAME ("./watchdog.out")
#define EQUAL (0)
#define ENV_CLIENT ("WD_PID")
#define ENV_CHILD ("WATCHDOG_PID")
#define SIG_DELAY_LIMIT (4)
#define REMAIN (0)
#define OVERWRITE (1)
#define RESET_COUNTER (0)

enum STOP_FLAG
{
    OFF = 0,
    ON = 1
};

enum RET
{
    FORK_FAIL = -1,
    SUCCESS = 0,
    FAIL = 1
};

enum WHO
{
    WATCHDOG_CHILD_PID = 0,
    CLIENT = 2,
    WATCH_DOG = 3
};

typedef struct info_reboot
{
    char **arguments;
    sched_t *schedular;
} info_reboot;

volatile sig_atomic_t g_stop_flag = OFF;
volatile sig_atomic_t g_reboot_flag = OFF;
static atomic_int g_counter = 0;

/* help functions */
static int ForkExecChild(char *argv[]);
static void *routine(void *arg);
static int InitSched(sched_t **sched, info_reboot *info);
static int WhoOwnsProgram(pid_t pid);
static int RebootWatchdog(sched_t *sched, char **args);
static int RebootClient(sched_t *sched, char **args);
/* tasks for scheduler */
static int SemFunc(void *action_param);
static int SendSigFunc(void *action_param);
static int VitalsFunc(void *action_param);
static int StopIfFunc(void *action_param);
static int RebootFunc(void *action_param);
/* clean functions for scheduler */
static void CleanSem(void *clean_up_param);
static void CleanStop(void *clean_up_param);
static void CleanUpFunc(void *clean_up_param);
/* handlers */
static int SetHandlers();
static void Siguser1Handler(int signo);
static void Siguser2Handler(int signo);

/********************************* API *************************************/

int WDMakeMeImmortal(char *argv[])
{
    int program = 0;
    int ret_val = 0;
    pid_t pid = 0;

    char client_pid[STR] = {0};
    sprintf(client_pid, "%d", getpid());

    if (SUCCESS != setenv(ENV_CLIENT, client_pid, REMAIN))
    {
        perror("setenv Failed");
        return FAIL;
    }
    program = WhoOwnsProgram(getpid());

    ret_val = SetHandlers();
    if (SUCCESS != ret_val)
    {
        return ret_val;
    }

    if (CLIENT == program)
    {
        printf("DAD IS %d\n\n", getpid());
        pid = ForkExecChild(argv);
        if (FAIL == pid)
        {
            perror("ForkExecChild function Failed");
            return FAIL;
        }
    }

    if (WATCH_DOG == program)
    {
        printf("DOG IS %d\n\n", getpid());
        routine(argv);
    }

    return SUCCESS;
}

void DoNotResuscitate()
{
    pid_t watchdog_pid = atoi(getenv(ENV_CHILD));
    pid_t ret = 0;
    printf("\n  XXXXX  DoNotResuscitate  %d , %d  XXXXX  \n", watchdog_pid, getpid());
    kill(watchdog_pid, SIGUSR2);
    raise(SIGUSR2);

    while (-1 == waitpid(watchdog_pid))
    {
        sleep(1);
    }
}

/****************************** help functions **********************************/

static int ForkExecChild(char *argv[])
{
    pid_t pid = 0;
    pthread_t dog_watcher = 0;
    pthread_attr_t attr = {0};

    char child_pid[15] = {0};

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (NULL == getenv(ENV_CHILD))
    {
        printf("FORKING\n\n");
        pid = fork();
        if (CHILD == pid)
        {
            execvp("./watchdog.out", argv);
            return FAIL;
        }
        else if (FORK_FAIL == pid)
        {
            perror("fork Failed");
            return FAIL;
        }

        sprintf(child_pid, "%d", pid);
        if (SUCCESS != setenv(ENV_CHILD, child_pid, OVERWRITE))
        {
            perror("setenv Failed");
            return FAIL;
        }
    }

    if (SUCCESS != pthread_create(&dog_watcher, &attr, &routine, argv))
    {
        perror("Failed to create thread");
        return FAIL;
    }
    pthread_detach(dog_watcher);

    return pid;
}

static void *routine(void *arg)
{
    sched_t *sched = NULL;
    info_reboot info;
    int ret_val = SUCCESS;
    sched = SchedCreate();
    if (NULL == sched)
    {
        perror("SetSchedUp failed in SchedCreate");
    }

    info.arguments = (char **)arg;
    info.schedular = sched;

    if (SUCCESS != InitSched(&sched, &info))
    {
        perror("SetSchedUp returned fail");
        return NULL;
    }

    ret_val = SchedRun(sched);
    if (SUCCESS != ret_val)
    {
        perror("SchedRun returned fail");
        return NULL;
    }

    SchedDestroy(sched);

    return NULL;
}

static int InitSched(sched_t **sched, info_reboot *info)
{
    ilrd_uid_t uid_sem = {0, 0, 0};
    ilrd_uid_t uid_signal = {0, 0, 0};
    ilrd_uid_t uid_vitals = {0, 0, 0};
    ilrd_uid_t uid_stop = {0, 0, 0};
    ilrd_uid_t uid_reboot = {0, 0, 0};
    time_t t_sem = time(0) + 1;
    time_t t_sig = time(0) + 2;
    time_t t_vit = time(0) + 3;
    time_t t_st = time(0) + 4;
    time_t t_re = time(0) + 5;

    uid_sem = SchedAddTask(*sched, SemFunc, NULL,
                           CleanSem, NULL, t_sem);
    if (UIDIsSame(uid_sem, UIDBadUID))
    {
        perror("SetSchedUp failed in adding task");
        return FAIL;
    }

    uid_signal = SchedAddTask(*sched, SendSigFunc, NULL,
                              CleanUpFunc, NULL, t_sig);
    if (UIDIsSame(uid_signal, UIDBadUID))
    {
        perror("SetSchedUp failed in adding task");
        return FAIL;
    }

    uid_vitals = SchedAddTask(*sched, VitalsFunc, NULL,
                              CleanUpFunc, NULL, t_vit);
    if (UIDIsSame(uid_vitals, UIDBadUID))
    {
        perror("SetSchedUp failed in adding task");
        return FAIL;
    }
    uid_stop = SchedAddTask(*sched, StopIfFunc, *sched,
                            CleanStop, NULL, t_st);
    if (UIDIsSame(uid_stop, UIDBadUID))
    {
        perror("SetSchedUp failed in adding task");
        return FAIL;
    }

    uid_reboot = SchedAddTask(*sched, RebootFunc, info,
                              CleanUpFunc, NULL, t_re);
    if (UIDIsSame(uid_reboot, UIDBadUID))
    {
        perror("SetSchedUp failed in adding task");
        return FAIL;
    }

    return SUCCESS;
}

static int WhoOwnsProgram(pid_t pid)
{
    char current_pid[STR] = {0};
    sprintf(current_pid, "%d", pid);

    if (NULL != getenv(ENV_CLIENT) && EQUAL == strcmp(getenv(ENV_CLIENT), current_pid))
    {
        return CLIENT;
    }

    return WATCH_DOG;
}

static int RebootWatchdog(sched_t *sched, char **args)
{
    pid_t pid = 0;
    ilrd_uid_t uid_sem = {0, 0, 0};
    char watchdog_pid[15] = {0};

    g_counter = RESET_COUNTER;
    g_reboot_flag = OFF;
    printf("client is alive, reeboot watchdog %d\n", getpid());

    pid = fork();
    sprintf(watchdog_pid, "%d", pid);
    if (SUCCESS != setenv(ENV_CHILD, watchdog_pid, OVERWRITE))
    {
        perror("setenv Failed");
        return FAIL;
    }

    if (CHILD == pid)
    {
        printf("EXECVP-ING!!\n\n");
        execvp("./watchdog.out", args);
        perror("failed to execvp to reboot!");
        return FAIL;
    }
    else if (FORK_FAIL == pid)
    {
        perror("fork Failed");
        return FAIL;
    }

    uid_sem = SchedAddTask(sched, SemFunc, NULL, CleanUpFunc, NULL, 3);
    if (UIDIsSame(uid_sem, UIDBadUID))
    {
        perror("SetSchedUp failed in adding task");
        return FAIL;
    }

    return SUCCESS;
}

static int RebootClient(sched_t *sched, char **args)
{
    ilrd_uid_t uid_sem = {0, 0, 0};
    pid_t pid = 0;
    char client_pid[15] = {0};
    char watchdog_pid[15] = {0};
    semid_t sem_f = SemCreate("/", 'F', 0);

    printf("watchdog is alive, reeboot client %d\n", getpid());
    unsetenv(ENV_CLIENT);
    unsetenv(ENV_CHILD);
    g_counter = RESET_COUNTER;
    g_reboot_flag = OFF;

    pid = fork();
    if (pid != 0) /* send parent to be client , child is new watchdog to sem_wait in sched until parent sched will post */
    {
        sprintf(client_pid, "%d", getpid());
        if (SUCCESS != setenv(ENV_CLIENT, client_pid, REMAIN))
        {
            perror("setenv Failed");
            return FAIL;
        }

        sprintf(watchdog_pid, "%d", pid);
        if (SUCCESS != setenv(ENV_CHILD, watchdog_pid, OVERWRITE))
        {
            perror("setenv Failed");
            return FAIL;
        }
        g_reboot_flag = OFF;

        SemIncrease(sem_f, 1, 1);
        printf("EXECVP-ING!!\n\n");
        execvp(*args, args);
        perror("failed to execvp to reboot!");
        exit(1);
    }
    else if (FORK_FAIL == pid)
    {
        perror("fork Failed");
        return FAIL;
    }
    else
    {
        SemDecrease(sem_f, 1, 1);
        uid_sem = SchedAddTask(sched, SemFunc, NULL, CleanUpFunc, NULL, 3);
        if (UIDIsSame(uid_sem, UIDBadUID))
        {
            perror("SetSchedUp failed in adding task");
            return FAIL;
        }
        SemDestroy(sem_f);
    }
    return SUCCESS;
}

/****************************** tasks for scheduler ******************************/

static int SemFunc(void *action_param)
{
    semid_t sem_d = SemCreate("/", 'D', 0);
    semid_t sem_c = SemCreate("/", 'C', 0);

    if (WATCH_DOG == WhoOwnsProgram(getpid()))
    {
        printf("dog is waiting! %d, %d\n******\n", getpid(), SemGetVal(sem_d));
        SemDecrease(sem_d, 1, 1);
        SemIncrease(sem_c, 1, 1);
        printf("dog is READY! %d\n~~~~~~~~\n", getpid());
    }
    else
    {
        printf("thread is waiting! %d\n******\n", getpid());
        SemIncrease(sem_d, 1, 1);
        SemDecrease(sem_c, 1, 1);
        printf("thread is READY! %d\n~~~~~~~\n", getpid());
    }

    return DONT_RESCHEDUAL;
}

static int SendSigFunc(void *action_param)
{
    (void)action_param;

    if (WATCH_DOG == WhoOwnsProgram(getpid()))
    {
        printf("Sending signal user1 from watchdog %d to client thread %d\n", getpid(), getppid());
        kill(getppid(), SIGUSR1);
    }
    else
    {
        printf("Sending signal user1 from client thread %d to watchdog %d\n", getpid(), atoi(getenv(ENV_CHILD)));
        kill(atoi(getenv(ENV_CHILD)), SIGUSR1);
    }

    ++g_counter;
    return time(NULL) + 4;
}

static int VitalsFunc(void *action_param)
{
    (void)action_param;
    printf("cheak for vitals\n");

    if (SIG_DELAY_LIMIT < g_counter)
    {
        g_reboot_flag = ON;
        printf("\n   CODE BLUE !!! start C-A-B CPR !! bring an AED and CALL 101 \n\n");
    }

    return time(NULL) + 4;
}

static int StopIfFunc(void *action_param)
{
    sched_t *sched = (sched_t *)action_param;
    printf("do i need to stop?%d\n", WhoOwnsProgram(getpid()));

    if (ON == g_stop_flag)
    {
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("STOP sched of pid %d\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", getpid());
        SchedStop(sched);
    }

    return time(NULL) + 4;
}

static int RebootFunc(void *action_param)
{
    info_reboot *info = (info_reboot *)action_param;

    if (ON == g_reboot_flag)
    {
        if (CLIENT == WhoOwnsProgram(getpid()))
        {
            if (SUCCESS != RebootWatchdog(info->schedular, info->arguments))
            {
                printf("FAIL\n");
                exit(1);
            }
        }
        else
        {
            if (SUCCESS != RebootClient(info->schedular, info->arguments))
            {
                printf("FAIL\n");
                exit(1);
            }
        }
    }
    return time(NULL) + 4;
}

/*********************** clean functions for scheduler ***************************/

static void CleanSem(void *clean_up_param)
{
    semid_t sem_d = SemCreate("/", 'D', 0);
    semid_t sem_c = SemCreate("/", 'C', 0);
    (void)clean_up_param;

    SemDestroy(sem_d);
    SemDestroy(sem_c);
    printf("cleaned up! Destroy Semaphore\n");
}

static void CleanStop(void *clean_up_param)
{
    printf("CleanStop -> who am i?%d, %d\n", WhoOwnsProgram(getpid()), getpid());
    if (CLIENT == WhoOwnsProgram(getpid()))
    {
        unsetenv(ENV_CLIENT);
        unsetenv(ENV_CHILD);
    }
}

static void CleanUpFunc(void *clean_up_param)
{
    (void)clean_up_param;
    printf("cleaned up!\n");
}

/************************************ handlers ************************************/
static int SetHandlers()
{
    struct sigaction sa1;
    struct sigaction sa2;

    memset(&sa1, 0, sizeof(sa1));
    sa1.sa_handler = &Siguser1Handler;

    memset(&sa2, 0, sizeof(sa2));
    sa2.sa_handler = &Siguser2Handler;

    if (sigaction(SIGUSR1, &sa1, NULL))
    {
        perror("sigaction error");
        return FAIL;
    }

    if (sigaction(SIGUSR2, &sa2, NULL))
    {
        perror("sigaction error");
        return FAIL;
    }

    return SUCCESS;
}

static void Siguser1Handler(int signo)
{
    (void)signo;
    g_counter = RESET_COUNTER;
}

static void Siguser2Handler(int signo)
{
    (void)signo;
    g_stop_flag = ON;
}
