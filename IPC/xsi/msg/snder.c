#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "proto.h"

int main(int argc, char *argv[])
{
	key_t key;
	int msgid;
	struct msg_st sndbuf;

	key = ftok(KEYPATH, KEYPROJ);
	if(key < 0){
		perror("ftok()");
		exit(1);
	}

	msgid = msgget(key, 0);
	if(msgid < 0){
		perror("msgget()");
		exit(1);
	}

	sndbuf.mtype = 1;
	strcpy(sndbuf.name, "Alan");
	sndbuf.math = rand() % 100;
	sndbuf.chinese = rand() % 100;
	if(msgsnd(msgid, &sndbuf, sizeof(sndbuf) - sizeof(long), 0) < 0){
		perror("msgsnd()");
		exit(1);
	}

	//msgctl();	谁创建谁销毁　谁打开谁关闭
	
	puts("OK");

	exit(0);
}
