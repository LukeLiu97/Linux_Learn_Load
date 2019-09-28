#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{	
	printf("IPC pip Test\n");
	printf("========================\n");

	printf("Process ID:%d\n",getpid());
	
	int pipefd_P2C[2];
	int pipefd_C2P[2];

	/* 创建无名管道 */
	int ret_pipe_P2C = pipe(pipefd_P2C);
	int ret_pipe_C2P = pipe(pipefd_C2P);

	if(ret_pipe_P2C != 0 || ret_pipe_C2P != 0)
	{
		printf("Create pipe fail,pipe retval P2C:%d,pipe retval C2P:%d\n",
		ret_pipe_P2C,
		ret_pipe_C2P);
		return -1;
	}

	int m = 0,n = 0;
	char cBuff[257] = {0};
	pid_t ret_fork = fork();
	if(ret_fork > 0)// Paretent process
	{
		printf("Parent process PID:%d PPID:%d m:%d n:%d\n",
				getpid(),
				getppid(),
				m,
				n++);

		close(pipefd_P2C[0]);
		close(pipefd_C2P[1]);

		write(pipefd_P2C[1],
			"Hello subProcess\n",
			strlen("Hello subProcess\n"));
		read(pipefd_C2P[0],
			cBuff,
			sizeof(cBuff)-1);
		printf("Parent process: Pipe message:%s",cBuff);

		wait(NULL);
		printf("------------------------\n");
	}
	else if(ret_fork == 0)// Child process
	{
		printf("Child process PID:%d PPID:%d m:%d n:%d\n",
				getpid(),
				getppid(),
				m++,
				n);
		
		close(pipefd_P2C[1]);
		close(pipefd_C2P[0]);

		read(pipefd_P2C[0],
			cBuff,
			sizeof(cBuff)-1);
		printf("Child process: Pipe message:%s",cBuff);

		write(pipefd_C2P[1],
			"Hello mainProcess\n",
			strlen("Hello mainProcess\n"));


		/* 主动结束进程，避免成为僵尸进程 */
		printf("SubProcess exti\n");	
		printf("++++++++++++++++++++++++\n");

		exit(0);
	}
	else// Fork process error
	{
		printf("Fork process fail\n");
		return -1;
	}

	printf("IPC pip Test End \n");
	return 0;
}



/* 单管道实现双进程通讯*/

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <unistd.h>
//
//int main()
//{	
//	printf("IPC pip Test\n");
//	printf("========================\n");
//
//	printf("Process ID:%d\n",getpid());
//	
//	int pipefd[2];
//
//	/* 创建无名管道 */
//	int ret_pipe = pipe(pipefd);
//
//	if(ret_pipe != 0)
//	{
//		printf("Create pipe fail,pipe retval:%d\n",ret_pipe);
//		return -1;
//	}
//
//	int m = 0,n = 0;
//	char cBuff[257] = {0};
//	pid_t ret_fork = fork();
//	if(ret_fork > 0)// Paretent process
//	{
//		printf("Parent process PID:%d PPID:%d m:%d n:%d\n",
//				getpid(),
//				getppid(),
//				m,
//				n++);
//
//		//close(pipefd[0]);
//		write(pipefd[1],
//			"Hello subProcess\n",
//			strlen("Hello subProcess\n"));
//		sleep(2);//不错开，会导致堵塞
//		read(pipefd[0],
//			cBuff,
//			sizeof(cBuff)-1);
//		printf("Parent process: Pipe message:%s",cBuff);
//
//		wait(NULL);
//		printf("------------------------\n");
//	}
//	else if(ret_fork == 0)// Child process
//	{
//		printf("Child process PID:%d PPID:%d m:%d n:%d\n",
//				getpid(),
//				getppid(),
//				m++,
//				n);
//		
//		//close(pipefd[1]);
//		read(pipefd[0],
//			cBuff,
//			sizeof(cBuff)-1);
//		printf("Child process: Pipe message:%s",cBuff);
//
//		write(pipefd[1],
//			"Hello subProcess\n",
//			strlen("Hello subProcess\n"));
//
//
//		/* 主动结束进程，避免成为僵尸进程 */
//		printf("SubProcess exti\n");	
//		printf("++++++++++++++++++++++++\n");
//
//		exit(0);
//	}
//	else// Fork process error
//	{
//		printf("Fork process fail\n");
//		return -1;
//	}
//
//	printf("IPC pip Test End \n");
//	return 0;
//}
//
//
