# 进程环境  

```C
//main函数  
int main(int argc, char *argv[]);  
```

- 进程的终止  
	- 正常终止  
		1. 从main函数返回  
			> 规定main必须作为当前进程的入口函数与出口函数  
			> return 0;是给当前进程的父进程看的  
			> 如果程序中没有return语句则返回值为程序中最后一条语句的返回值  
			> 例:
			
			```C
			int main()  
			{
				printf("hello!\n");
			}//这段程序执行后#echo $?的输出值为7(print函数的返回值为显示字节数)
			```

        2. 调用exit  
			>exit()函数能带回去的值只有256种  
			调用exit时会执行钩子函数  

		3. 调用_exit或者_Exit  
			> 导致当前进程正常终止  
			> 不执行【钩子函数】也不执行【IO清理】  
			> 在出错了什么都不敢动的情况下使用_exit函数  

		4. 最后一个线程从其启动例程返回  

		5. 最后一个线程调用pthread_exit  

    - 异常终止  
		6.	调用abort  
		7.	接到一个信号并终止  
		8.	最后一个线程对其取消请求作出相应  

	## <u>***钩子函数***</u>  
	```C
	/*
	*	注册一个函数在正常进程终止时调用
	*/
	#include <stdlib.h>
	int atexit(void (*function)(void)); // 钩子函数
	int on_exit(void (*function)(int , void *), void *arg);	// 钩子函数
	```

- 命令行参数的分析  
	```C
	#include <unistd.h>
	int getopt(int argc, char *const argv[], const char *optstring);
	
	extern char *optarg;
	extern int optind, opterr, optopt;
	
	#include <getopt.h>
	int getopt_long(int argc, char * const argv[],
					const char *optstring,
					const struct option *longopts, int *longindex);
	
	int getopt_long_only(int argc, char * const argv[],
						const char *optstring,
						const struct option *longopts, int *longindex);
	```
	
- 环境变量  
	> KEY = value  
	> shell的外部命令-在磁盘上存储的二进制文件
	
	+ 在C语言中使用环境变量
		```C
		extern char **environ;  
		int main()  
		{  
			for(int i = 0; environ[i] != NULL; i++)  
			puts(environ[i]);  
			exit(0);  
		}  
		```
	
- 函数：  
	```C
	#include <stdlib.h>

	char *getenv(const char *name);	//获取环境变量
	int setenv(const char *name, const char *value, int overwrite);	//改变或添加环境变量值
	int unsetenv(const char *name);	//删除环境变量
	int putenv(char *string);
	```

<u>***C程序的存储空间布局***</u>  

```C
/*
 *32Bit - 4G
 */

4G	|-----------|
	|           |
3G	|-----------|------0xC0000000
	|argv/env   |
	|-----------|
	|STACK      |
	|-----------|
	|加载库      |
	|           |
	|-----------|
	|HEAP       |
	|-----------|
	|BSS        |
	|-----------|
	|Initialized|
	|-----------|
	|Text       |
	|-----------|------0x08048000
	|           |
0G	|___________|------0x00000000
```

# pmap 命令查看进程地址  

- 库
	- 动态库
	- 静态库	
	- 手工装载库
		```C
		#include <dlfcn.h>  
		void *dlopen(const char *filename, int flag);  
		int dlclose(void *handle);  
		char *dlerror(void);  
		void *dlsym(void *handle, const char *symbol);  
		链接时使用-ldl选项  
		```
- 函数的跳转  
	```C
	#include <setjmp.h>
	int setjmp(jmp_buf env);	//设置跳转点  
	int sigsetjmp(sigjmp_buf env, int savesigs);
	
	void longjmp(jmp_buf env, int val);	//从某一位置跳回到某一跳转点
	void siglongjmp(sigjmp_buf env, int val);
	```
- 资源的获取与控制  

