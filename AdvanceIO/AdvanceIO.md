# 高级IO
>阻塞IO -- 非阻塞IO  
>
><u>***没有简单的程序，只有头脑简单的程序员***</u>  

---

<u>*man手册第七章是最全的一章*</u>  

## <u>***深入了解一下假错***</u>  

## 非阻塞IO - 有限状态机  

1. 非阻塞IO
   + 数据中继
   
   + 简单流程
     
     + 一个程序的自然流程是结构化的
     
   + 复杂流程
     
     + 一个程序的自然流程不是结构化的
     
   + ### <u>***使用dia工具绘制流程状态图***</u>  
   
   + ***中继引擎实现***  
   
     + <u>***仔细阅读  “有限状态机 ”  与  “中继引擎 ”  实现代码 ！！！***</u>  
       + 中继引擎只是一个IO密集型的任务，而不能算是负载重的程序
   
2. IO多路转接

   1. ***<u>select() - 高级IO - IO多路转接</u>***

      > 由于存在忙等一直假错，CPU占用高

      监视文件描述符的行为 - 当每一个文件描述符发生感兴趣的动作的时候才进行操作

      + 布置监视任务

      + 开始监视

      + 可以监视多个文件描述符

   ```C
   select();	//非常古老很多都兼容，但是有错误 ； 以事件为单位来组织文件描述符
   poll();		//以文件描述符为单位来组织事件 ； 移植性与效率都不错
   epoll();	//以poll()为基础重新组织的方言，目的为提高效率，方言，不能移植
   ```

   <u>***回忆，sleep()不太靠谱的原因：***</u>  

   + 有些平台对sleep()的实现使用了alarm(),程序中不能出现两个时钟  

   ---

   ```C
   #include <sys/select.h>
   
   int select(int nfds, fd_set *readfds, fd_set *writefds,
              fd_set *exceptfds, struct timeval *timeout);
   /*
    *nfds监视的文件描述符最大的加1
    *readfds关心的可以发生读的文件描述符
    *writefds关心的可写的文件描述符
    *exceptfds关心的异常情况的文件描述符
    *timeout超时设置，如果不进行超时设置，则变成死等
    *
    *返回值：感兴趣的行为的文件描述符的个数，失败返回-1；超时返回假错
    */
   ```

   + ***<u>select()函数中文件描述符集没有const修饰，意味着在selecr函数中用户设置的readfds, writefds, exceptfds会被修改，select函数返回时readfds, writefds, exceptfds变成存放结果的地方</u>***  
   + ***<u>如果select被信号打断，则readfds, writefds, exceptfds为空，只能重新布置任务，再实现监视</u>***  
   + 步骤：
     1. 布置监视任务
     2. 查看监视结果
     3. 根据监视结果触发行为

   ## ***select的问题***  

   + select布置监视任务和存放监视结果的集合重叠 - 监视现场和监视结果存放在同一块空间
   + nfds类型为int，在一个进程中能打开的文件描述符的个数其实不止65536
   + 以事件为单位组织文件描述符，事件太单一，只有三种状态

   

   2. ***<u>poll - IO多路转接 - 在文件描述符的基础上等待事件</u>***  

      ```C
      #include <poll.h>
      int poll(struct pollfd *fds, nfds_t fds, int timeout);
      //第一个参数fds:结构体数组的首地址
      //timeout:单位为毫秒，-1为阻塞，0为非阻塞，具体时间为超时时间设置
      //返回值：成功为0，失败为-1，存在假错情况
      
      struct pollfd{
          int fd;
          short events;	//关心事件
          short revents;	//返回事件
      };
      ```

      + poll中第一个输入参数的数组元素内容的赋值可以提到循环之前完成

3. 其他读写函数

   ```C
   #include <sys/uio.h>
   /*
    *操作空间不是一块连续的地址空间，将多个碎片的小地址写进同一个文件中去
    */
   ssize_t readv(int fd, const struct iovec *io, int iovcnt);
   ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
   ssize preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset);
   ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset);
   ssize_t preadv2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags);
   ssize_t pwritev2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags);
   /**********iovec结构体**********/
   	struct iovec{
           void *iov_base;
           size_t iov_len;
       };
   
   /***************readn & writen***************/
   //APUE作者自己杜撰出来的两个功能
   //不是发布出来的标准函数
   //其实就是坚持读够/写够n个字节的功能
   ```

4. 存储映射IO

   + 函数

     ```C
     #include <sys/mman.h>
     //从fd文件的offset偏移量开始映射length个字节到addr地址
     void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
     
     /*
      *flags参数--位图
      *MAP_ANONYMOUS 匿名映射
      */
     int munmap(void *addr, size_t length);
     ```

   + 使用mmap函数进行进程间通信

     > 进程间通信存在竞争问题，需要有协议来规避

   

5. 文件锁

   > 通过文件描述符对文件加锁，是反映在inode层面，而不是文件结构体的层面
   >
   > 加锁后通过另外一个文件描述符close文件可能会造成文件意外解锁

   ```C
   #include <sys/file.h>
   
   int flock(int fd, int operation);
   
   #include <unistd.h>
   int lockf(int fd, int cmd, off_t len);
   ```