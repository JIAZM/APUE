# 进程间通信

+ 同一台主机
+ 不同主机间

+ 通信的两个进程是否有亲缘关系

---

## 两个进程间数据交换

> 1.pipe(管道)
>
> 2.XSI IPC	-›	SysV IPC
>
> 3.网络套接字socket

1. 管道 - 由内核维护，单工，有自同步机制(迁就速度慢的一方)

   > 管道
   >
   > 1.速度上自动同步最慢的操作
   >
   > 2.管道必须凑齐读写双方才能操作
   + 匿名管道：在文件系统不存在

     ```C
     #include <unistd.h>
     int pipe(int pipefd[2]);
     //参数pipefd[0]为读端
     //参数pipefd[1]为写端     
     ```

   + 命名管道：磁盘上文件类型为P的文件
   
     > 没有亲缘关系的进程无法使用匿名管道
     
     ```C
     #include <sys/types.h>
     #include <sys/stat.h>
     int mkfifo(const char *pathname, mode_t mode);
     ```
     
     