#include "../../apue.h"

#define TTY1	"/dev/tty11"
#define TTY2	"/dev/tty12"
#define BUFSIZE	4096

enum{
	STATE_R = 1,
	STATE_W,
	STATE_AUTO,	//STATE_AUTO =3，当状态为STATE_EX或STATE_T(即大于3时，自动推动状态机)
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
static int max(int a, int b);


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
	struct pollfd pfd[2];

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

/*	while(fsm_12.state != STATE_T || fsm_21.state != STATE_T){
		fsm_driver(&fsm_12);
		fsm_driver(&fsm_21);
	}
*/
	pfd[0].fd = fd1;
	pfd[1].fd = fd1;

	while(fsm_12.state != STATE_T || fsm_21.state != STATE_T){
		//布置监视任务
		pfd[0].events = 0;
		if(fsm_12.state == STATE_R)
			pfd[0].events |= POLLIN;
		if(fsm_12.state == STATE_W)
			pfd[0].events |= POLLOUT;

		pfd[1].events = 0;
		if(fsm_21.state == STATE_R)
			pfd[1].events |= POLLIN;
		if(fsm_21.state == STATE_W)
			pfd[1].events |= POLLOUT;

		//监视
		if(fsm_12.state < STATE_AUTO || fsm_21.state < STATE_AUTO){
			while(poll(pfd, 2, -1)){
				if(errno == EINTR)
					continue;
				perror("poll()");
				exit(1);
			}
		}
		//查看监视结果，根据结果有条件的推动状态机
		if(pfd[0].revents & POLLIN || pfd[1].revents & POLLOUT \
				|| fsm_12.state > STATE_AUTO)	//fsm_12.state>STATE_STATE_AUTO实现状态为Ex或T时自动推一下状态机
			fsm_driver(&fsm_12);	//使用FD_ISSET探测读集与写集的行为

		if(pfd[0].revents & POLLOUT || pfd[1].revents & POLLIN \
				|| fsm_21.state > STATE_AUTO)
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

static int max(int a, int b)
{
	if(a > b)
		return a;
	else
		return b;
}
