#include <semaphore.h>
#include <pthread.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void *routineFunA(void *arg);
void *routineFunB(void *arg);

sem_t sem1;
sem_t sem2;

char cBuff[256] = {0};


int main()
{
	sem_init(&sem1,0,1);
	sem_init(&sem2,0,0);

	pthread_t pID_A;
	pthread_create(&pID_A,NULL,routineFunA,NULL);

	pthread_t pID_B;
	pthread_create(&pID_B,NULL,routineFunB,NULL);	
	
	pthread_join(pID_A,NULL);
	pthread_join(pID_B,NULL);
	
	return 0;
}

void *routineFunA(void *arg)
{
	while(1)
	{
		sem_wait(&sem1);

		sleep(1);
		printf("Pthread A\n");
		strcpy(cBuff,"A message");
		
		sem_post(&sem2);
	}
}
void *routineFunB(void *arg)
{
	while(1)
	{
		sem_wait(&sem2);
		
		sleep(1);
		printf("Pthread B\n");
		printf("%s\n",cBuff);
		memset(cBuff,0,sizeof(cBuff));
		printf("%s\n",cBuff);
			
		sem_post(&sem1);
	}
}
