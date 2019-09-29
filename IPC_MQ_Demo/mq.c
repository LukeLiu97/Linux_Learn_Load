#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PROJ_ID 1
#define LENGTH 257
#define MTYPE 1

typedef struct msgbuf{
	long mtype;
	char mtext[LENGTH];
}msgpack;

int identifyARG(int argc,char **argv);
int createMQ(const char *pathName,int projectID);
int deleteMQ(const char *pathName,int projectID);
int sendMQ(const char *pathName,int projectID);
int recvMQ(const char *pathName,int projectID);

int main(int argc,char **argv)
{
    switch(identifyARG(argc,argv))
    {
        case 0://创建
		return createMQ(argv[1],PROJ_ID);
        case 1://发送
		return sendMQ(argv[1],PROJ_ID);
        case 2://接受
		return recvMQ(argv[1],PROJ_ID);
        case 3://删除
		return deleteMQ(argv[1],PROJ_ID);
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
            printf("消息队列创建模式\n");
            return 0;
        }
        else if(strcmp(argv[2],"-s") == 0)
        {
            printf("消息队列发送模式\n");
            return 1;
        }
        else if (strcmp(argv[2], "-r") == 0)
        {
            printf("消息队列接收模式\n");
            return 2;
        }
        else if (strcmp(argv[2], "-d") == 0)
        {
            printf("消息队列删除模式\n");
            return 3;
        }
        else
        {
            printf("指令不存在\n");
            return -1;
        }
    }
}

int createMQ(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:%d\n",key);
	if(key < 0)
	{
		printf("文件路径错误\n");
		return -1;
	}

	int msgID = msgget(key,IPC_CREAT);
	printf("msg ID:%d\n",msgID);
	if(msgID < 0)
	{
		printf("创建消息队列失败\n");
		return -1;
	}

	return 0;
}

int deleteMQ(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:%d\n",key);
	if(key < 0)
	{
		printf("文件路径错误\n");
		return -1;
	}

	int msgID = msgget(key,IPC_EXCL);
	printf("msg ID:%d\n",msgID);
	if(msgID < 0)
	{
		printf("打开消息队列失败\n");
		return -1;
	}
	
	
	int retval = msgctl(msgID,IPC_RMID,NULL);
	if(retval != 0)
	{
		printf("删除消息队列失败\n");
		return -1;
	}
	printf("删除消息队列成功\n");

	return 0;
}

int sendMQ(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:%d\n",key);
	if(key < 0)
	{
		printf("文件路径错误\n");
		return -1;
	}

	
	int msgID = msgget(key,IPC_EXCL);
	printf("msg ID:%d\n",msgID);
	if(msgID < 0)
	{
		printf("打开消息队列失败\n");
		return -1;
	}
	
	msgpack message = {MTYPE,"Hello"};

	int retval = msgsnd(msgID,&message,sizeof(msgpack),0);
	printf("Send retval:%d\n",retval);
	if(retval < 0)
	{
		printf("消息发送失败\n");
		return -1;
	}
	printf("消息发送成功\n");

	return 0;
}

int recvMQ(const char *pathName,int projectID)
{
	key_t key = ftok(pathName,projectID);
	printf("key:%d\n",key);
	if(key < 0)
	{
		printf("文件路径错误\n");
		return -1;
	}

	
	int msgID = msgget(key,IPC_EXCL);
	printf("msg ID:%d\n",msgID);
	if(msgID < 0)
	{
		printf("打开消息队列失败\n");
		return -1;
	}
	
	msgpack message = {MTYPE,0};

	size_t sizeMsg = msgrcv(msgID,&message,sizeof(msgpack),MTYPE,0);
	printf("Message size:%d\n",sizeMsg);
	if(sizeMsg < 0)
	{
		printf("消息接收失败\n");
		return -1;
	}
	printf("Message:%s\n",message.mtext);
	printf("消息接受成功\n");

	return 0;
}
