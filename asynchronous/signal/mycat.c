#include "../apue.h"

#define CPS	10		//character per second
#define BUFSIZE 4096

static volatile int loop = 1;

static void alrm_handler(int s);

int main(int argc, char *argv[])
{
	int sfd, dfd = 1, len, ret, pos = 0;
	char buf[BUFSIZE];

	if(argc < 2){
		fprintf(stderr, "Usage :\tCopy file data to destination\n");
		exit(1);
	}

	signal(SIGALRM, alrm_handler);
	alarm(1);

	sfd = open(argv[1], O_RDONLY);
	if(sfd < 0){
		perror("open()");
		fprintf(stderr, "\t %s \n", argv[1]);
		exit(1);
	}

	while(1){

		while(loop)
			pause();
		loop = 1;

		len = read(sfd, buf, BUFSIZE);
		if(len < 0){
			perror("read()");
			//fprintf(stderr, "");
			break;
		}
		else if(len == 0)	break;

		pos = 0;

		do{
			ret = write(dfd, buf + pos, len);
			if(ret < 0){
				perror("write()");
				break;
			}
			pos += ret;
			len -= ret;
		}while(len > 0);
	}

	close(sfd);

	exit(0);
}

static void alrm_handler(int s)
{
	loop = 0;
	alarm(1);
}
