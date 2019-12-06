#include "./apue.h"

#define BUFSIZE	4096

int main(int argc, char *argv[])
{
	int fd, len, ret;
	char buf[BUFSIZE];

	fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		perror("open()");
		exit(1);
	}

	do{
		len = read(fd, buf, BUFSIZE);
		if(len < 0){
			perror("read()");
			fprintf(stderr, "Device file %s read failed !\n", argv[1]);
			break;
		}
	
		fprintf(stdout, "%s", buf);
	}while(1);	//buf[0] != '\0'

	close(fd);

	exit(0);
}
