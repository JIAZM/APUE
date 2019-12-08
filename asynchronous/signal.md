#				并发(信号 - 初步异步 / 线程 - 强烈异步)  
  
同步  
	  
  
异步  
	异步事件的处理：查询 / 通知  
		异步事件发生频率低	使用通知法  
		异步事件发生频率高	使用查询法  
  
	在软件层面没有真正的通知法	其实没有严格意义上的通知法  
  
------信号-------  
  
1-	信号是什么 - 信号的概念  
		信号是软件层面的中断。	信号的响应依赖于中断  
		$kill -l ;查看信号内容  
  
2-	signal();  
		#include <signal.h>  
  
		typedef void (*sighandler_t)(int);  
		sighandler_t signal(int signum, sighandler_t handler);  
  
		void (*signal(int signum, void (*handler)(int)))(int);  
		信号会打断阻塞的系统调用.  
  
3-	信号的不可靠  
		信号的行为不可靠:执行不是我们控制的，现场也不是我们布置的  
  
4-	重入 / 可重入函数  
		为了解决信号的不可靠性  
		第一次调用没有结束，就发生第二次调用的情况下也是可以的  
		所有的系统调用都是可重入的，一部分库函数也是可重入的,如：memcpy();带_r后缀名版本的库函数都是可重入的  
  
5-	信号的响应过程  
	信号从收到到响应由一个不可避免的延迟  
	思考：信号是如何忽略掉的？  
		  标准信号为什么要丢失？  
		  	pending置1就置1，无法记录置了几次，只能记录最后一次的记录  
  
		  标准信号的响应没有严格的顺序 ！！  
	信号实际上是在由kernel回到user的路上响应的  
  
	不能控制信号什么时候到来，但是可以控制信号在什么时候被响应以及是否被响应  
  
6-	信号的常用函数  
		#include <sys/types.h>  
		#include <signal.h>  
	  
		int kill(pid_t pid, int sig);	//给一个进程发送信号  
		int raise(int sig);	//给当前进程发送一个信号  
  
		#include <unistd.h>  
		unsigned int alarm(unsigned int seconds);	#//没办法实现多任务的alarm  
		#涉及时钟信号的情况时，signal()要发生在alarm()前  
		pause();  
		#在某些环境下sleep()是用alarm()+pause()封装实现的  
		#使用alarm()函数实现slow cat流控  
  
		abort();  
		system();  
  
		sleep();的问题  
  
7-	信号集  
	#include <signal.h>  
	#信号集类型	sigset_t  
	int sigemptyset(sigset_t *set);	#//设为空集  
  
	int sigfillset(sigset_t *set);	#//设为全集  
  
	int sigaddset(sigset_t *set, int signum);	#//添加信号到信号集  
  
	int sigdelset(sigset_t *set, int signum);	#//从信号集中删除信号  
  
	int sigismember(const sigset_t *set, int signum);	#//测试signum是否是set中的一员  
  
8-	信号屏蔽字 / pending集 的处理  
	#include <signal.h>  
	int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);  
	#//决定信号什么时候被响应  
	  
	int sigpending(sigset_t *set);	#//到内核看pending位图  
  
	#不能从信号处理函数中随意的往外跳(setjmp();, longjmp();---------> 可能会错过mask信号屏蔽字恢复的过程)  
	使用sigsetjmp();	siglongjmp(); #可以跳  
	  
	  
9-	扩展  
		#include <signal.h>  
		int sigsuspend(const sigset_t *mask);	#//原子操作  
  
		sigaction();	//替换signal()函数  
		# signal()函数的问题  
			## 多个信号共用同一个信号处理函数时实现的功能可以不一样，但是有重入的危险  
			## 重入可能会发生内存泄漏  
			## 在响应某一个信号时，要把其他信号都阻塞住  
		# int sigaction(int signul, const struct sigaction *act, struct sigaction *oldact);  
		struct sigaction{  
			void	(*sa_handler)(int);  
			void	(*sa__sigaction)(int, siginfo_t *, void *);  
				//第三个参数void*的意思是：从哪里跳过来的（进入信号处理函数之前的被打断的现场）,是一个可还原的现场  
			sigset_t 	a_mask;  
			int		sa_flags;  
			void	(*sa_restorer)(void);  
		}  
		setitimer();	// 替换alarm()函数  
  
		# getcontext()与setcontext()的问题  
		#include <ucontext.h>
		int getcontext(ucontext_t *ucp);	//获取一个现场，后续工作可以切换到其他现场工作  
		int setcontext(const ucontext_t *ucp);	//还原一个可执行的现场  
		##在struct sigaction中的三参成员第三个（void *）的参数其实是 ucontext *ucp 类型  
  
  
# 实时信号  
10-	实时信号相关内容  
	标准信号会丢失  标准信号是为定义行为  
	同时有标准信号、实时信号时，先响应标准信号，后响应实时信号  
	信号位置:/usr/include/bits/signum.h  
	## 实时信号会排队，标准信号只产生一次  
	$ulimit -a命令中，pending signal为实时信号的数量，可修改  

