# 同步  

+ sync函数

  <u>***同步内核的buffer与cache	(commit buffer cache to disk)***</u>

  ```C
  #include <unistd.h>
  void sync(void);
  ```

+ fsync函数：同步一个文件的buffer

+ fdatasync函数：只刷数据，不刷亚数据

  > 数据：一个文件中的数据的有效内容
  > 亚数据：文件最后修改时间、文件数据等... 

+ fcntl()函数：文件描述符所变的魔术全都由fcntl实现  

  ```c
  int fcntl(int fd, int cmd, ... /* arg */ );
  ```

+ ioctl()函数：设备相关的内容都由ioctl来管理
	```C
	#include <sys/ioctl.h>
	int ioctl(int fd, unsigned long repuest, ...);
	```
+ /dev/fd目录：虚目录：显示当前进程的文件描述符信息——进程的镜子