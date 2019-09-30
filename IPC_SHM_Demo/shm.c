#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PROJ_ID 1
#define SHM_SIZE 256

int identifyARG(int argc,char **argv);
int createSHM(const char *pathName,int projectID);
int deleteSHM(const char *pathName,int projectID);
int writeSHM(const char *pathName,int projectID);
int readSHM(const char *pathName,int projectID);

int main(int argc,char **argv)
{
    switch(identifyARG(argc,argv))
    {
        case 0://创建
		return createSHM(argv[1],PROJ_ID);
        case 1://发送
		return readSHM(argv[1],PROJ_ID);
        case 2://写入
		return writeSHM(argv[1],PROJ_ID);
        case 3://删除
		return deleteSHM(argv[1],PROJ_ID);
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
        if(strcmp(argv[2],"-n") == 0)
        {
            printf("共享内存创建模式\n");
            return 0;
        }
        else if(strcmp(argv[2],"-r") == 0)
        {
            printf("共享内存读取模式\n");
            return 1;
        }
        else if (strcmp(argv[2], "-w") == 0)
        {
            printf("共享内存写入模式\n");
            return 2;
        }
        else if (strcmp(argv[2], "-d") == 0)
        {
            printf("共享内存删除模式\n");
            return 3;
        }
        else
        {
            printf("指令不存在\n");
            return -1;
        }
    }
}

int createSHM(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:0x%08x\n",key);
	if(key < 0)
	{
		printf(" 获取键值失败\n");
		return -1;
	}

	int shmID = shmget(key,SHM_SIZE,IPC_CREAT);
	printf("shm ID:%d\n",shmID);
	if(shmID < 0)
	{
		printf("创建/获取共享内存失败\n");
		return -1;
	}

	return 0;
}

int deleteSHM(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:0x%08x\n",key);
	if(key < 0)
	{
		printf("获取键值失败\n");
		return -1;
	}

	int shmID = shmget(key,SHM_SIZE,IPC_EXCL);
	printf("shm ID:%d\n",shmID);
	if(shmID < 0)
	{
		printf("获取共享内存失败\n");
		return -1;
	}

	int retval = shmctl(shmID,IPC_RMID,NULL);
	if(retval < -1)
	{
		printf("删除共享内存失败\n");
	}
	printf("删除共享内存成功\n");

	return retval;
}

int writeSHM(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:0x%08x\n",key);
	if(key < 0)
	{
		printf("获取键值失败\n");
		return -1;
	}

	int shmID = shmget(key,SHM_SIZE,IPC_EXCL);
	printf("shm ID:%d\n",shmID);
	if(shmID < 0)
	{
		printf("获取共享内存失败\n");
		return -1;
	}

	char *p = (char *)shmat(shmID,NULL,SHM_W);
	if(p == (void *)-1)
	{
		printf("写映射内存失败\n");
	}
	printf("写映射内存成功\n");
	
	strcpy(p,"Hello");

	int retval = shmdt(p);
	
	printf("retval:%d\n",retval);
	if(retval < 0)
	{
		printf("取消映射失败\n");
	}
	printf("取消映射成功\n");
	
	return retval;
}

int readSHM(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:0x%08x\n",key);
	if(key < 0)
	{
		printf("获取键值失败\n");
		return -1;
	}

	int shmID = shmget(key,SHM_SIZE,IPC_EXCL);
	printf("shm ID:%d\n",shmID);
	if(shmID < 0)
	{
		printf("获取共享内存失败\n");
		return -1;
	}

	char *p = (char *)shmat(shmID,NULL,SHM_RDONLY);
	if(p == (void *)-1)
	{
		printf("只读映射内存失败\n");
	}
	printf("只读映射内存成功\n");

	printf("SHM:%s\n",p);
	
	int retval = shmdt(p);
	
	printf("retval:%d\n",retval);
	if(retval < 0)
	{
		printf("取消映射失败\n");
	}
	printf("取消映射成功\n");
	
	return retval;
}
