//约定双方对话格式

#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH	"/tmp/keyfile"
#define KEYPROJ	'g'
#define NAMESIZE	32

struct msg_st{
	long mtype;
	char name[NAMESIZE];
	int math;
	int chinese;
};




#endif

