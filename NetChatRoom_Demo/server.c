/**
******************************************************************************
  * @file       server.c
  * @brief      
  * @version    0.1.0
  * @date       Oct-14-2019 Mon
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "server.h"

/** @addtogroup Server
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

char messageBuff[5][1024];

int taskServer(char *strIP, int iPort)
{
    int flag = 0;

    /* 建立监听套接字 */
    int listFD = socket(AF_INET, SOCK_STREAM, 0);
    if (listFD < 0)
    {
        printf("无法创建套接字\n");
        return -1;
    }

    /* 绑定IP&Port */
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(iPort);
    sockAddr.sin_addr.s_addr = inet_addr(strIP);
    socklen_t addrLen = sizeof(sockAddr);

    if (bind(listFD, (struct sockaddr *)&sockAddr, addrLen) != 0)
    {
        printf("无法绑定IP&Port\n");
        close(listFD);
        return -1;
    }

    /* 监听客户端请求 */
    if (listen(listFD, QUEUE_LEN) != 0)
    {
        printf("无法监听\n");
        close(listFD);
        return -1;
    }

    /* 初始话文件描述符集 */
    fd_set readFDS;
    FD_ZERO(&readFDS);
    FD_SET(listFD, &readFDS);

    int sockArray[QUEUE_LEN + 1] = {0};
    int tempSockArray[QUEUE_LEN + 1] = {0};
    int lenSA = 0;
    int maxFD = 0;

    sockArray[0] = listFD;
    lenSA++;
    maxFD = listFD;

    int sockFD;
    char recvBuff[1024] = {0};

    int i, j,k;
    int errorCount = 0;
    ssize_t recvCount;
	ssize_t sendCount;


    while (1)
    {

        /* 查看文件描述符状态 */
        if (select(maxFD + 1, &readFDS, NULL, NULL, NULL) <= 0)
        {
            continue;
        }

        /* 选择是否处理请求 */
        if (FD_ISSET(listFD, &readFDS))
        {

            sockFD = accept(listFD,
                            (struct sockaddr *)&sockAddr,
                            &addrLen);

            if (sockFD < 0)
            {
                printf("无法处理请求\n");
            }
            else
            {
                printf("Client %d connect\n", sockFD);
                /* 处理套接字数组 */
                if (lenSA < QUEUE_LEN)
                {
                    if (sockFD > maxFD)
                    {
                        maxFD = sockFD;
                    }
                    sockArray[lenSA] = sockFD;
                    FD_SET(sockArray[lenSA], &readFDS);
                    lenSA++;
                }
                else
                {
                    close(sockFD);
                }
            }
        }
        else
        {
            printf("监听中...\n");
        }

	k = 0;
        /* 处理对应客户端通信 */
        for (i = 1; i < lenSA; i++)
        {
            printf("%d ", i);
            printf("%d\n", sockArray[i]);
            if (FD_ISSET(sockArray[i], &readFDS))
            {
		memset(recvBuff,0,sizeof(recvBuff));
                recvCount = recv(sockArray[i],
                                 recvBuff,
                                 sizeof(recvBuff) - 1,
                                 0);

                printf("Client %d ", sockArray[i]);
                switch (recvCount)
                {
                case 0:
                    printf("超时\n");
                    close(sockArray[i]);
                    sockArray[i] = -1;
                    errorCount++;
                    break;
                case -1:
                    printf("无法接收\n");
                    close(sockArray[i]);
                    sockArray[i] = -1;
                    errorCount++;
                    break;
                default:
                    printf("Message:%s\n", recvBuff);
			strcpy(messageBuff[i++],recvBuff);
                }
            }
        }

        /* 断连客户端处理 */
        if (errorCount != 0)
        {
            j = 0;
            maxFD = listFD;
            memset(tempSockArray, 0, sizeof(tempSockArray));
            for (i = 0; i < lenSA; i++)
            {
                if (sockArray[i] != -1)
                    tempSockArray[j++] = sockArray[i];
            }
            for (i = 0; i < sizeof(sockArray); i++)
            {
                sockArray[i] = tempSockArray[i];
                if (sockArray[i] > maxFD)
                    maxFD = sockArray[i];
            }
            lenSA = lenSA - errorCount;
            errorCount = 0;
        }

	
	for(i = 0;i < k;i++)
	{
		for(j = 1;j<lenSA ;j++)
		{
			sendCount = send(sockArray[j],messageBuff[i],sizeof(messageBuff[i]),0);
			printf("Client %d sendCount%d\n",sockArray[j],sendCount);
		}
		memset(messageBuff,0,sizeof(messageBuff));
	}

	k = 0;

        /* 恢复对套接字的监视 */
        for (i = 0; i < lenSA; i++)
        {
            FD_SET(sockArray[i], &readFDS);
        }

    }

    close(listFD);

    return 0;
}

/**
  * @}
  */



