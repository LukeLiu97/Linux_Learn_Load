#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("命令参数错误\n");
		return -1;
	}
	
	FILE *fp = fopen(argv[1],"r");
	if(fp == NULL)
	{
		printf("文件不存在\n");
		return -1;
	}

	char cBuffer[1025];
	int count = 0;
	
	char *strp= NULL;
	
	while(1)
	{
		memset(cBuffer,0,sizeof(cBuffer));
		strp = fgets(cBuffer,sizeof(cBuffer),fp);
		if(strp != NULL)
		{
			if(strlen(cBuffer) >= sizeof(cBuffer)-1)
			{
				printf("行过长\n");
			}
			else if(cBuffer[strlen(cBuffer)-1] == '\n')
			{
				printf("%s",strp);
				count++;
			}
			else
			{
				printf("不支持二进制文件\n");
				fclose(fp);
				return -1;
			}
		}
		if(feof(fp) == 1)
		{
			printf("读取结束,line count = %d\n",count);
			break;
		}
	}	

	return 0;
}
