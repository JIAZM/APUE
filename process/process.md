# 进程的基本知识  

<u>***已经进入多进程阶段，不涉及到并发***</u>

1. 进程标识符 pid  
	类型pid_t	//传统意义上是有符号16位整型数  

  ```shell
  $ ps axf  
  $ ps axm  
  $ ps ax -L  
  ```
	
  进程标识顺次向下使用，与文件描述符不同，不是使用当前可用的最小的  
	
  ```C
  #include <sys/types.h>
  #include <unistd.h>
  getpid();
  getppid();
  ```
  
2. 进程如何产生

  ```C
  #include <unistd.h>
  pid_t fork(void);
  ```

  父进程通过复制自己的方式形成子进程，关键字duplicating意味着拷贝、克隆、一模一样等含义  
  fork后父子进程的区别：fork的返回值不一样，pid不同，ppid也不相同，未决信号和文件锁不继承，资源利用量清零  
  ***init进程***:  

  + init产生之前内核是一个程序在跑，init产生之后内核变成一个库守在后台，出现异常时产生调用  

  + 是所有进程的祖先进程  
  + 1号进程  

  ***fork之前要刷新一下当前打开的所有输出流***:

  ```C
  fflush(NULL);	//避免当前缓冲区中的缓冲被子进程使用  
  ```

  调度器的调度策略决定那个进程先运行  
  子进程一定要有结束符，否则会陷入递归陷阱  

  ***vfork()***
  	fork()函数copy了父进程的所有资源，但是对于工作需求小的进程代价过大  
  	使用vfork()函数，子进程与父进程共享同一块数据  

  ```C
  fork()	//函数中加入了写实拷贝技术，将vfork()的功能加入进来，导致vfork()目前基本废弃。  
  ```

  父子进程的关系：除了最初子进程由父进程创建，其他的都相互独立，谁也不能到对方的空间拿什么东西  

3. 进程的消亡以及释放资源

  ```C
  #include <sys/types.h>  
  #include <sys/wait.h>  
  pid_t wait(int *status);	//死等  
  pid_t waitpid(pid_t pid, int *status, int options);	  
  //等待进程状态发生改变  
  
  waitid();  
  wwait3();  
  wait4();  
  ```

  ***进程分配：***

  + 分块
  + 交叉分配
  + 池类分配  


4. exec函数族
```C
#include <unistd.h>  
extern **environ;  
int execl(const char *path, const char *arg, ...  
          /* (char  *) NULL */);  
int execlp(const char *file, const char *arg, ...  
            /* (char  *) NULL */);  
int execle(const char *path, const char *arg, ...  
            /*, (char *) NULL, char * const envp[] */);  
//以上三个都是定参的实现，参数从argv[0]开始，以NULL结束  
int execv(const char *path, char *const argv[]);  
int execvp(const char *file, char *const argv[]);  
int execvpe(const char *file, char *const argv[],  
            char *const envp[]);  
//以上三个才是变参的实现  
```
<u>***使用exec函数族时也应该注意刷新输出流缓冲	在调用exec前使用fflush();***</u>  

5. 用户权限以及组权限
```shell
$ u+s	#可执行文件有这个权限，当别的用户调用当前可执行文件的时候用户会切换为当前用户的user 
```
  + ​uid  
    > realuid  
    > effectiveuid	//检测的是effectiveuid  
    > saveuid	//可以没有  

```shell
$ g+s	#可执行文件有这个权限，当别的用户调用当前可执行文件的时候用户组会切换为当前用户同组权限
```

  + gid  
    > real  
    > effective  
    > save	//可以没有  
  
  + init产生的时候还是root权限  
    > fork()+exec() --> getey进程 -->exec() -->login()输入用户名密码等登录信息后 成功 -->fork()+exec() -->产生shell (带着r e s三种权限)  

  ```C
  uid_t getuid(void);	//返回进程的实际用户id  
  uid_t geteuid(void); //返回effective uid  
  
  gid_t getgid(void);  
  gif_t getegid(void);  
  
  int setuid(uid_t uid);	//设置effective uid  
  int seteuid(uid_t uid);	//设置effective uid  
  int setgid(gid_t gid);  
  int setegid(gid_t gid);  
  
  int setreuid(uid_t ruid, uid_t euid);	//原子化的交换ruid与euid  
  int setregid(gid_t rgid, gid_t egid);	//原子化的交换rgid与egid  
  ```

6. 观摩：解释器文件

  <u>***解释器文件 - 脚本文件***</u>

  ```shell
  #!/bin/bash  
  /etc/passwd文件中设置winguest:x:1001:1001::/home/winshare:/usr/bin/top -S  
  ```

  限制用户登录shell为/usr/bin/top，并以安全模式登录  


7. system();
	
```C
#include <stdlib.h>  
int system(const chat *command);  
```
  
<u>***调用shell执行一个shell命令,相当于对fork() / exec() / wait() 函数的一个封装***</u>
  
8. 进程会计  
统计进程占用资源量  

```C
#include <unistd.h>
int acct(const chat *filename);
```

<u>***UNIX系统方言，不是POSIX协议中的函数	，不可移植***</u>

9. 进程时间

```C
#include <sys/times.h>  
clock_t times(struct tms *buf);  
//clock_t类型为clock ticks数(滴答数 / 系统心跳)，使用宏  
sysconf(_SC_CLK_TCK);
//可以检测每秒钟有多少system ticks数
```

10. 守护进程

  <u>***写守护进程一定要写系统日志！！！！！***</u>  

  <u>***守护进程一般脱离控制终端而存在***</u>  

  <u>***PPID为1、PID、PGID、SID相同，并且TTY项为？的进程是守护进程***</u>  
  <u>***守护进程一般都是一个会话的leader***</u>  

  + 会话：session,标识sid  
    一个终端的登录产生了一个标准的会话形式  

  + 终端：  
    真正意义上的终端是一个本设备	真正的终端只负责输入与输出  

  + 前台进程组 / 后台进程组  
    前台进程组能够使用标准输入	如果企图把标准输入给后台进程，将会杀掉后台运行的进程  

  ```C
  #include <unistd.h>  
  /**********标准UNIX调用************/  
  pid_t setsid(void);  
  pid_t getpgid(pid_t pid);	//获取某一个指定进程的进程组ID  
  pid_t setpgid(pid_t pid);	//设置指定进程到指定进程组中  
    调用方会成为当前进程组的leader并且脱离控制终端  
  /**********方言************/  
  pid_t getpgrp(void);	//返回当前进程所在进程组的ID  
  pid_t getpgrp(psid_t pid);	//查看一个指定的进程所属的进程组是哪一个  
  ```

#	单实例守护进程：锁文件/var/run/\<name\>.pid    
#	启动脚本文件：	/etc/rc.d/rc.local  

  

11. 系统日志的书写  
  <u>***syslogd服务***</u>  
  <u>***系统日志格式由syslogd服务来控制***</u>  

  ```C
  #include <syslog.h>  
  void openlog(const char *ident, int option, int facility);  
  void syslog(int [riority, const char *format, ...);  
  void closelog(void);  
  ```
