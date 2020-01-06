# 进程环境  

```C
//main函数  
int main(int argc, char *argv[]);  
```

- 进程的终止  
	- 正常终止：  
		1. 从main函数返回  
			>规定main必须作为当前进程的入口函数与出口函数  
			return 0;是给当前进程的父进程看的  
			如果程序中没有return语句则返回值为程序中最后一条语句的返回值  
			例：  
				int main()  
				{  
					printf("hello!\n");  
				}  
			这段程序执行后#echo $?的输出值为7(print函数的返回值为显示字节数)  

		2. 调用exit  
			>exit()函数能带回去的值只有256种  
			调用exit时会执行钩子函数  

		3. 调用_exit或者_Exit  
			>导致当前进程正常终止  
			不执行【钩子函数】也不执行【IO清理】  
			在出错了什么都不敢动的情况下使用_exit函数  

		4. 最后一个线程从其启动例程返回  
		5. 最后一个线程调用pthread_exit  

	- 异常终止：  
		6.	调用abort  
		7.	接到一个信号并终止  
		8.	最后一个线程对其取消请求作出相应  

	```C
	#include <stdlib.h>  
	int atexit(void (*function)(void))：钩子函数  
	on_exit()函数也是  
	```


- 命令行参数的分析  
	```C
	getopt()  
getopt_long()  
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
		getenv()	//获取环境变量
		setenv()	//改变或添加环境变量值
		unsetenv()	//删除环境变量
		putenv()
		```

<u>***C程序的存储空间布局***</u>  

```C
/*
 *32Bit - 4G
 */

4G	|-----------|
	|			|
3G	|-----------|------0xC0000000
	|argv/env	|
	|-----------|
	|	STACK	|
	|-----------|
	|	加载库   |
	|			|
	|-----------|
	|   HEAP	|
	|-----------|
	|   BSS		|
	|-----------|
	|Initialized|
	|-----------|
	|   Text	|
	|-----------|------0x08048000
	|			|
0G	|___________|------0x00000000
```

# #pmap 命令查看进程地址  

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
	setjmp();	//设置跳转点  
longjmp();	//从某一位置跳回到某一跳转点  
	```
	
- 资源的获取与控制  