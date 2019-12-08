/*
 *
 *守护进程
 *使用sigaction()函数
 *
 */
#include "../../apue.h"

#define FNAME	"/tmp/out"

int daemonize(void);
static void daemon_exit(int s);

static FILE *fp;

int main(int argc, char *argv[])
{
	int i;
	struct sigaction sa;

	sa.sa_handler = daemon_exit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGQUIT);
	sigaddset(&sa.sa_mask,SIGTERM);
	sigaddset(&sa.sa_mask,SIGINT);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	

	openlog("mydaemon", LOG_PID, LOG_DAEMON);

	if(daemonize()){
		syslog(LOG_ERR, "daemonize() failed !");
		exit(1);
	}
	else{
		syslog(LOG_INFO, "daemonize() successed !");
	}

	fp = fopen(FNAME, "w");
	if(fp == NULL){
		syslog(LOG_ERR, "fopen():%s", strerror(errno));
		exit(1);
	}

	syslog(LOG_INFO, "%s was opened .", FNAME);

	for(i = 0; ; i++){
		fprintf(fp, "%d \n", i);
		fflush(fp);
		syslog(LOG_DEBUG, "%d is printed.", i);
		sleep(1);
	}

}

int daemonize(void)
{
	int fd;
	pid_t pid;

	pid = fork();
	if(pid < 0)
		return -1;	//exit(1);
	if(pid > 0)
		exit(0);

	fd = open("/dev/null", O_RDWR);
	if(fd < 0)
		return -1;	//exit(1);

	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);
	if(fd > 2)
		close(fd);

	setsid();

	chdir("/");
	//umask(0);	//确保不产生文件关掉umask
	
	return 0;
}
static void daemon_exit(int s)
{
	fclose(fp);
	closelog();

	exit(0);
	
}
