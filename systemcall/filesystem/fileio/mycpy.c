#include "./apue.h"

#define BUFSIZE 4096

int main(int argc, char *argv[])
{
	int sfd, dfd, len, ret, pos = 0;
	char buf[BUFSIZE];

	if(argc < 2){
		fprintf(stderr, "Usage :\tCopy file data to destination\n");
		exit(1);
	}

	sfd = open(argv[1], O_RDONLY);
	if(sfd < 0){
		perror("open()");
		fprintf(stderr, "\t %s \n", argv[1]);
		exit(1);
	}
	dfd = open(argv[2], O_RDWR | O_CREAT);
	if(dfd < 0){
		perror("open()");
		fprintf(stderr, "\t %s \n", argv[2]);
		exit(1);
	}

	while(1){
		len = read(sfd, buf, BUFSIZE);
		if(len < 0){
			perror("read()");
			//fprintf(stderr, "");
			break;
		}
		else if(len == 0)	break;

		pos = 0;

		do{
			ret = write(dfd, buf + pos, len);
			if(ret < 0){
				perror("write()");
				break;
			}
			pos += ret;
			len -= ret;
		}while(len > 0);
	}

	close(sfd);
	close(dfd);
}
