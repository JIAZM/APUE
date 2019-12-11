# 线程  
>*进程是一个容器，用来承载多线程*  
>*信号与多线程可以在安全的情况下小范围混用*  
---  


+	__线程的概念__  
	><u>***线程：一个正在运行的函数***</u>  
	>***POSIX线程是一套标准，而不是实现***  
	>***openmp线程***  
	>***线程标识:pthread_t - 只是一个标识，具体什么类型各家自己实现***  
---  
+ __线程的创建__  
	> `#include <pthread.h>`  
	 `int pthread_equal(pthread_t t1, pthread_t t2);`  
	 `pthread_t pthread_self(void);`  
	 `int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(start_routine)(void *), void *arg);`  
  
	- 使用strerror();对错误代码进行解析
	>`fprintf(stderr, "pthread_create():%s \n", strerror(err));`  
	- 线程的调度取决于调度器的调度策略  

## ***回忆 - 进程终止的八种方式***  

- __线程的终止__  
	>3种方式：
	>- 线程从启动例程返回，返回值就是线程的退出码  
	>- 线程可以被同一进程中的其他线程取消  
	>- 线程调用pthread_exit()函数  
	
	`void pthread_exit(void *retval);`  
	- 实现线程栈的清理

	`int pthread_join(pthread_t thread, void **retval);`  
	- 收尸，相当于进程中的wait();

- __栈的清理__  
	`void pthread_cleanup_push(void (*routine)(void *), void *arg);`  
	`pthread_cleanup_pop(int execute);`  
	- (钩子函数) - 将处理函数压栈 / 出栈
	- push与pop应该成对出现，同栈的操作顺序一样  

	## ***资源，谁打开谁关闭；谁申请谁释放***  

- __线程的取消选项__  
	`int pthread_cancel(pthread_t thread); //线程取消`  
	- 取消有2种状态:允许 、不允许  
	允许取消又分为：
		- 异步cancel
		- 推迟cancel(默认)：推迟至cancel点取消  
			- POSOX定义的cancel点都是可能引发阻塞的系统调用  
	- 设置cancel点：
		>`int pthread_setcancelstate(int state, int *oldstate);`  //设置是否允许取消  
		>`int pthread_setcanceltype(int type, int *oldtype);`  //设置取消方式  
		>`void pthread_testcancel(void);`  //什么都不做，就是一个取消点  
	- 线程分离  
		>`int pthread_detach(pthread_t thread);`  //设置线程分离属性，无法收尸，生死存亡 各安天命  
## ***线程的代码段是共用的，但是栈是独立的***  
> + 当前空间能创建出多少线程个数，取决于外在的资源量  
> + 但在64位系统中最先消耗完的可能是进程号  



+ __线程同步__  

+ __线程属性__  
	
- __线程同步的属性__  

+ __重入__  

- __线程与信号__  

- __线程与fork__  

