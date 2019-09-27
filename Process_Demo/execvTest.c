#include <stdio.h>
#include <unistd.h>

char * argv[] = {"./pid",NULL};

int main()
{
	printf("EXECV task,PID:%d\n",getpid());
	
	execv("./pid",argv);

	printf("++++++++++");	
	return 0;
}
