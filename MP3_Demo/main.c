#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "list.h"

int createFileList(List *list,const char * path);
int joinFilePath(char *emptyBuff,const char *strPath,const char *strFile);

int main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("命令参数错误\n");
		return -1;
	}
		
	List list;
                                                                 
        initList(&list);
                                                                 
	int retval;	
	retval = createFileList(&list,argv[1]);

        char *string;	
                                                                 
        do
        {
        	printf("List length:%d\n",list.size);
        	removeList(&list,NULL,(void **)&string);
        	printf("String:%s\n",string);	
        }while(list.head != NULL);

	return retval;
}

int createFileList(List *list,const char * path)
{
	DIR *dir = opendir(path);	
	if(dir == NULL)
	{
		printf("目录无法打开\n");
		return -1;
	}
	
	char nameBuff[256];
	struct dirent *curDirent = readdir(dir);
	while(1)
	{
		if(curDirent != NULL)
		{
			memset(nameBuff,0,sizeof(nameBuff));
			printf("file name:%s file type:%d\n",
				curDirent->d_name,
				curDirent->d_type);
			if(joinFilePath(nameBuff,path,curDirent->d_name)== 0)
			{
				if(curDirent->d_type == DT_REG)
				{
					insertList(list,list->tail,(void *)curDirent->d_name);
				}
				else if(curDirent->d_type == DT_DIR)
				{
					createFileList(list,nameBuff);
				}

			}	
		}
		else
		{
			break;
		}
		curDirent = readdir(dir);
	}

	closedir(dir);
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
