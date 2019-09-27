#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

int myCP(const char *oriFile,const char *objFile);
int myEcho(int oriFD,int objFD);
int myCPDir(const char *oriPath,const char * objPath);

int joinFilePath(char *emptyBuff,const char *strPath,const char *strFile);

int main(int argc,char **argv)
{
	if(argc != 3)
	{
		printf("命令参数错误\n");
		return -1;
	}
		
	int retval;	
//	retval = myCP(argv[1],argv[2]);
	
	retval = myCPDir(argv[1],argv[2]);

	return retval;
}

int myCP(const char *oriFile,const char *objFile)
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

int myCPDir(const char *oriPath,const char * objPath)
{
	DIR *oriDir = opendir(oriPath);	
	if(oriDir == NULL)
	{
		printf("源目录无法打开\n");
		return -1;
	}
	
	if(mkdir(objPath,0777) != 0)
	{
		printf("目标目录创建失败\n");
		closedir(oriDir);
		return -1;
	}

	DIR *objDir = opendir(objPath);
	if(oriDir == NULL)
	{
		printf("目标目录无法打开\n");
		closedir(oriDir);
		return -1;
	}
	
	char oriBuff[256];
	char objBuff[256];
	struct dirent *curDirent = readdir(oriDir);
	while(1)
	{
		if(curDirent != NULL)
		{
			memset(oriBuff,0,sizeof(oriBuff));
			memset(objBuff,0,sizeof(objBuff));
			printf("file name:%s file type:%d\n",
				curDirent->d_name,
				curDirent->d_type);
			if(joinFilePath(oriBuff,oriPath,curDirent->d_name)== 0)
			{
				joinFilePath(objBuff,objPath,curDirent->d_name);
				if(curDirent->d_type == DT_REG)
				{
					myCP(oriBuff,objBuff);
				}
				if(curDirent->d_type == DT_DIR)
				{
					myCPDir(oriBuff,objBuff);
				}

			}	
		}
		else
		{
			break;
		}
		curDirent = readdir(oriDir);
	}

	closedir(oriDir);
	closedir(objDir);
	return 0;
}

int joinFilePath(char *emptyBuff,const char *strPath,const char *strFile)
{
	char string[256] = {0};
	strcat(string,strPath);
	/* 预处理文件路径 */
	if(string[strlen(string)-1] == '/')
	{
		string[strlen(string)-1] = 0;
	}
	
	if(strcmp(strFile,".") == 0 || strcmp(strFile,"..") == 0)
	{
		return -2;
	}

	sprintf(emptyBuff,"%s/%s",string,strFile);

	return 0;
}

