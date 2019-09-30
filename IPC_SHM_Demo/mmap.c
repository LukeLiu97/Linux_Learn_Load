#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/mman.h>

#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#define NUM_PAGE 2 

int identifyARG(int argc,char **argv);
int readMMap(const char *pathName);
int writeMMap(const char *pathName);
int initMMapFile(const char *pathName);

int main(int argc,char **argv)
{
    switch(identifyARG(argc,argv))
    {
        case 0://读取
		return readMMap(argv[1]);	
        case 1://写入
        	return writeMMap(argv[1]);
	case 2://初始化文件
		return initMMapFile(argv[1]); 
        default://错误
            return -1;
    }
	
	return 0;
}

int identifyARG(int argc,char **argv)
{
    if(argc != 3)
    {
        printf("参数列表数量错误\n");
        return -1;
    }
    else
    {
        if(strcmp(argv[2],"-r") == 0)
        {
            printf("内存映射读取模式\n");
            return 0;
        }
        else if(strcmp(argv[2],"-w") == 0)
        {
            printf("内存映射写入模式\n");
            return 1;
        }
	else if(strcmp(argv[2],"-i") == 0)
	{
		printf("文件初始化模式\n");
		return 2;
	}
        else
        {
            printf("指令不存在\n");
            return -1;
        }
    }
}

int initMMapFile(const char *pathName)
{
	
	off_t fileSize = getpagesize() * NUM_PAGE;
	
	if(truncate(pathName,fileSize) != 0)
	{
		printf("修改文件大小失败\n");
	}
	
	printf("将改变文件大小为:%dkb\n",fileSize/1024);

	int fd = open(pathName,O_WRONLY);
	printf("fd:%d\n",fd);
	if(fd < 0)
	{
		printf("文件无法打开");
	}
	
	char buff[1024] = {0};
	
	memset(buff,' ',sizeof(buff));
	buff[1023] = '\n';
		
	ssize_t retval = 0;
	int i = 0;

	do
	{
		retval = write(fd,buff,sizeof(buff));
		i++;
	}while(i < 4*NUM_PAGE);
	printf("i:%d\n",i);
	
	close(fd);
	
	return 0;
}

int readMMap(const char *pathName)
{
	int fd = open(pathName,O_RDONLY);
	printf("fd:%d\n",fd);
	if(fd < 0)
	{
		printf("文件无法打开");
	}
	
	char *buff = mmap(
			NULL,
			getpagesize() * NUM_PAGE,
			PROT_READ,
			MAP_SHARED,
			fd,
			0);
	printf("ShareMemory:\n%s\n",buff);
		
	munmap(buff,256);
	close(fd);
	
	return 0;
}

int writeMMap(const char *pathName)
{
	int fd = open(pathName,O_RDWR);
	printf("fd:%d\n",fd);
	if(fd < 0)
	{
		printf("文件无法打开");
	}
	
	char *buff = mmap(
			NULL,
			getpagesize(),
			PROT_WRITE,
			MAP_SHARED,
			fd,
			0);
	
	memset(buff,' ',getpagesize());
	munmap(buff,getpagesize());

	buff = mmap(
			NULL,
			256,
			PROT_WRITE,
			MAP_SHARED,
			fd,
			getpagesize() * 1);

	strcpy(buff,"hellohelloworld");	

	munmap(buff,256);

	close(fd);
	
	return 0;
}
