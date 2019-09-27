#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("SYSTEM task,PID:%d\n",getpid());
	
	system("./pid");
	
	printf("++++++++++++++++\n");
	
	return 0;
}
