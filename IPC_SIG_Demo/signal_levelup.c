#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <signal.h>

#include <unistd.h>

void sigactionFun(int arg,siginfo_t *info,void *argv);
int identifyARG(int argc,char **argv);
int insatllSIG();
int killSIG(const char *strPID,const char *strSIG);
int sendSIG(const char *strPID,const char *strSIG);

int main(int argc,char **argv)
{	
	pid_t pid = getpid();
 	printf("pid:%d\n",pid);   
	switch(identifyARG(argc,argv))
    {
	case 0://安装
		return insatllSIG();
	case 10://kill
		return killSIG(argv[2],argv[3]);
	case 11://Send queue
		return sendSIG(argv[2],argv[3]);
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
		else if(strcmp(argv[1],"-s") == 0)
		{
		printf("向pid:%s发送信号%s\n",argv[2],argv[3]);
		return 11;
		}
	}
	
	
	printf("参数列表错误");
	return -1;
}

void sigactionFun(int arg,siginfo_t *info,void *argv)
{
	printf("Signal:%d\n",arg);
	printf("Send PID:0x%08x\n",info->si_pid);
	printf("Send value%d\n",info->si_int);
	sleep(1);

	return ;
}

int insatllSIG()
{
	struct sigaction sigStu;
	memset(&sigStu,0,sizeof(sigStu));

	sigStu.sa_sigaction = sigactionFun;
	sigStu.sa_flags = SA_SIGINFO;

	if(sigaction(SIGINT,&sigStu,NULL) != 0)
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
int sendSIG(const char *strPID,const char *strSIG)
{
	pid_t pid = atoi(strPID);                            	
	int sig = atoi(strSIG);
	union sigval value;
	value.sival_int = 555;

	if(sigqueue(pid,sig,value) != 0)
	{
		printf("Signal queue pid:%d sig:%d error\n",pid,sig);
		return -1;
	}
	printf("Sigqueue int 555 success\n");

	return 0;
}
