#include <pthread.h>
#include <time.h>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

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

	printf("Sleep ...\n");
	sleep(5);
	printf("Will unlock\n");
	pthread_cond_signal(&cond);
	
	pthread_join(threadID,NULL);

	return 0;
	
}

void *routineFun(void *arg)
{
	printf("Thread ID:%0x\n",pthread_self());

	printf("Wait ...\n");
	pthread_cond_wait(&cond,&mutex);
	printf("wait over\n");


	struct timespec ts;
	clock_gettime(CLOCK_REALTIME,&ts);
	ts.tv_sec += 5;

	printf("Wait 5 sec...\n");
	pthread_cond_timedwait(&cond,&mutex,&ts);
	printf("wait over\n");

	pthread_exit(NULL);
}
