#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int identifyArgument(int argc, char **argv);
int serverTask(char **argv);
int clientTask(char **argv);

int main(int argc, char **argv)
{
    switch (identifyArgument(argc, argv))
    {
    case 0:
        return serverTask(argv);
    case 1:
        return clientTask(argv);
    default:
        return -1;
    }
}

int identifyArgument(int argc, char **argv)
{
    if (argc == 3)
    {
        if (strcmp(argv[1], "-s") == 0)
        {
            return 0;
        }
        else if (strcmp(argv[1], "-c") == 0)
        {
            return 1;
        }
        else
        {
            printf("参数不匹配\n");
        }
    }
    else
    {
        printf("参数数量错误\n");
    }
    return -1;
}

#define PORT 10000
#define IP_ADDR "192.168.10.81"
#define QUEUE_LEN 5

int serverTask(char **argv)
{
    printf("Server Task\n");

    int port = atoi(argv[2]);
    printf("Port:%d\n", port);

    /* 建立套接字 */
    int sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD < 0)
    {
        printf("创建套接字失败\n");
        return -1;
    }

    /* 关联端口和IP */
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(PORT);
    sockAddr.sin_addr.s_addr = inet_addr(IP_ADDR);
    int addrLen = sizeof(sockAddr);

    if (port > 1023)
    {
        sockAddr.sin_port = htons(port);
    }

    if (bind(sockFD, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) != 0)
    {
        printf("关联失败\n");
        return -1;
    }

    /* 监听请求 */
    if (listen(sockFD, QUEUE_LEN) != 0)
    {
        printf("无法监听\n");
    }
    printf("监听中...\n");

    int linkFD;
    char cBuff[1024];
    ssize_t recvSize;
    while (1)
    {
        /* 处理请求 */
        linkFD = accept(sockFD, (struct sockaddr *)&sockAddr, &addrLen);
        if (linkFD < 0)
        {
            printf("无法处理\n");
        }

        /* 接收数据 */
        memset(cBuff, 0, sizeof(cBuff));
        recvSize = recv(linkFD, cBuff, sizeof(cBuff) - 1, 0);
        printf("cBuff len:%d\n", recvSize);
        printf("Data:%s\n", cBuff);

        close(linkFD);
    }
    close(sockFD);

    return 0;
}

int clientTask(char **argv)
{
    printf("Server Task\n");

    int port = atoi(argv[2]);
    printf("Port:%d\n", port);

    /* 建立套接字 */
    int sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD < 0)
    {
        printf("创建套接字失败\n");
        return -1;
    }

    /* 关联端口和IP */
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(PORT);
    sockAddr.sin_addr.s_addr = inet_addr(IP_ADDR);
    int addrLen = sizeof(sockAddr);

    if (port > 1023)
    {
        sockAddr.sin_port = htons(port);
    }

    int retval;
    char cBuff[1024];
    ssize_t sendSize;
    /* 连接请求 */
    retval = connect(sockFD, (struct sockaddr *)&sockAddr, addrLen);
    if (retval < 0)
    {
        printf("无法连接\n");
        return -1;
    }
    /* 发送数据 */
    strcpy(cBuff, "hello");
    sendSize = send(sockFD, cBuff, strlen(cBuff), 0);
    printf("cBuff len:%d\n", sendSize);
    printf("Data:%s\n", cBuff);

    close(sockFD);

    return 0;
}
