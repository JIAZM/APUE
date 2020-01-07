# 文件描述符fd

> 整形数->数组下标 文件描述符优先选择当前可用的下标中最小的  
> 流stream 数组中第0,1,2个位置分别是stdin,stdout,stderr  

文件共享：多个任务共同处理同一个文件  
补充函数：truncate / ftruncate  
IO的效率问题  

> 原子操作：不可分割的操作  
> 原子：不可分割的做小单位  
> 原子操作的作用：解决竞争和冲突  

+ 程序中的重定向:dup / dup2  

+ 文件属性  
```C
#include <sys/types.h>  
#include <sys/stat.h>  
#include <unistd.h>  
int stat(const char *pathname, struct stat *statbuf);  
	//通过文件路径获取属性，面对符号链接文件时获取的是指向的目标文件的属性  
int fstat(int fd, struct stat *statbuf);  
	//通过文件描述符获取文件属性  
int lstat(const char *pathname, struct stat *statbuf);  
	//面对符号链接文件时获取的是符号链接文件阿德属性，而并非时目标文件的属性  
```
+ 空洞文件 - 文件中全是acsii码为无效字符的文件  

```C
off_t     st_size;        /* Total size, in bytes */
blksize_t st_blksize;     /* Block size for filesystem I/O */
blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */
```
<u>***cp命令支持空洞文件拷贝***</u>  

+ 文件系统 FAT / UFS(UNIX file system)  
	> 文件系统： 文件或数据的存储和管理  
	> FAT16/32:静态存储的单链表  