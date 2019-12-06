#include "../apue.h"

#define fname	"/tmp/out"

int main(int argc, char *argv[])
{
	int fd, ttyfd;

	fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if(fd < 0){
		perror("open()");
		exit(1);
	}

/**************************	//需要原子操作
	close(1);
	dup(fd);
**************************/
	dup2(fd, 1);			//以下为原子操作
	if(fd != 1)	close(fd);

	puts("hello ! \n");

	ttyfd = open("/dev/tty", O_RDWR);
	if(ttyfd < 0){
		perror("open()");
		exit(1);
	}

	dup2(ttyfd, 1);
	if(ttyfd != 1){
		close(ttyfd);
	}

	return 0;

}
