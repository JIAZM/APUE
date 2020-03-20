/*
 *命令行进度条
 *‘#’符号表示进度
 *进度值由百分号表示
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int i = 0;
	char bar[50] = {0};
	char *lab = "-\\|/";

	while(i <= 49){
		//printf("[%-49s][%d%%][%c] \r", bar, i, lab[i%4]);
		printf("[%-49s][%.1f%%][%c] \r", bar, (float)i*100/50, lab[i%4]);
		fflush(stdout);
		sleep(1);
		bar[i++] = '#';
		bar[i] = '\0';
	}
	printf("\n");

	exit(0);
}
