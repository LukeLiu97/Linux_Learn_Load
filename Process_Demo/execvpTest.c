#include <stdio.h>
#include <unistd.h>

char * argv[] = {"sh","-c","./pid",NULL};

int main()
{
	printf("EXECVP task,PID:%d\n",getpid());
	
	execvp("sh",argv);

	printf("++++++++++");	
	return 0;
}
