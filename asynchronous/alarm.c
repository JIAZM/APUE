#include "../apue.h"

int main(int argc, char *argv[])
{

	alarm(5);

	while(1)
		pause();

	exit(0);
}
