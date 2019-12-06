/*
 *使用glob()函数解析目录 / 解析pattern
 *解析目录时将宏PAT改为	"/etc/*"
 */
#include "../apue.h"

#define PAT	"/etc/*.conf"	//不包括隐藏文件
							//隐藏文件使用	".*"

int errfunc_(const char *errpath, int errno);

int main(int argc, char *argv[])
{
	glob_t globres;
	int err, i;
	
	err = glob(PAT, 0, NULL, &globres);
	if(err){
		printf("Error code is %d \n", err);
		exit(1);
	}

	for(i = 0; i < globres.gl_pathc; i++){
		puts(globres.gl_pathv[i]);
	}

	globfree(&globres);
	exit(0);
}

#if 0
int errfunc_(const char *errpath, int errno)
{
	puts(errpath);
	fprintf(stderr, "ERROR MSG:%s \n", strerror(error));

	return 0;
}
#endif
