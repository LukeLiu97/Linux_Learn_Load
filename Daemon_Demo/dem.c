#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/resource.h>

int main()
{
	pid_t pid = fork();

	if(pid > 0)// Parent Process
	{
		// 主动退出进程，使子进程成为孤儿进程
		exit(0);
	}
	else if(pid == 0)
	{
		// 退出会话并创建新会话
		pid_t sid = setsid();
		printf("Session ID:%#08x\n",sid);
		
		// 切换工作目录
		chdir("/");

		// 修改掩码
		mode_t mode = umask(0);
		printf("Mode:%#08x\n",mode);

		// 关闭继承到的文件
		struct rlimit rlim;
		getrlimit(RLIMIT_NOFILE,&rlim);
		
		printf("Soft limit:%d,Hard limit:%d\n",
			rlim.rlim_cur,
			rlim.rlim_max);		

		int i;
		for(i = 0;i < rlim.rlim_cur;i++)
		{
			close(i);
		}

		i = 0;
		while(1)
		{
			if(i++ < 10)	
			{
				sleep(1);
			}
			else
			{
				break;
			}
		}		
	}
	else
	{
		return -1;
	}

	return 0;
}
