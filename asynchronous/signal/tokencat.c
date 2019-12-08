/*
 *
 *
 *使用alarm()+signal()实现流控
 *
 *
 *
 *
 */


#include "../apue.h"

#define CPS	10		//character per second
#define BUFSIZE CPS
#define BURST	100

static volatile int token = 0;

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
		while(token <= 0)
			pause();
		token--;

		while((len = read(sfd, buf, BUFSIZE)) < 0){
			if(errno = EINTR)
				continue;
			perror("read()");
			break;
		}
		if(len == 0)	break;

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
	alarm(1);
	token++;
	if(token > BURST)
		token = BURST;
}



