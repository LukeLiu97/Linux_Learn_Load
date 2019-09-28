#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{	
	printf("IPC pip Test\n");
	printf("========================\n");

	printf("Process ID:%d\n",getpid());
	
	int pipefd[2];

	/* 创建无名管道 */
	int ret_pipe = pipe(pipefd);

	if(ret_pipe != 0)
	{
		printf("Create pipe fail,pipe retval:%d\n",ret_pipe);
		return -1;
	}

	int m = 0,n = 0;
	char cBuff[257] = {0};
	pid_t ret_fork = fork();
	if(ret_fork > 0)// Paretent process
	{
		printf("Parent process PID:%d PPID:%d m:%d n:%d\n",
				getpid(),
				getppid(),
				m,
				n++);

		close(pipefd[0]);
		write(pipefd[1],
			"Hello subProcess\n",
			strlen("Hello subProcess\n"));

		wait(NULL);
		printf("------------------------\n");
	}
	else if(ret_fork == 0)// Child process
	{
		printf("Child process PID:%d PPID:%d m:%d n:%d\n",
				getpid(),
				getppid(),
				m++,
				n);
		read(pipefd[0],
			cBuff,
			sizeof(cBuff)-1);
		printf("Child process: Pipe message:%s",cBuff);

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

	printf("IPC pip Test End \n");
	return 0;
}


