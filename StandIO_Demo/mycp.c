#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		printf("命令错误\n");
		return -1;
	}

	FILE *oriFile = fopen(argv[1],"r");
	if(oriFile == NULL)
	{
		printf("原文件不存在\n");
		return -1;
	}

	FILE * objFile = NULL;
	
	char chTemp = 0;
	
	objFile = fopen(argv[2],"r");
        if(objFile != NULL)
        {
        	printf("目标文件已存在\n");	
		fclose(objFile);

		printf("是否进行覆盖(y/n):");
		scanf("%c",&chTemp);
		if(chTemp == 'y')
		{
			printf("将覆盖文件\n");
		}
		else if(chTemp == 'n') 
		{
			printf("程序退出\n");
			fclose(oriFile);
        		return 0;
		}
		else
		{
			printf("输入错误\n");
			fclose(oriFile);
			return -1;
		}
        }	

	objFile = fopen(argv[2],"w");
	if(objFile == NULL)
	{
		printf("目标文件打开失败\n");		
        	fclose(oriFile);
        	return -1;
	}
	
	size_t count = 0;
	size_t count_f = 0;
	size_t retval;
	char cBuffer[1024];
	char ch;
	while(1)
	{
		memset(cBuffer,0,sizeof(cBuffer));
		retval = fread(cBuffer,sizeof(cBuffer),1,oriFile);
		if(retval == 0)
		{
			printf("文件剩余不足1kB\n");
			fseek(oriFile,count*sizeof(cBuffer),SEEK_SET);
			
			ch = fgetc(oriFile);
			while(feof(oriFile) != 1)
			{
				count_f += 1;
				fputc(ch,objFile);	
				ch = fgetc(oriFile);
			};
                           		
			printf("读取结束\n");
			break;

/*
			fwrite(cBuffer,1,strlen(cBuffer),objFile);
			break;
*/
		}
		else
		{
			count += retval;
		}
		retval = fwrite(cBuffer,sizeof(cBuffer),1,objFile);
		
	}
	
	printf("写入结束\n");

	double filesize = (count_f + 3)/1024.0 + count;
	printf("file size = %0.2fkB\n",filesize);

	fclose(objFile);
	fclose(oriFile);
	return 0;
}

