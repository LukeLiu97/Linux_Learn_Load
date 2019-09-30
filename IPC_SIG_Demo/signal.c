#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <signal.h>

#include <unistd.h>

int identifyARG(int argc,char **argv);
int insatllSIG();
int killSIG(const char *strPID,const char *strSIG);
int alarmSIG();

int main(int argc,char **argv)
{	
	pid_t pid = getpid();
 	printf("pid:%d\n",pid);   
	switch(identifyARG(argc,argv))
    {
	case 0://安装
		return insatllSIG();
	case 1://alarm
		return alarmSIG();
	case 10://kill
		return killSIG(argv[2],argv[3]);
        default://错误
            return -1;
    }
	
	return 0;
}

int identifyARG(int argc,char **argv)
{
    	if(argc == 2)
	{
		if(strcmp(argv[1],"-i") == 0)
		{
			printf("安装信号\n");
			return 0;
		}
		else if(strcmp(argv[1],"-a") == 0)
		{
			printf("定时信号\n");
			return 1;
		}
		else
		{
			printf("指令错误\n");
		}
	}
	else if(argc == 4)
	{
		if(strcmp(argv[1],"-k") == 0)
		{
		printf("向pid:%s发送信号%s\n",argv[2],argv[3]);
		return 10;
		}
	}
	
	
	printf("参数列表错误");
	return -1;
}

void sighandlerFun(int arg)
{
	printf("Signal:%d\n",arg);
	sleep(1);

	return ;
}

int insatllSIG()
{

//	if(signal(SIGINT,SIG_DFL) == SIG_ERR)
	if(signal(SIGINT,sighandlerFun) == SIG_ERR)
	{
		printf("安装信号失败\n");
		return -1;
	}

	int i = 0;
	while(1)
	{
		printf("Working\n");
		sleep(1);
		if(i++ > 20)
		{
			break;
		}
	}

	return 0;
}

int killSIG(const char *strPID,const char *strSIG)
{
	pid_t pid = atoi(strPID);
	int sig = atoi(strSIG);

	if(kill(pid,sig) != 0)
	{
		printf("kill pid:%d sig:%d error\n",pid,sig);
		return -1;
	}
	printf("kill success\n");

	return 0;
}

int alarmSIG()
{
	//  默认动作结束进程
	if(signal(SIGALRM,sighandlerFun) == SIG_ERR)
	{
		printf("安装信号失败\n");
		return -1;
	}

	while(1)
	{
		printf("Working\n");
		alarm(1);
		printf("New Alarm 1 second\n");
		pause();
	}

	return 0;
}
