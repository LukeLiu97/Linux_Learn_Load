#include <stdio.h>
#include <string.h>

int main()
{
	FILE *fp = popen("ls -al","r");

	if(fp == NULL)
	{
		printf("标准流管道调用失败\n");
		return -1;
	}	

	char cBuff[1025];
	char *retval;

	while(1)
	{
		memset(cBuff,1,sizeof(cBuff)-1);
		
		retval = fgets(cBuff,sizeof(cBuff)-1,fp);
	
		printf("retval:");
		if(retval != NULL)
		{
			printf("%s",cBuff);
		}	
		else
		{
			printf("NULL\n");
			break;
		}
	}


	return 0;
}
