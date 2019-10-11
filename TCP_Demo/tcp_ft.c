#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int identifyArgument(int argc, char **argv);
int serverTask(char **argv);
int clientTask(char **argv);
int sendFile(int sockFD);
int recvFile(int sockFD);

typedef struct FileStu_
{
	char fileName[256];
	off_t fileSize;
}FileStu;


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
	else if(argc == 4)
	{
if (strcmp(argv[1], "-s") == 0)
{
    return 0;
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

    int linkFD;
    char cBuff[1024];
    ssize_t recvSize;
    while (1)
    {
    	printf("监听中...\n");
        /* 处理请求 */
        linkFD = accept(sockFD, (struct sockaddr *)&sockAddr, &addrLen);
        if (linkFD < 0)
        {
            printf("无法处理\n");
        }

	sendFile(linkFD);

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

    recvFile(sockFD);

    close(sockFD);

    return 0;
}

int sendFile(int sockFD)
{
	printf("需要发送的文件：");	
	char pathName[256] = {0};
	scanf("%s",pathName);

	int fd = open(pathName,O_RDONLY);
	if(fd < 0)
	{
		printf("无法打开文件\n");	
		return -1;
	}

FileStu oriFile;
memset(&oriFile,0,sizeof(oriFile));
                                    
	/* 分离文件名 */
	int place = strlen(pathName);
	while(place)
	{
		if(pathName[place-1] == '/')
		{	
			break;
		}
		else
		{
			place--;
		}
	}

	strcpy(oriFile.fileName,&pathName[place]);

	oriFile.fileSize = lseek(fd,0,SEEK_END);
	printf("FileSize:%d\n",oriFile.fileSize);	
	lseek(fd,0,SEEK_SET);

	char cBuffer[1025] = {0};
	ssize_t readLength;
    	ssize_t sendSize;

    	sendSize = send(sockFD, &oriFile, sizeof(oriFile), 0);

	while(1)
	{
		memset(cBuffer,0,sizeof(cBuffer));
		readLength = read(fd,cBuffer,sizeof(cBuffer)-1);
		printf("Read length:%d\n",readLength);
		if(readLength == -1)
		{
			printf("Read File error\n");
			break;
		}
		else if(readLength == 0)
		{	
			printf("Read EOF\n");
			break;
		}

    		sendSize = send(sockFD, cBuffer, readLength, 0);
	}	

	close(fd);	

	return 0;
}

int recvFile(int sockFD)
{
ssize_t recvLength;
char cBuffer[1025] = {0};

	FileStu objFile;

	recvLength = recv(sockFD,&objFile,sizeof(objFile),0);
	if(recvLength == -1)
	{
		printf("无法接收\n");
		return -1;
	}
	else if(recvLength == 0)
	{
		printf("超时\n");
		return 0;
	}
	
	printf("FileName:%s FileSize:%d\n",objFile.fileName,objFile.fileSize);	

	char pathName[256] = {0};
	sprintf(pathName,"./download/%s",objFile.fileName);

	int fd = open(pathName,O_WRONLY|O_CREAT|O_EXCL,0666);
	if(fd < 0)                              		
	{
		printf("无法创建文件\n");	
		return -1;
	}
	ssize_t count = 0;

	while(1)
	{
	memset(cBuffer,0,sizeof(cBuffer));
	recvLength = recv(sockFD,&cBuffer,sizeof(cBuffer)-1,0);
	if(recvLength == -1)
	{
		printf("无法接收\n");
		break;
	}
	else if(recvLength == 0)
	{
		printf("超时\n");
		break;
	}
		write(fd,cBuffer,recvLength);
		count += recvLength;
		printf("Recv Length%d ,%d\%\n",recvLength,100*count/objFile.fileSize);
	}
	close(fd);
	
	return 0;
}
