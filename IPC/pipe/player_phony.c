/*
 * 伪码
 */

#include "../../apue.h"

#define BUFSIZE	1024

int main(int argc, char *argv[])
{
	int pd[2], len;
	char buf[BUFSIZE];
	pid_t pid;

	if(pipe(pd) < 0){
		perror("pipe()");
		exit(1);
	}

	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(1);
	}

	if(pid == 0){	//子进程通过管道读数据,调用exec
		close(pd[1]);	//关闭写端
		dup2(pd[0], 0);
		close(pd[0]);
		fd = open("/dev/null", O_RDWR);
		dup2(fd, 1);
		dup2(fd, 2);
		execl("/usr/bin/mpg123", "mpg123", "-", NULL);
		perror("execl()");
		exit(1);
	}
	else{	//父进程读数据写入管道
		close(pd[0]);	//关闭读端
		//父进程收数据通过写端向管道传输数据
		//写入媒体数据需要考虑 码率、流控 等问题
		close(pd[1]);
		wait(NULL);
		exit(0);
	}
}
