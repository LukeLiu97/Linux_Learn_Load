#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int checkCommand(char **argv);
int sendFIFO(char *filename_FIFOPipe);
int recvFIFO(char *filename_FIFOPipe);
int fork_and_FIFO(char *filename_FIFOPipe);

int main(int argc,char **argv)
{
	printf("FIFO pipe Test\n");
	printf("===========================\n");

	if(argc != 3)
	{
		printf("命令参数数量错误\n");
		return -1;
	}

	switch(checkCommand(argv))
	{
		case 0://输入参数为"-w",管道只写	
			return sendFIFO(argv[1]);
		case 1://输入参数为"-w",管道只读
			return recvFIFO(argv[1]);
		case 2://输入参数为"-fork",利用单一管道在父子进程间收发
			return fork_and_FIFO(argv[1]);
		defalut:
			printf("参数错误");
			return -1;
	}

}

int checkCommand(char **argv)
{
	if(strcmp(argv[2],"-r") == 0)
	{
		return 0;
	}
	else if(strcmp(argv[2],"-w") == 0)
	{
		return 1;
	}
	else if(strcmp(argv[2],"-fork") == 0)
	{
		return 2;
	}
	else
	{
		return -1;
	}
}

int sendFIFO(char *filename_FIFOPipe)
{
	int fd = open(filename_FIFOPipe,O_WRONLY);

	char cBuffer[257] = {"A message"};

	if(fd == -1)
	{
		printf("无法打开管道文件\n");
		return -1;
	}

	printf("成功打开管道文件\n");
	write(fd,cBuffer,strlen(cBuffer));

	return 0;
}

int recvFIFO(char *filename_FIFOPipe)
{
	int fd = open(filename_FIFOPipe,O_RDONLY);

	char cBuffer[257] = {0};

	if(fd == -1)
	{
		printf("无法打开管道文件\n");
		return -1;
	}

	printf("成功打开管道文件\n");
	read(fd,cBuffer,sizeof(cBuffer)-1);
	printf("receive data:%s\n",cBuffer);

	return 0;
}

int fork_and_FIFO(char *filename_FIFOPipe)
{
	int fd = open(filename_FIFOPipe,O_RDWR);

	if(fd == -1)
        {
        	printf("无法打开管道文件\n");
        	return -1;
        }

	char cBuff[257] = {0};
	pid_t ret_fork = fork();
	if(ret_fork > 0)// Paretent process
	{
		printf(
			"Parent process PID:%d PPID:%d\n",
			getpid(),
			getppid());

		write(
			fd,
			"Hello subProcess\n",
			strlen("Hello subProcess\n"));

		sleep(2);		

		read(
			fd,
			cBuff,
			sizeof(cBuff)-1);
		printf("Parent process: Pipe message:%s",cBuff);

		wait(NULL);
		printf("------------------------\n");
	}
	else if(ret_fork == 0)// Child process
	{
		printf(
			"Child process PID:%d PPID:%d\n",
			getpid(),
			getppid());


		read(
			fd,
			cBuff,
			sizeof(cBuff)-1);
		printf("Child process: Pipe message:%s",cBuff);

		write(
			fd,
			"Hello mainProcess\n",
			strlen("Hello mainProcess\n"));


		/* 主动结束进程，避免成为僵尸进程 */
		printf("SubProcess exti\n");	
		printf("++++++++++++++++++++++++\n");

		exit(0);
	}                                                          
       	else// Fork process error
	{
		printf("Fork process fail\n");
		return -1;
	}

	return 0;
}

