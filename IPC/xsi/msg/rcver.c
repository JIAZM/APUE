#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "proto.h"

int main(int argc, char *argv[])
{
	key_t key;
	int msgid;
	struct msg_st rcvbuf;

	key = ftok(KEYPATH, KEYPROJ);
	if(key < 0){
		perror("ftok()");
		exit(1);
	}
	
	//rcv方要先运行 - 一定要CREAT
	msgid = msgget(key, IPC_CREAT | 0600);
	if(msgid < 0){
		perror("msgget()");
		exit(1);
	}

	while(1){
		if(msgrcv(msgid, &rcvbuf, sizeof(rcvbuf) - sizeof(long), 0, 0) < 0){
			perror("msgrcv()");
			exit(1);
		}

		printf("NAME = %s \r\n", rcvbuf.name);
		printf("MATH = %d \r\n", rcvbuf.math);
		printf("CHINESE = %d \r\n", rcvbuf.chinese);
	}

	msgctl(msgid, IPC_RMID, NULL);

	exit(0);
}
