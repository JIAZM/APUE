#include "../../apue.h"

#define TTY1	"/dev/tty11"
#define TTY2	"/dev/tty12"
#define BUFSIZE	4096

enum{
	STATE_R = 1,
	STATE_W,
	STATE_Ex,
	STATE_T
};

struct fsm_st{
	int state;
	int sfd;
	int dfd;
	char buf[BUFSIZE];
	int len;
	int pos;
	char *errstr;
};

static void fsm_driver(struct fsm_st *fsm);
static void relay(int fd1, int fd2);


int main(int argc, char *argv[])
{
	int fd1, fd2;

	fd1 = open(TTY1, O_RDWR);
	if(fd1 < 0){
		perror("open()");
		exit(1);
	}
	write(fd1, "TTY1 \n", 6);

	open(TTY2, O_RDWR | O_NONBLOCK);
	if(fd2 < 0){
		perror("open()");
		exit(1);
	}
	write(fd2, "TTY2 \n", 6);

	relay(fd1, fd2);

	close(fd1);
	close(fd2);

	exit(0);
}

static void relay(int fd1, int fd2)
{
	int fd1_save, fd2_save;
	struct fsm_st fsm_12, fsm_21;

	fd1_save = fcntl(fd1, F_GETFL);
	fcntl(fd1, F_SETFL, fd1_save | O_NONBLOCK);

	fd2_save = fcntl(fd2, F_GETFL);
	fcntl(fd2, F_SETFL, fd2_save | O_NONBLOCK);

	fsm_12.state = STATE_R;
	fsm_12.sfd = fd1;
	fsm_12.dfd = fd2;
	
	fsm_21.state = STATE_R;
	fsm_21.sfd = fd2;
	fsm_21.dfd = fd1;

	while(fsm_12.state != STATE_T || fsm_21.state != STATE_T){
		fsm_driver(&fsm_12);
		fsm_driver(&fsm_21);
	}

	fcntl(fd1, F_SETFL, fd1_save);
	fcntl(fd2, F_SETFL, fd2_save);
}

static void fsm_driver(struct fsm_st *fsm)
{
	int ret;

	switch(fsm->state){
		case STATE_R:
			fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
			if(fsm->len == 0)
				fsm->state = STATE_T;
			else if(fsm->len < 0){
				if(errno == EAGAIN)
					fsm->state = STATE_R;
				else{
					fsm->errstr = "read()";
					fsm->state = STATE_Ex;
				}
			}
			else{
				fsm->pos = 0;
				fsm->state = STATE_W;
			}
			break;
		case STATE_W:
			ret = write(fsm->dfd, fsm->buf+fsm->pos, fsm->len);
			if(ret < 0){
				if(errno == EAGAIN)
					fsm->state = STATE_W;
				else{
					fsm->errstr = "write()";
					fsm->state = STATE_Ex;
				}
			}
			else{
				fsm->pos += ret;
				if((fsm->len -= ret) == 0)
					fsm->state = STATE_R;
				else
					fsm->state = STATE_W;
			}
			break;
		case STATE_Ex:
			perror(fsm->errstr);
			fsm->state = STATE_T;
			break;
		case STATE_T:
			/****Do something***/
			abort();
			break;
		default:
			break;
	}
}


