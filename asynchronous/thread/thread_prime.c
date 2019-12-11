#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	LEFT	30000000
#define	RIGHT	30000200
#define	THRNUM	(RIGHT-LEFT+1)

typedef struct{
	int n;
}thr_arg_st;

static void *thr_prime(void *p);

int main(int argc, char *argv[])
{
	int i, err;
	pthread_t tid[THRNUM];
	thr_arg_st *p;
	void *ptr;

	for(i = LEFT; i <= RIGHT; i++){

		p = malloc(sizeof(*p));
		if(p == NULL){
			perror("malloc()");
			exit(1);
		}
		p->n = i;

		err = pthread_create(tid+(i-LEFT), NULL, thr_prime, p);
		if(err){
			fprintf(stderr, "pthread_create():%s \n", strerror(err));
			exit(1);
		}
	}

	for(i = LEFT; i <= RIGHT; i++){
		pthread_join(tid[i - LEFT], &ptr);
		free(ptr);
	}

	exit(0);
}

static void *thr_prime(void *p)
{
	int i, j, mark;
	i = ((thr_arg_st *)p)->n;

	//free(p);
	//在这里free不好，free最好和malloc在同一个函数中

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

	pthread_exit(p);
}
