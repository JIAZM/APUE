#include "../apue.h"

#define BUFSIZE	4096

int main(int argc, char *argv[])
{
	int fd, len;
	char buffer[BUFSIZE];

	fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		perror("open()");
		exit(1);
	}

	do{
		len = read(fd, buffer, BUFSIZE);
		if(len < 0){
			perror("read()");
			exit(1);
		}

		printf("%s", buffer);
	}while(len > 0);

	close(fd);

	exit(0);
}
