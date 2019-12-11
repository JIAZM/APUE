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

- __栈的清理__  

- __线程的取消选项__  

+ __线程同步__  

+ __线程属性__  
	
- __线程同步的属性__  

+ __重入__  

- __线程与信号__  

- __线程与fork__  

