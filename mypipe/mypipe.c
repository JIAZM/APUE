#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "mypipe.h"

struct mypipe_st{
	int head;
	int tail;
	char data[PIPESIZE];
	int datasize;	//Record the number of valid byte in this pipe
	pthread_mutex_t mut;	//The use of the current pipeline is based on the existence of an exclusive form
	pthread_cond_t cond;
};
