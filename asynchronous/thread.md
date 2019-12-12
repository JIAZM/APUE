# 线程  
>*进程是一个容器，用来承载多线程*  
>*信号与多线程可以在安全的情况下小范围混用*  
---  


+	__线程的概念__  
	><u>***线程：一个正在运行的函数***</u>  
	>***POSIX((Portable Operating System Interface of UNIX, 可移植操作系统接口)线程是一套标准，而不是实现***  
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
			- POSIX定义的cancel点都是可能引发阻塞的系统调用  
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
	- 互斥量  
		>类型：  
		>pthread_mutex_t  
		- 创建互斥量  
	`int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);`  
		- 销毁互斥量  
	`int pthread_mutex_destroy(pthread_mutex_t *mutex);`  
		- 互斥量加锁  
	`int pthread_mutex_lock(pthread_mutex_t *mutex)`  //等锁，死等  
	`int pthread_mutex_trylock(pthread_mutex_t *mutex);`  //非阻塞加锁  
		- 互斥量解锁  
	`pthread_mutex_unlock(pthread_mutex_t *mutex);`  

	- 条件变量 - 实现通知  
		>类型:  
		>pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  
		- 创建条件变量  
	`int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);`  
		- 销毁条件变量  
	`int pthread_cond_destroy(pthread_cond_t *restrict *cond);`  
		- 发消息  
	`int pthread_cond_broadcast(pthread_con_t *cond);`//广播唤醒等待  
	`int pthread_cond_signal(pthread_cond_t *cond);`//唤醒任意一个等待  
		- 等待  
	`int pthread_cond_timedwait()pthread_cond_t *restrict cond, pthread_mutex_t *mutex, const struct timespec *restrict abstime;`  
	`int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);`  //死等  
		## 通知的重点在于监听，有通知时一定要有wait，否则通知传达不到  

	- 信号量  
	>区别于互斥量，互斥量以独占形式来使用某一资源  
	## ***哲学家就餐问题***  
	使用互斥量 + 条件变量实现信号量  
	>具体实现见视频94.6-2-5线程-信号量  

	- 读写锁  
		>读锁->共享锁  写锁->互斥锁  

+ __线程属性__  
	>`int pthread_create(pthread_t, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);`函数中的attr参数  
	- 初始化线程属性  
		>`int pthread_attr_init(pthread_attr_t *attr);`  
	- 销毁线程属性  
		>`int pthread_attr_destroy(pthread_attr_t *attr);`  
	## ***线程间通信比进程间通信更快***  
## ***#pmap命令 - 查看进程空间的分布情况***  
	
- __线程同步的属性__  
	- 互斥量属性  

	- 条件变量属性  

+ __重入__  
	+ 线程安全  
	>先有标准后有实现，POSIX规定如果发布出来一个库，那么这个库应该本身支持多线程并发，否则要在名字上体现出来  
	+ 多线程中的IO  
		>标准IO都支持多线程并发  
		例：puts("aaaa");先锁住缓冲区再向缓冲区中写入数据，再解锁

- __线程与信号__  
	>以线程位单位，线程是如何实现的? - 见笔记  
	- 设置线程mask  
		>`int pthread_sigmask(int how, const sigset_t *set, sigset_t *pldset);`  
	>#include <signal.h>
	- 等待信号  
		>`int sigwait(const sigset_t *set, int *sig);`  
	- 发送信号  
		>`int pthread_kill(pthread_t thread, int sig);`  

- __线程与fork__  
	- 不同线程源语中fork产生的结果不一定是一样的  

---

# 其他线程标准  
+ openmp线程标准  
	>www.OpenMP.org  

	不是从语言角度实现，也不是函数级别的实现
	借助编译器实现并发，gcc4.0以后的版本都能支持OpenMP的语法标记  