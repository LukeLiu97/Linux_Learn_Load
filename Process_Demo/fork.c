#include <stdio.h>
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
			sleep(2);
			printf("Child process PID:%d PPID:%d m:%d n:%d\n",
				getpid(),
				getppid(),
				m++,
				n);
			//if(m > 10) break;//成为僵尸进程
		}
	}
	else if(pid > 0)
	{
		while(1)
		{
			sleep(2);
			printf("Parent process PID:%d PPID:%d m:%d n:%d\n",
					getpid(),
					getppid(),
					m,
					n++);
			//if(n > 10) break;//子进程成为孤儿进程
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

