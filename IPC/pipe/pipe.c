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

	if(pid == 0){	//子进程通过管道读数据，进行下一步操作
		close(pd[1]);
		len = read(pd[0], buf, BUFSIZE);
		write(1, buf, len);
		close(pd[0]);
		exit(0);
	}
	else{			//父进程读数据写入管道
		close(pd[0]);
		write(1, "hello!\r\n", 8);
		close(pd[1]);
		wait(NULL);
		exit(0);
	}
}
