/*
 *
 *	由一个子进程永远拿不到质数，他拿到的永远是某一个数的整倍数
 *	0	1	2
 *	3	4	5
 *	6	7	8
 *	9	10	11
 *	.	.	.
 *	.	.	.
 *	.	.	.
 *
 */

#include "../apue.h"

#define	LEFT	30000000
#define RIGHT	30000200
#define N	3

int main(int argc, char *argv[])
{
	int i, j, mark, n;
	pid_t pid;

	for(n = 0; n < N; n++){
		pid = fork();
		if(pid < 0){
			perror("fork():");
			exit(1);
		}
		if(pid == 0){
			for(i = LEFT+n; i <= RIGHT; i+=N){
					mark = 1;
					for(j = 2; j < (i / 2); j++){
						if(i % j == 0){
							mark = 0;
							break;
						}
					}
					if(mark){
						printf("[%d]%d is a primer \n", n, i);
					}
			}
			exit(0);
		}
	}

	for(n = LEFT; n <= RIGHT; n++)
		wait(NULL);
	exit(0);
}
