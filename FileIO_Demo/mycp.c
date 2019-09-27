#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int myCP(char *oriFile,char *objFile);
int myEcho(int oriFD,int objFD);

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		printf("命令参数错误\n");
		return -1;
	}
		
	int retval;
	
	retval = myCP(argv[1],argv[2]);
	
	return retval;
}

int myCP(char *oriFile,char *objFile)
{
	int oriFD,objFD;

	oriFD = open(oriFile,O_RDONLY);
	if(oriFD == -1)
	{
		printf("无法打开源文件\n");
		return -1;
	}	
	
	objFD = open(
		objFile,
		O_WRONLY|O_CREAT|O_EXCL,
		S_IRWXU|S_IRWXG|S_IRWXO);
	if(objFD == -1)
	{
		printf("无法创建目标文件\n");
		objFD = open(
			objFile,
			O_RDWR|O_EXCL);
		if(objFD == -1)
		{
			printf("无法打开文件\n");
			close(oriFD);
			return -1;
		}
		else
		{
			char chTemp;
			printf("已存在是否覆盖(y/n):");
			scanf("%c",&chTemp);
			if(chTemp == 'n')
			{
				close(objFD);
				close(oriFD);
				printf("文件未复制\n");
				return 0;
			}
			else if(chTemp == 'y')
			{
			}
			else
			{
				close(objFD);
				close(oriFD);
				printf("命令输入错误");
				return -1;
			}
		}
	}

	myEcho(oriFD,objFD);	

	close(oriFD);
	close(objFD);
	return 0;
}

int myEcho(int oriFD,int objFD)
{
	char cBuffer[1025];
	ssize_t oriSize;
	ssize_t objSize;
	while(1)
	{
		memset(cBuffer,1,sizeof(cBuffer));
		oriSize = read(oriFD,cBuffer,sizeof(cBuffer)-1);
		printf("oriSize:%d\n",oriSize);
		if(oriSize == -1)
		{
			return -1;
		}
		else if(oriSize == 0)
		{
			break;
		}

		objSize = write(objFD,cBuffer,oriSize);
		printf("objSize:%d\n",objSize);
		if(objSize == -1)
		{
			return -1;
		}
	}

	return 0;
}


