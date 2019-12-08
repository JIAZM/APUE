/*
 *	信号驱动程序
 *	当收到信号时进行下一步操作
 */


#include "../../apue.h"

static void int_handler(int s);

int main(int argc, char *argv[])
{
	int i, j;
	sigset_t set, oset, saveset;

	signal(SIGINT, int_handler);
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigprocmask(SIG_UNBLOCK, &set, &saveset);
	sigprocmask(SIG_UNBLOCK, &set, &oset);

	for(j = 0; j < 1000; j++){
		sigprocmask(SIG_BLOCK, &set, NULL);
		for(i = 0; i < 5; i++){
			write(1, "*", 1);
			sleep(1);
		}
		write(1, "\n", 1);

		sigsuspend(&oset);
		/**以下几句操作是不原子的操作**/
		//sigset_t tmpset;
		//sigprocmask(SIG_UNBLOCK, &set, NULL);
		//pause();
		//sigprocmask(SIG_SETMASK, &tmp, NULL);
	}

	sigprocmask(SIG_SETMASK, &saveset, NULL);
}

static void int_handler(int s)
{
	write(1, "!", 1);
}
