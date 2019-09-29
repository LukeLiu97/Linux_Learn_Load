#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PROJECT_ID 1
#define NUMBER_SEM 2

int identifyARG(int argc,char **argv);
int createSem(const char *pathName,int projectID);
int deleteSem(const char *pathName,int projectID);
int getSem(const char *pathName,int projectID);
int setSem(const char *pathName,int projectID);

int main(int argc,char **argv)
{
    switch(identifyARG(argc,argv))
    {
        case 0://创建&初始化信号量集
		return createSem(argv[1],PROJECT_ID);	
        case 1://设置
		return setSem(argv[1],PROJECT_ID);
        case 2://获取
		return getSem(argv[1],PROJECT_ID);;
        case 3://删除
		return deleteSem(argv[1],PROJECT_ID);
        default://错误
            return -1;
    }
	
	return 0;
}

int identifyARG(int argc,char **argv)
{
    if(argc != 3)
    {
        printf("参数列表数量错误\n");
        return -1;
    }else
    {
        if(strcmp(argv[2],"-c") == 0)
        {
            printf("信号量集创建模式\n");
            return 0;
        }
        else if(strcmp(argv[2],"-s") == 0)
        {
            printf("信号量集设置模式\n");
            return 1;
        }
        else if (strcmp(argv[2], "-g") == 0)
        {
            printf("信号量集获取模式\n");
            return 2;
        }
        else if (strcmp(argv[2], "-d") == 0)
        {
            printf("信号量集删除模式\n");
            return 3;
        }
        else
        {
            printf("指令不存在\n");
            return -1;
        }
    }
}

int createSem(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:%d\n",key);
	if(key < 0)
	{
		printf("文件路径错误\n");
		return -1;
	}

	int semID = semget(key,NUMBER_SEM,IPC_CREAT|IPC_EXCL);
	printf("sem ID:%d\n",semID);
	if(semID < 0)
	{
		printf("创建信号量集失败\n");
		return -1;
	}

	/* 首次操作信号量集即初始化，不可为负值 */	
	struct sembuf buf[NUMBER_SEM] = {
		{0,0,0},
		{1,0,0}};
	int retval = semop(
		semID,
		buf,
		sizeof(buf)/sizeof(struct sembuf));

	if(retval != 0)
	{
		printf("初始化信号量集失败\n");
		return -1;
	}
	printf("初始化信号量集成功\n");
	
	return 0;
}

int deleteSem(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:%d\n",key);
	if(key < 0)
	{
		printf("文件路径错误\n");
		return -1;
	}

	int semID = semget(key,0,IPC_EXCL);
	printf("sem ID:%d\n",semID);
	if(semID < 0)
	{
		printf("获取信号量集失败\n");
		return -1;
	}
	
	int retval = semctl(semID,0,IPC_RMID);
	if(retval == -1)
	{
		printf("删除信号量集失败\n");
		return -1;
	}
	printf("删除信号量集成功，将唤醒被阻塞进程,retval:%d\n",retval);

	return 0;
}

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
				    (Linux-specific) */
};

int getSem(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:%d\n",key);
	if(key < 0)
	{
		printf("文件路径错误\n");
		return -1;
	}
	
	int semID = semget(key,0,IPC_EXCL);
	printf("sem ID:%d\n",semID);
	if(semID < 0)
	{
		printf("获取信号量集失败\n");
		return -1;
	}
	
	union semun semun;
	unsigned short semVarAry[NUMBER_SEM]= {0};
	semun.array = semVarAry;

	int retval = semctl(semID,0,GETALL,semun);
	printf("Retval:%d\n",retval);
	if(retval == -1)
	{
		printf("信号量集获取失败\n");
		return -1;
	}
	printf("信号量集获取成功\n");

	int i;
	for(i = 0;i < NUMBER_SEM;i++)
	{
		printf("Sem no %d:value = %d\n",i,semVarAry[i]);
	}

	return 0;
}

int setSem(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:%d\n",key);
	if(key < 0)
	{
		printf("文件路径错误\n");
		return -1;
	}
	
	int semID = semget(key,0,IPC_EXCL);
	printf("sem ID:%d\n",semID);
	if(semID < 0)
	{
		printf("获取信号量集失败\n");
		return -1;
	}
	
	union semun semun;
	unsigned short semVarAry[NUMBER_SEM]= {1,0};
	semun.array = semVarAry;

	int i;
        for(i = 0;i < NUMBER_SEM;i++)
        {
        	printf("Sem no %d:value will be %d\n",i,semVarAry[i]);
        }

	int retval = semctl(semID,0,SETALL,semun);
	printf("Retval:%d\n",retval);
	if(retval == -1)
	{
		printf("信号量集设置失败\n");
		return -1;
	}
	printf("信号量集设置成功\n");

	return 0;
}
