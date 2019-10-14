#include <stdio.h>
#include "global.h"
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "server.h"

int identifyARG(int argc,char **argv);
int identifySerOrCli(char *str);

int main(int argc,char **argv)
{
    int retval;

    switch(identifyARG(argc,argv))
    {
        case 0:
            printf("Server Task\n");
		retval = taskServer(strIP,iPort);
            break;
        case 1:
            printf("Client Task\n");
            retval = taskClient(strIP,iPort);
            break;
        default:
            printf("Command Error\n");
            retval = -1;
    }

    return retval;
}

int identifyARG(int argc,char **argv)
{
    int retval;

    switch(argc)
    {
        case 2:
            printf("使用默认地址与端口\n");
            retval = identifySerOrCli(argv[1]);
            break;
        case 3:
            iPort = atoi(argv[2]);
            retval = identifySerOrCli(argv[1]);
            break;
        default:
            printf("参数数量错误\n");
            retval = -1;
    }

    printf("IP address:%s Port:%d\n",strIP,iPort);

    return retval;
}

int identifySerOrCli(char *str)
{
    if(str[0] == '-')
    {
        switch(str[1])
        {
            case 's':
                return 0;
            case 'c':
                return 1;
        }
    }

    return -1;
}
