#include "../apue.h"

#define PAT	"/etc"

int main(int argc, char *argv[])
{
	DIR *dp;
	struct dirent *curr;

	dp = opendir(PAT);
	if(dp == NULL){
		perror("opendir()");
		fprintf(stderr, "\t%s, %d\n", __FILE__, __LINE__);
		exit(1);
	}

	while((curr = readdir(dp)) != NULL){
		puts(curr->d_name);
	}

	closedir(dp);

	exit(0);
}
