#include "../apue.h"

int main(int argc, char *argv[])
{

	time_t end;
	int64_t count = 0;
	
	end = time(NULL) + 5;

	while(time(NULL) <= end)
		count++;

	printf("%lld \n", count);

	exit(0);
}
