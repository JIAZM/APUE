#include <stdio.h>
#include <stdlib.h>

static void f1(void)
{
	puts("f1 is working ! \n");
}

static void f2(void)
{
	puts("f2 is working ! \n");
}

static void f3(void)
{
	puts("f3 is working ! \n");
}

int main()
{
	puts("Begin !\n");

	atexit(f1);
	atexit(f2);
	atexit(f3);

	puts("End! \n");

	exit(0);
}
