#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PROJECT_ID 1
#define NUMBER_SEM 2

int createSem(const char *pathName, int projectID);
int deleteSem(const char *pathName, int projectID);
int getSem(const char *pathName, int projectID);
int setSem(const char *pathName, int projectID);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("参数列表数量错误\n");
        return -1;
    }

    printf("IPC SEM Test\n");
    printf("========================\n");

    printf("Process ID:%d\n", getpid());

    if (createSem(argv[1], PROJECT_ID) != 0)
    {
        return -1;
    }

    if (setSem(argv[1], PROJECT_ID) != 0)
    {
        return -1;
    }

    key_t key = ftok(argv[1], PROJECT_ID);
    printf("key:%d\n", key);
    if (key < 0)
    {
        printf("文件路径错误\n");
        return -1;
    }

    int semID = semget(key, NUMBER_SEM, IPC_EXCL);
    printf("sem ID:%d\n", semID);
    if (semID < 0)
    {
        printf("获取信号量集ID失败\n");
        return -1;
    }

    int m = 0, n = 0;

    pid_t ret_fork = fork();
    if (ret_fork > 0) // Pareten:t process
    {

        int semVal[2] = {0};
        while (1)
        {
            getSem(argv[1], PROJECT_ID);
            semVal[0] = semctl(semID, 0, GETVAL);
            semVal[1] = semctl(semID, 1, GETVAL);
            printf("%d %d\n", semVal[0], semVal[1]);
            sleep(1);
            if (semVal[0] == 1 && semVal[1] == 0)
            {
                break;
            }
        }

        struct sembuf buf1[NUMBER_SEM] = {
            {1, 0, 0},
            {0, -1, 0}};

        struct sembuf buf2[NUMBER_SEM] = {
            {0, 0, 0},
            {1, 1, 0}};
        while (1)
        {
            getSem(argv[1], PROJECT_ID);
            int retval = semop(
                semID,
                buf1,
                sizeof(buf1) / sizeof(struct sembuf));
            sleep(1);
            printf("Parent process PID:%d PPID:%d m:%d n:%d\n",
                   getpid(),
                   getppid(),
                   m,
                   n++);

            if (n > 10)
            {
                deleteSem(argv[1], PROJECT_ID);
                break;
            }

            getSem(argv[1], PROJECT_ID);
            retval = semop(
                semID,
                buf2,
                sizeof(buf2) / sizeof(struct sembuf));
            getSem(argv[1], PROJECT_ID);
        }
        wait(NULL);
        printf("------------------------\n");
    }
    else if (ret_fork == 0) // Child process
    {
        struct sembuf buf1[NUMBER_SEM] = {
            {0, 0, 0},
            {1, -1, 0}};

        struct sembuf buf2[NUMBER_SEM] = {
            {1, 0, 0},
            {0, 1, 0}};

        int semVal[2] = {0};
        while (1)
        {
            getSem(argv[1], PROJECT_ID);
            semVal[0] = semctl(semID, 0, GETVAL);
            semVal[1] = semctl(semID, 1, GETVAL);
            printf("%d %d\n", semVal[0], semVal[1]);
            sleep(1);
            if (semVal[0] == 0 && semVal[1] == 1)
            {
                break;
            }
        }

        while (1)
        {

            getSem(argv[1], PROJECT_ID);
            int retval = semop(
                semID,
                buf1,
                sizeof(buf1) / sizeof(struct sembuf));
            if (retval < 0)
            {
                break;
            }
            sleep(1);
            printf("Child process PID:%d PPID:%d m:%d n:%d\n",
                   getpid(),
                   getppid(),
                   m++,
                   n);

            getSem(argv[1], PROJECT_ID);
            retval = semop(
                semID,
                buf2,
                sizeof(buf2) / sizeof(struct sembuf));
            printf("Retval:%d\n", retval);
            if (retval < 0)
            {
                break;
            }
            getSem(argv[1], PROJECT_ID);

        }
        /* 主动结束进程，避免父进程成为僵尸进程 */
        printf("SubProcess exti\n");
        printf("++++++++++++++++++++++++\n");

        exit(0);
    }
    else // Fork process error
    {
        printf("Fork process fail\n");
        return -1;
    }

    printf("IPC SEM Test End \n");
    return 0;
}

int createSem(const char *pathName, int projectID)
{
    key_t key = ftok(pathName, projectID);
    printf("key:%d\n", key);
    if (key < 0)
    {
        printf("文件路径错误\n");
        return -1;
    }

    int semID = semget(key, NUMBER_SEM, IPC_CREAT | IPC_EXCL);
    printf("sem ID:%d\n", semID);
    if (semID < 0)
    {
        printf("创建信号量集失败\n");
        return -1;
    }

    /* 首次操作信号量集即初始化，不可为负值 */
    struct sembuf buf[NUMBER_SEM] = {
        {0, 0, 0},
        {1, 0, 0}};
    int retval = semop(
        semID,
        buf,
        sizeof(buf) / sizeof(struct sembuf));

    if (retval != 0)
    {
        printf("初始化信号量集失败\n");
        return -1;
    }
    printf("初始化信号量集成功\n");

    return 0;
}

int deleteSem(const char *pathName, int projectID)
{
    key_t key = ftok(pathName, projectID);
    printf("key:%d\n", key);
    if (key < 0)
    {
        printf("文件路径错误\n");
        return -1;
    }

    int semID = semget(key, 0, IPC_EXCL);
    printf("sem ID:%d\n", semID);
    if (semID < 0)
    {
        printf("获取信号量集失败\n");
        return -1;
    }

    int retval = semctl(semID, 0, IPC_RMID);
    if (retval == -1)
    {
        printf("删除信号量集失败\n");
        return -1;
    }
    printf("删除信号量集成功，将唤醒被阻塞进程,retval:%d\n", retval);

    return 0;
}

union semun {
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO
				    (Linux-specific) */
};

int getSem(const char *pathName, int projectID)
{
    key_t key = ftok(pathName, projectID);
    printf("key:%d\n", key);
    if (key < 0)
    {
        printf("文件路径错误\n");
        return -1;
    }

    int semID = semget(key, 0, IPC_EXCL);
    printf("sem ID:%d\n", semID);
    if (semID < 0)
    {
        printf("获取信号量集失败\n");
        return -1;
    }

    union semun semun;
    unsigned short semVarAry[NUMBER_SEM] = {0};
    semun.array = semVarAry;

    int retval = semctl(semID, 0, GETALL, semun);
    printf("Retval:%d\n", retval);
    if (retval == -1)
    {
        printf("信号量集获取失败\n");
        return -1;
    }
    printf("信号量集获取成功\n");

    int i;
    for (i = 0; i < NUMBER_SEM; i++)
    {
        printf("Sem no %d:value = %d\n", i, semVarAry[i]);
    }

    return 0;
}

int setSem(const char *pathName, int projectID)
{
    key_t key = ftok(pathName, projectID);
    printf("key:%d\n", key);
    if (key < 0)
    {
        printf("文件路径错误\n");
        return -1;
    }

    int semID = semget(key, 0, IPC_EXCL);
    printf("sem ID:%d\n", semID);
    if (semID < 0)
    {
        printf("获取信号量集失败\n");
        return -1;
    }

    union semun semun;
    unsigned short semVarAry[NUMBER_SEM] = {1, 0};
    semun.array = semVarAry;

    int i;
    for (i = 0; i < NUMBER_SEM; i++)
    {
        printf("Sem no %d:value will be %d\n", i, semVarAry[i]);
    }

    int retval = semctl(semID, 0, SETALL, semun);
    printf("Retval:%d\n", retval);
    if (retval == -1)
    {
        printf("信号量集设置失败\n");
        return -1;
    }
    printf("信号量集设置成功\n");

    return 0;
}
