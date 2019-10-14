/*
******************************************************************************
  * @file       client.c
  * @brief      
  * @version    0.1.0
  * @date       Oct-14-2019 Mon
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "client.h"

/** @addtogroup Client
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
//typedef struct CliMessageType_
//{

//}CliMessageType;


/* Private define ------------------------------------------------------------*/
#define THREAD_NUM 4

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int flag = 0;

int sockFD;
struct sockaddr_in sockAddr;
socklen_t addrLen = sizeof(sockAddr);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int initTCPSock(char *strIP,int iPort)
{
    /* 建立套接字 */
    int sockFD = socket(AF_INET,SOCK_STREAM,0);
    if(sockFD < 0)
    {
        printf("无法创建套接字\n");
        return -1;
    }

    memset(&sockAddr,0,sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(iPort);
    sockAddr.sin_addr.s_addr = inet_addr(strIP);

    if(connect(sockFD,(struct sockaddr *)&sockAddr,addrLen) < 0)
    {
        printf("无法连接\n");
        close(sockFD);
        return -1;
    }

    return sockFD;
}

void *recvThreadFun(void *arg)
{
    


    pthread_exit(NULL);
}

void *sendThreadFun(void *arg)
{

    
    pthread_exit(NULL);
}

void *scanfThreadFun(void *arg)
{
    char cBuff[1024] = {0};
    while(1)
    {
        if(flag != 0)
        {
            scanf("%256s",cBuff);
        }
        sleep(1);
    }

    pthread_exit(NULL);
}

void *displayThreadFun(void *arg)
{
    while(1)
    {
        flag = 0;
        //system("clear");
        printf("1.qazwsx\n");
        printf("2.edcrfv\n");
        printf("3.tgbyhn\n");
        printf("Your Message:\n");
        sleep(1);
        flag = 1;
    }

    pthread_exit(NULL);
}

/* Exported functions --------------------------------------------------------*/

int taskClient(char *strIP,int iPort)
{
    sockFD = initTCPSock(strIP,iPort);
    if(sockFD < 0)
    {
        printf("无法初始化套接字\n");
        return -1;
    }

    char cBuff[1024];
    ssize_t sendSize;

    /* 发送数据 */
    strcpy(cBuff, "hello");
    sendSize = send(sockFD, cBuff, strlen(cBuff), 0);
    printf("cBuff len:%d\n", sendSize);
    printf("Data:%s\n", cBuff);

    close(sockFD);

    pthread_t tid[THREAD_NUM];
    void *(*(threadFun[THREAD_NUM]))(void *) = 
        {
            recvThreadFun,
            sendThreadFun,
            scanfThreadFun,
            displayThreadFun
        };

    int i;
    for(i = 0;i < THREAD_NUM;i++)
    {
        if(pthread_create(&tid[i],NULL,threadFun[i],NULL) != 0)
        {
            printf("无法创建线程\n");
            return -1;
        }
        else
        {
            printf("创建线程%lu成功\n",tid[i]);
        }
        
    }

    for(i = 0;i<THREAD_NUM;i++)
    {
        if(pthread_join(tid[i],NULL) != 0)
        {
                printf("等待线程%lu退出失败\n",tid[i]);
        }
	else
	{
		printf("线程%lu已退出\n",tid[i]);
	}
    }
    return 0;
}

/**
  * @}
  */
