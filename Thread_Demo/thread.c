#include <pthread.h>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int thread();
void *routineFun(void *arg);

int main()
{
	printf("Thread Demo\n");
	printf("PID:%d\n",getpid());
	return thread();
}


int thread()
{
	printf("Thread ID:%0x\n",pthread_self());

	pthread_t threadID;
	if(pthread_create(&threadID,NULL,routineFun,NULL) != 0)
	{
		printf("Create thread error\n");
		return -1;
	}
	printf("Return thread ID is %0x\n",threadID);

	int i=5;
	while(i--)
	{
		printf("Thread ID:%0x\n",pthread_self());
		sleep(1);
	}

	pthread_join(threadID,NULL);
	pthread_cancel(pthread_self());

	return 0;
	
}

void *routineFun(void *arg)
{
	int i = 20;
	while(i--)
	{
		printf("Thread ID:%0x\n",pthread_self());
		sleep(1);
	}
	pthread_exit(NULL);

}
