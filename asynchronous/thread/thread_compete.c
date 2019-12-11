/*
 *这是一个线程竞争的实验
 *程序竞争非常成功
 *没有任何输出
 */

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	LEFT	30000000
#define	RIGHT	30000200
#define	THRNUM	(RIGHT-LEFT+1)


static void *thr_prime(void *p);

int main(int argc, char *argv[])
{
	int i, err;
	pthread_t tid[THRNUM];

	for(i = LEFT; i <= RIGHT; i++){
		err = pthread_create(tid+(i-LEFT), NULL, thr_prime, (void *)i);
		//竞争的原因：201个线程同时在使用 i 的地址
		// 使用强制转换可以解决，但是不是一种好办法
		//pthread_join(tid[i-LEFT], NULL);
		if(err){
			fprintf(stderr, "pthread_create():%s \n", strerror(err));
			exit(1);
		}
	}

	for(i = LEFT; i <= RIGHT; i++)
		pthread_join(tid[i - LEFT], NULL);

	exit(0);
}

static void *thr_prime(void *p)
{
	int i, j, mark;
	i = (int)p;

	mark = 1;

	for(j = 2; j < i/2; j++){
		if(i % j == 0){
			mark = 0;
			break;
		}
	}
	if(mark){
		printf("%d is a primer \n", i);
	}

	pthread_exit(NULL);
}
