#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../apue.h"

#define THRNUM	20
#define	FNAME	"/tmp/out"
#define	BUFSIZE	4096

static void *thr_add(void *p);

const char charac[5] = "1234";

int main(int argc, char *argv[])
{
	pthread_t tid[THRNUM];
	int err, i;

	for(i = 0; i < THRNUM; i++){
		err = pthread_create(tid + 1, NULL, thr_add, NULL);
		if(err){
			fprintf(stderr, "pthread_create():%s \n", strerror(err));
			exit(1);
		}
	}

	exit(0);
}

static void *thr_add(void *p)
{
	int fd, ret;
	char linebuf[BUFSIZE];

	fd = open(FNAME, O_RDWR);
	if(fd < 0){
		perror("open()");
		exit(1);
	}

	ret = read(fd, linebuf, BUFSIZE);
	if(ret < 0){
		perror("read()");
		exit(1);
	}

	printf("read : %s \n", linebuf);

	ret = write(fd, charac, 5);
	if(ret < 0){
		perror("write()");
		exit(1);
	}

	pthread_exit(NULL);
}
