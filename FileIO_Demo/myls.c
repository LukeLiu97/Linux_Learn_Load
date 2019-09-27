/* 文件权限显示，以及时间显示有误 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <pwd.h>
#include <grp.h>

#include <time.h>

#include <string.h>

void pathNameCreate(char *pathName,const char *dirName,const char *fileName);
void fileStatDisp(const struct stat *fileStat,const struct dirent *pDirent);
int main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("输入格式错误\n");
		return -1;
	}

	DIR * curDir = opendir(argv[1]);
	if(curDir == NULL)
	{
		printf("目录不存在\n");
		close(curDir);
		return -1;
	}
	else
	{
		printf("目录已打开\n");
	}

	struct dirent *curDirent = readdir(curDir);
	struct stat fileStat;

	char dirStr[256];

	while(curDirent != NULL)
	{
//		printf("filename:%s\n",curDirent->d_name);

		pathNameCreate(dirStr,argv[1],curDirent->d_name);
//		printf("path name:%s\n",dirStr);
		stat(dirStr,&fileStat);

		fileStatDisp(&fileStat,curDirent);	
		curDirent = readdir(curDir);
	}

	return 0;
}

void pathNameCreate(char *pathName,const char *dirName,const char *fileName)
{
	pathName[0]=0;
	strcat(pathName,dirName);
	strcat(pathName,"/");
	strcat(pathName,fileName);
}

void fileStatDisp(const struct stat *fileStat,const struct dirent *pDirent)
{
	char string[256] = {0};

//	//printf("devID:%d\n",fileStat->st_dev);
	/* 依据st_mode,拼接对应文件类型 */
	switch (fileStat->st_mode & S_IFMT) {
		case S_IFBLK:  strcat(string,"b");  break;
		case S_IFCHR:  strcat(string,"c");  break;
		case S_IFDIR:  strcat(string,"d");  break;
		case S_IFIFO:  strcat(string,"p");  break;
		case S_IFLNK:  strcat(string,"l");  break;
		case S_IFREG:  strcat(string,"-");  break;
		case S_IFSOCK: strcat(string,"s");  break;
		default:       strcat(string,"?");  break;
	}

	int i;
	/* 依据st_mode，拼接文件权限 */
//	printf("Mode: %lo (octal)\n",fileStat->st_mode);
	for(i = 0;i < 3;i++)
	{
		if(fileStat->st_mode & 0x00400>>(4*i))
		{
			strcat(string,"r");
		}
		else
		{
			strcat(string,"-");
		}

		if(fileStat->st_mode & 0x00200>>(4*i))
		{
			strcat(string,"w");
		}
		else
		{
			strcat(string,"-");
		}

		if(fileStat->st_mode & 0x00100>>(4*i))
		{
			strcat(string,"x");
		}
		else
		{
			strcat(string,"-");
		}
	}
	strcat(string,".");

	/*拼接硬链接数 */
	char nlinkStr[32] = {0};
	sprintf(nlinkStr," %2d ",fileStat->st_nlink);
	strcat(string,nlinkStr);

	/* 拼接用户名与组名 */
	struct passwd *pwd = NULL;
	pwd = getpwuid(fileStat->st_uid);
	strcat(string,pwd->pw_name);
	strcat(string," ");
	
	struct group *grp = NULL;
	grp = getgrgid(fileStat->st_gid);
	strcat(string,grp->gr_name);

	/* 拼接文件大小 */
	char sizeStr[32] = {0};
	sprintf(sizeStr," %4d ",fileStat->st_size); 
	strcat(string,sizeStr);

	/* 拼接修改日期 */
	strcat(string,ctime(&(fileStat->st_mtime)));
	string[strlen(string) - 1] = 0;
	strcat(string," ");
	
	/* 拼接文件名 */
	strcat(string,pDirent->d_name);

	printf("%s\n",string);
}


