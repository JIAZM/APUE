<font face="Simsun" size=4>

- [问题来源](#问题来源)
  - [***<u>那么是不是说在父进程和子进程中分别打印变量的地址空间就会得到不同的结果？</u>***](#u那么是不是说在父进程和子进程中分别打印变量的地址空间就会得到不同的结果u)
- [用一段代码测试一下](#用一段代码测试一下)
  - [问题来了，***fork出的子进程和父进程同名变量的地址也是相同的***](#问题来了fork出的子进程和父进程同名变量的地址也是相同的)
  - [深入内核或许可以得到更清晰的解答](#深入内核或许可以得到更清晰的解答)
- [<u>***以下为个人理解，欢迎指正***</u>](#u以下为个人理解欢迎指正u)



# 问题来源  

  > 一个现有的进程可以调用fork()函数来创建一个新进程  
  ```C
    #include <unistd.h>
    #include <sys/types.h>

    pid_t fork(void);
  ```
  由man手册，fork函数被调用一次，但返回两次，两次返回的区别是子进程的返回值是0，而父进程的返回值则是新建子进程的进程ID  

  ***<u>一个进程的子进程可以有多个，没有一个函数是一个进程可以获得其所有子进程的进程ID</u>***  
  ***<u>一个子进程之可能有一个父进程，子进程可以通过调用getppid()来获取其父进程的进程ID，子进程的进程ID不可能为0</u>***  

  **调用fork()函数之后，子进程成为父进程的副本（***<u>子进程获得父进程的数据空间、堆和栈的副本</u>***），子进程有独立的地址空间，不与父进程共享这些存储空间部分，父进程和子进程共享正文段**  

  ## ***<u>那么是不是说在父进程和子进程中分别打印变量的地址空间就会得到不同的结果？</u>***  



# 用一段代码测试一下
  ```C
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/types.h>
    #include <sys/wait.h>

    int main(int argc, char *argv[])
    {
        int s = 50;
        int i = 50;
        int pid = 0;
        int wstat = 0;

        if(!(pid = fork())){

            printf("Children i, s address before write: %p, %p \r\n", &i, &s);

            for(;i > 0;i--)
                printf("++ : %d \n\r", ++s);
            printf("Children Done ! \r\n");
            printf("Children i, s address after write: %p, %p \r\n", &i, &s);
            exit(0);
        }
        else{
            
            printf("Parent i, s address before write: %p, %p \r\n", &i, &s);

            for(;i > 0;i--)
                printf("-- : %d \n\r", --s);
            printf("Parent Done ! \r\n");
            printf("Parent i, s address after write: %p, %p \r\n", &i, &s);
        }

        printf("Children Pid : %d \r\n", pid);

        if(pid == wait(&wstat))
            printf("Children exit , pid.exit code : %d.%d \r\n", pid, wstat);
        else
            printf("wait() return.exit code : %d.%d \r\n", pid, wstat);
        
        exit(0);
    }
  ```
  考虑到fork函数采用了<u>***写时复制技术(Copy-On-Write, COW)***</u>，上文程序中每个进程使用两次printf输出变量i与s的地址值  
  得到的结果是这样的：
  ```shell
    $ ./forktest | grep address
    Children i, s address before write: 0x7fffb3800b6c, 0x7fffb3800b68 
    Children i, s address after write: 0x7fffb3800b6c, 0x7fffb3800b68 
    Parent i, s address before write: 0x7fffb3800b6c, 0x7fffb3800b68 
    Parent i, s address after write: 0x7fffb3800b6c, 0x7fffb3800b68
  ```
  ## 问题来了，***fork出的子进程和父进程同名变量的地址也是相同的***  
  > ***写时复制到底是什么意思***  
  调用fork时，允许父进程和子进程最初共享相同的页面来工作，这些共享页面标记为写时复制，这意味着如果任何一个进程写入共享页面，就创建共享页面的副本。不修改的页面可以由父进程和子进程继续共享  
  ***<u>内核为新生成的子进程创建虚拟空间结构，用来复制父进程的虚拟空间结构，但是不为这些段分配物理内存.父子进程中有更改相应段的行为发生之前，父子进程共享父进程的物理空间，发生更改相应段的行为时，在为子进程相应的段分配物理空间</u>***  

  ## 深入内核或许可以得到更清晰的解答  
  
  > 以下为linux4.15内核中内存管理结构体的定义
  ```C
    /* struct mm_struct 结构体在 include/linux/mm_types.h 中定义 */
    struct mm_struct {
        struct vm_area_struct *mmap;            /* 指向虚拟区域（VMA）链表 */
        struct rb_root mm_rb;                   /* 指向 red_black 树 */
        // 红黑树 管理 VMA 链表
        pgd_t * pgd;                            /* 指向进程的页目录表 */
        atomic_t mm_users;                      /* 用户空间中有多少用户 */
        atomic_t mm_count;                      /* 对 struct mm_struct 有多少引用 */
        int map_count;                          /* number of VMAs */
                                                /* 虚拟区间的个数 */
        struct list_head mmlist;                /* 所有活动 (active) mm的链表 */
        /* List of maybe swapped mm's.  These are globally strung
        * together off init_mm.mmlist, and are protected
        * by mmlist_lock
        */
        unsigned long start_code, end_code, start_data, end_data;
        unsigned long start_brk, brk, start_stack;
        unsigned long arg_start, arg_end, env_start, env_end;
    };
    struct vm_area_struct;
    /* 用来描述一个虚拟内存区域（VMA）
    * 内核将每一个内存区域作为一个单独的内存对象管理，内一个内存区域都有一致的属性，如权限
    * 所以程序的 代码段、数据段、BSS段 在内核里都分别由一个 struct vm_area_struct 结构体描述
    */
    struct vm_area_struct {
        struct mm_struct *vm_mm;        /* 虚拟内存区域所在的地址空间. */
        /* mm_struct 通过vm_area_struct 管理内存
        * 所以对于具体的 vm_area_struct 需要指出隶属与哪一个 mm_struct
        */

        unsigned long vm_start;         /* Our start address within vm_mm. */
        unsigned long vm_end;
        /* The first byte after our end address within vm_mm. */

        struct vm_area_struct *vm_next, *vm_prev;
        /* 每一个 mm_struct 中维护了一个 vm_area_struct 链表 */

        pgprot_t vm_page_prot;          /* Access permissions of this VMA. */
        unsigned long vm_flags;         /* Flags, see mm.h. */
        /* 虚拟内存区域的标志
        *
        * VM_READ      此虚拟内存区域可读
        * VM_WRITE     此虚拟内存区域可写
        * VM_EXEC      此虚拟内存区域可执行
        * VM_SHARED    此虚拟内存区域可共享
        * VM_IO        此虚拟内存区域映射设备IO空间
        */

        struct rb_node vm_rb;
        
        const struct vm_operations_struct *vm_ops;
        /* Function pointers to deal with this struct. */

        struct file * vm_file;          /* File we map to (can be NULL). */
        /* 可以将文件映射到 vm_start 处 */
    };
  ```

  # <u>***以下为个人理解，欢迎指正***</u>  
  
  > 所谓的“复制父进程的内存资源是复制了***VMA***因此子进程VMA中的成员值与父进程相同，但发生写入行为时VMA与实际物理内存的指向发生变化，***mm_struct***中向物理内存映射的成员发生变化，由MMU实现的内存映射保证了两进程中变量的虚拟地址值相同但实际物理地址是不同的  



</font>