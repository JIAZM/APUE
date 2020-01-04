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
		close(pd[1]);
		dup2(pd[0], 0);
		close(pd[0]);
		execl("/usr/bin/mpg123", "mpg123", "-", NULL);
		perror("execl()");
		exit(1);
	}
	else{	//父进程读数据写入管道
		close(pd[0]);
		
		close(pd[1]);
		wait(NULL);
		exit(0);
	}
}
