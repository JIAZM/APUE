#include "../apue.h"

#define DELIMS	" \t\n"

typedef struct{
	glob_t globres;
}cmd_st;

static void prompt(void);
static void parse(char *line, cmd_st *res);

int main(int argc, char *argv[])
{
	pid_t pid;
	char *linebuf = NULL;
	size_t linebuf_size = 0;
	cmd_st cmd;

	while(1){
		prompt();	//输出提示符

		if(getline(&linebuf, &linebuf_size, stdin) < 0)	//读取一行命令
		{
			printf("getline() error ! \n");
			break;
		}

		parse(linebuf, &cmd);	//解析命令

		if(0){	//内部命令
			
		}
		else{
			pid = fork();
			if(pid < 0){
				perror("fork()");
				exit(1);
			}
			if(pid == 0){
				fflush(NULL);

				execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
				perror("execvp()");
				exit(1);
			}
			else{
				wait(NULL);
				puts("OK");
			}
		}
	}
	exit(0);
}

static void prompt(void)
{
	printf("mysh-0.1$ ");
}

static void parse(char *line, cmd_st *res)
{
	char *tok;
	int i = 0;

	while(1){
		tok = strsep(&line, DELIMS);
		if(tok == NULL)
			break;
		if(tok[0] == '\0')
			continue;

		glob(tok, GLOB_NOCHECK | GLOB_APPEND * i, NULL, &res->globres);	//应该只有第一次不追加，以后每一次都追加
		i = 1;
	}
}
