#include "../apue.h"

int main(int argc, char *argv[])
{
	pid_t pid;

	printf("[%d]Begin ! \n", getpid());

	fflush(NULL);
	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(1);
	}

	if(pid == 0){	//child process
		printf("[%d]:child is working!\n", getpid());
	}
	else{			//parent process
		printf("[%d]:parent is working \n", getpid());
	}


	
	printf("[%d]End ! \n", getpid());
	//getchar();
	exit(0);
}
