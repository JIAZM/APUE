#include "../apue.h"

#define BUFSIZE	4096

int main(int argc, char *argv[])
{
	char *pbuf_w = NULL;
	//char buf[BUFSIZE] = {0};
	char ch;
	int fd, ret = 0, i = 0;

	fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd < 0){
		perror("open()");
		exit(1);
	}
	write(fd, "1", 1);

	//使用mmap()函数,不能直接操作指针修改值
	pbuf_w = (char *)mmap(NULL, sizeof(char), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	printf("pbuf_w:%p \n", pbuf_w);
	close(fd);

	while((*pbuf_w = getchar()) != 'x')
		*pbuf_w = 0x30 + i++;
	*pbuf_w = 'x';
	//printf("*pbuf_w:cs \n", *pbuf_w);

	ret = munmap(pbuf_w, BUFSIZE);
	perror("munmap()");

	pbuf_w = NULL;
	return 0;
}
