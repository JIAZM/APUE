#include <stdio.h>
#include <stdlib.h>

#define TTY1	"/dev/tty11"
#define TTY2	"/dev/tty12"

enum{
	STATE_R = 1,
	STATE_W,
	STATE_Ex,
	STATE_T
};

struct fsm_st{
	int state;
}

int main(int argc, char *argv[])
{
	int fd1, fd2;

	fd1 = open(TTY1, O_RDWR);
	if(fd1 < 0){
		perror("open()");
		exit(1);
	}

	open(TTY2, O_RDWR | O_NONBLOCK);
	if(fd2 < 0){
		perror("open()");
		exit(1);
	}

	relay();

	close(fd1);
	close(fd2);

	exit(0);
}

relay(int fd1, int fd2)
{
	int fd1_save, fd2_save;

	fd1_save = fcntl(fd1, F_GETFL);
	fcntl(fd1, F_SETFL, fd1_save | O_NONBLOCK);

	fd2_save = fcntl(fd2, F_GETFL);
	fcntl(fd2, F_SETFL, fd2_save | O_NONBLOCK);


}
