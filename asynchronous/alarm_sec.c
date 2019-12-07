#include "../apue.h"

static void alarm_handler(int s);

static volatile int loop = 1;	//去到这个变量真正的内存空间取数值，不要轻信内存中对这个变量的存放

int main(int argc, char *argv[])
{
	int64_t count = 0;

	signal(SIGALRM, alarm_handler);
	alarm(5);	//默认情况下时钟信号终止后进程会退出

	while(loop)
		count++;

	printf("%lld \n", count);

	exit(0);
}

static void alarm_handler(int s)
{
	loop = 0;	
}
