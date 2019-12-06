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
		execl("/bin/date", "date", "+%s", NULL);
		perror("exec()");
		exit(1);
	}
	wait(NULL);

	puts("End");

	exit(0);
}
