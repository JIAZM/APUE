+ 硬链接 / 软链接  
	> 硬链接与目录项是同义词，且建立硬链接有限止：不能给分区、目录建立  
	> 符号链接有点：课跨分区、可以给目录建立符号链接  
	```C
	#include <unistd.h>
	int link(const char *oldpath, const char *newpath);
	int unlink(const char *pathname);
	```
+ 删除文件
	```C
	remove();
	```
+ 重命名文件
	```C
	rename();	//封装出来的命令为mv
	```
+ 文件时间
	```C
	#include <sys/types.h>
	#include <utime.h>
	int utime(const char *filename, const struct utimebuf *times);

	#include <sys/time.h>
	int utimes(const char *filename, const struct timeval times);
	utime();	//可以更改文件最后读、修改的时间
	```

+ 目录的创建与销毁
	```C
	#include <unistd.h>
	int emdir(const char *pathname);	//只能销毁空目录

	#include <sys/types.h>
	#include <sys/stat.h>
	int mkdir(const char *pathname, mode_t mode);
	```

+ 更高当前工作路径
	```C
	#include <unistd.h>
	int chdir(const char *path);
	int fchdir(int fd);

	long getcwd(char *buf, unsigned long size);
	```

+ 假根技术  
	> chroot  
	> 使用 fchdir(); 函数可以突破假根技术(安全机制)

+ 分析目录 / 读取目录内容  
	```C
	#include <glob.h>
	int glob(const char *pattern, int flags, 
				int (*errfunc)(const char *epath, int errno), 
				glob_t *pglob);
	//main函数参数与命令行参数解析
	/* glob函数搜索匹配 函数pattern中的参数，如/*是匹配根文件下的所有文件
	 * （不包括隐藏文件，要找的隐藏文件需要从新匹配），然后会将匹配出的结果存放
	 * 到 pglob，即第4个参数中，第二个参数能选择匹配模式，如是否排序，或者在函
	 * 数第二次调用时，是否将匹配的内容追加到pglob中，等，第3个参数是查看错误信
	 * 息用，一般置为NULL； */
	void globfree(glob_t *pglob);
	
	glob();
	//glob()函数能实现以下函数所有操作
	//解析模式 / 通配符

	opendir();
	|___#include <sys/types.h>
	|___#include <dirent.h>
	|___DIR *opendir(const char *name);
	|___DIR *fdopendir(int fd);

	closedir();
	readdir();	//这是一个库函数
	rewenddir();
	seekdir();
	telldir();
	```