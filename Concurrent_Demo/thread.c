#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_LEN 5

/* 默认IP&Port */
char strIP[] = "192.168.0.81";
int port = 12345;

int identifyARG(int argc,char **argv);
void *sockThreadFun(void *arg);

int main(int argc,char **argv)
{
	printf("Server Task\n");
	identifyARG(argc,argv);

	/* 建立监听套接字 */
	int listFD = socket(AF_INET,SOCK_STREAM,0);
	if(listFD < 0)
	{
		printf("无法创建套接字\n");
		return -1;
	}

	/* 绑定IP&Port */
	struct sockaddr_in sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(port);
	sockAddr.sin_addr.s_addr = inet_addr(strIP);
	socklen_t addrLen = sizeof(sockAddr);

	if(bind(listFD,(struct sockaddr *)&sockAddr,addrLen) != 0)
	{
		printf("无法绑定IP&Port\n");
		close(listFD);
		return -1;
	}
	
	/* 监听客户端请求 */
	if(listen(listFD,QUEUE_LEN) != 0)
	{
		printf("无法监听\n");
		close(listFD);
		return -1;
	}
	
	int sockFD;

	while(1)
	{
		printf("监听中\n");
		sockFD = accept(listFD,
				(struct sockaddr *)&sockAddr,
				&addrLen);

		printf("Sock FD:%d\n",sockFD);
		if(sockFD < 0)
		{
			printf("无法处理请求\n");
		}
		else
		{
			pthread_t tid;
			if(pthread_create(&tid,NULL,sockThreadFun,(void *)&sockFD)!=0)
			{
				printf("无法创建线程\n");
				close(sockFD);
			}
		}
	}

	close(listFD);

	return 0;
}

int identifyARG(int argc,char **argv)
{
	switch(argc)
	{
		case 1:
			printf("使用默认地址及端口\n");
			break;
		case 2:
			port = atoi(argv[1]);
			break;
		default:
			printf("参数数量错误\n");
	}
	printf("IP address:%s Port:%d\n",strIP,port);

	return 0;
}

void *sockThreadFun(void *arg)
{
	if(arg == NULL)
	{
		pthread_exit(NULL);
	}
	char msgBuff[1024];
	char sendMsgBuff[2048];
	ssize_t recvLen;
	
	int sockFD = *(int *)arg;

	while(1)
	{
		memset(msgBuff,0,sizeof(msgBuff));
		memset(sendMsgBuff,0,sizeof(sendMsgBuff));
		recvLen = recv(sockFD,msgBuff,sizeof(msgBuff)-1,0);
		if(recvLen == -1)
		{
			printf("Receive Error\n");
			close(sockFD);
			break;
		}
		else if(recvLen == 0)
		{
			printf("Receive timeout\n");			
			close(sockFD);
			break;
		}
		else
		{
			sprintf(sendMsgBuff,
				"TID:%lu Client:%d Message:%s\n",
				//gettid(),多线程中等效getpid()
				pthread_self(),
				sockFD,
				msgBuff);
			printf("%s\n",sendMsgBuff);
			send(sockFD,
				sendMsgBuff,
				strlen(sendMsgBuff)+1,
				0);
		}
	}
	pthread_exit(NULL);
}
