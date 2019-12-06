#include "../apue.h"

#define BUFSIZE	4096

int main(int argc, char *argv[])
{
	//char buf[BUFSIZE] = {0};	//"asdfgx";
	char *pbuf;
	int fd;

	fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd < 0){
		perror("open()");
		exit(1);
	}
	write(fd, "1", 1);

	pbuf = mmap(NULL, sizeof(char), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	printf("pbuf : %p \n", pbuf);
/*
	while(1){
		while(buf[i] == 0);
		printf("[%d]:%c \n", i, buf[i]);
		if(buf[i++] == 'x')	break;
	}
*/

	while((*pbuf) != 'x'){
		printf("*pbuf changed to %c \n", *pbuf);
		sleep(1);
	}

	munmap(pbuf, sizeof(char));
	pbuf = NULL;
	close(fd);
	return 0;
}
