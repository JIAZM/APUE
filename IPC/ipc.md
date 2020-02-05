# 进程间通信

+ 同一台主机
+ 不同主机间

+ 通信的两个进程是否有亲缘关系
  > 有亲缘关系的进程间通信 - 使用mmap  
  
  <u>***协议 - 通信双方约定的对话的格式***</u>  

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
   + 匿名管道：在文件系统不存在（ls命令看不到）
     ```C
     #include <unistd.h>
     int pipe(int pipefd[2]);
     //参数pipefd[0]为读端
     //参数pipefd[1]为写端
     //类比 tmpfile()函数
     ```
     在具有亲缘关系的进程间通信非常方便

   + 命名管道：磁盘上文件类型为P的文件
     > 没有亲缘关系的进程无法使用匿名管道
     ```C
     #include <sys/types.h>
     #include <sys/stat.h>
     int mkfifo(const char *pathname, mode_t mode);
     ```
2.  XSI -> SysV  
    > IPC -> Inter-Process Communication  
    > 主动端　(先发包的一端)　＆　被动端　(先收包的一端)　　

    ```shell
    $ipcs  #查看当前消息队列
    ```

    + 三种通信方式(有没有亲缘关系的进程间通信都能用)
      + Message Queues          (msqid)
      + Semaphore Arrays        (semid)
      + Shared Memory Segments  (shmid)
    + key - 确定通信双方拿到同一个消息队列(通信机制)
      + 函数：ftok();
        ```C
        #include <sys/types.h>
        #include <sys/ipc.h>

        key_t ftok(const char *pathname, int proj_id);
        //从文件inod着手

        /*
         * XSI函数族命名规律
         * xxxget(创建)  xxxop(使用)   xxxctl(其他控制 / 销毁)
         * xxx -> msg sem shm   (三种机制)
         */
        ```
        ```mermaid
        graph LR
            xxx_keyfile&proj_id --ftok--> key
        ```
    + Messege Queue
      > 消息队列 - 双工  
      > 有缓存消息的能力　-　缓存空间见    $ulimit -a
      + msgget()
      ```C
        #include <sys/types.h>
        #include <sys/ipc.h>
        #include <sys/msg.h>

        int msgget(key_t key, int msgflg);
      ```
      + msgop
      ```C
        #include <sys/types.h>
        #include <sys/ipc.h>
        #include <sys/msg.h>

        int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

        ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);

        struct msgbuf {
               long mtype;       /* message type, must be > 0 */
               char mtext[1];    /* message data */
        };
      ```
      + msgctl()
      ```C
        #include <sys/types.h>
        #include <sys/ipc.h>
        #include <sys/msg.h>

        int msgctl(int msqid, int cmd, struct msqid_ds *buf);
      ```