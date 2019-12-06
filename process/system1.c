#include "../apue.h"

int main(int argc, char *argv[])
{
	pid_t pid;

	puts("Begin");

	fflush(NULL);

	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(1);
	}

	if(pid == 0){
		execl("/bin/sh", "sh", "-c", "date +%s", NULL);
		perror("exec()");
		exit(1);
	}
	wait(NULL);

	puts("End");

	exit(0);
}
