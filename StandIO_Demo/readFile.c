#include <stdio.h>

int main()
{
	char strBuff[1024];	
	FILE * fp = fopen("./test.txt","w+");
	
	if(fp == NULL)
	{
		printf("打开文件失败\n");
		return -1;
	}

	int flag;
	int count = 0;
	while(1)
	{
		count++;
		flag = fputc('1',fp);
		if(flag == EOF)
		{
			printf("写入失败\n");\
			return -1;
		}

		flag = fgetc(fp);
		if(flag == EOF)
		{
			rewind(fp);
			fgets(strBuff,1024,fp);
			printf("%s",strBuff);	
			break;
		}
		printf("%d\n",count);
	}

	printf("缓冲刷新\n");
	return 0;
}
