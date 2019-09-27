#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	printf("===========\n");
	printf("process test\n");

	int m = 0,n = 0;	

	pid_t pid = fork();

	if(pid == 0)
	{
		while(1)
		{	
			sleep(1);
			printf("Child process PID:%d PPID:%d m:%d n:%d\n",
				getpid(),
				getppid(),
				m++,
				n);
			if(m > 10) break;
			/* 不做处理，在父进程结束前结束，成为僵尸进程*/
		}
		
		/* 主动结束进程，避免成为僵尸进程 */
		printf("SubProcess exti\n");	
		printf("++++++++++++++++\n");
		exit(0);
		
	}
	else if(pid > 0)
	{
		while(1)
		{
			sleep(1);
			printf("Parent process PID:%d PPID:%d m:%d n:%d\n",
					getpid(),
					getppid(),
					m,
					n++);

			if(n > 10) break;
			/* 不做处理，在子进程结束前结束，子进程成为孤儿进程 */
			else if(n >= 5)
			{
				/* 使用堵塞方式等待子进程结束 */
				pid_t retPID  = 0;
				int retStatus = 0;
				retPID = wait(retStatus);
				printf("retPID:%d status:%d\n",retPID,retStatus);
			}
		}
	}
	else
	{
		printf("Create fail\n");
		return -1;
	}
	
	printf("PID %d is killed\n",getpid());
	printf("------------\n");
	return 0;
}

