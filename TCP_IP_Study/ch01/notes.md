# 第1章 理解网络编程和套接字

## 本章学习要求：
- 代码：**跟着打一遍**，先不要纠结细枝末节，大概知道socket、open、write、bind、listen等函数是干什么的，与各些参数混个脸熟
- 定义与概念：
  - 套接字（作用、概念、构成、在网络通讯中如何运作的）

## Linux文件描述符(Linux中一切皆文件)

Linux文件描述符是系统分配给文件或套接字的**整数**<br>
| 文件描述符  | 对象
|:-----------:|:-------------------|
|     0       | 标准输入：Standard Input
|     1       | 标准输出：Standard Output
|     2       | 标准错误：Standard Error

## 知识补给站（以_t为后缀的数据类型）
**核心原因**：通过typedef定义与平台相关的别名（如size_t），隐藏底层基本类型的实现细节。当移植到不同位宽的系统时，只需调整这些别名背后的实际类型，无需修改业务代码。<br>

对于ssize_t、size_t等陌生的数据类型其实都是基本数据类型的别名。为何要另起名字呢？<br>

因为现今主流操作系统均采用了64位（绝大多数）、32位，而早先操作系统是16位，以前的int是16位，而现在一般是32位。基本数据的变动会对程序造成巨大的影响，若编程时仍使用基本数据类型会导致其难以移植，此时代码修改量巨大，相反如果用别名那么到时候只需要重新修改typedef的声明就可以重新编译移植了。**而_t的后缀是为了与程序员定义的数据类型作区分。**

# 课后习题选做

## 1.套接字在网络编程中的作用是什么？
套接字是网络数据传输用的软件设备，其构成为<ip,port>，即ip地址加上端口号。不同的计算机通过ip地址找到网络上的另一个计算机，再通过端口号来与该计算机上相应进程通讯，而套接字则包含了这一过程中最重要的两个部分，因此网络编程又称为套接字编程。

## 2.在服务器创建套接字后，会依次调用listen函数和accept函数。请比较二者作用。
**listen**：服务器转为可接受状态<br>
**accept**：服务器受理连接请求<br>
这二者在套接字连接中缺一不可，listen是accept的前置步骤，无listen不成服务器，无accept无法与外界交流

## 3.创建套接字后一般会给它分配地址，为什么？为了完成地址分配需要调用哪个函数？
ip地址是套接字的重要组成部分，其他计算机就是通过该ip地址与本计算机通信的。套接字本身是内核对象，在Linux中通过文件访问符访问(fd)<br>
分配地址调用的是bind函数

## 4.底层文件I/O函数与ANSI标准定义的文件I/O函数之间有何区别？
Deepseek：<br>
**最核心区别**：  
底层文件I/O（如`open`/`read`）是操作系统提供的**无缓冲系统调用**，直接操作文件描述符，适合高频大块数据或精细控制（如非阻塞I/O）。ANSI标准I/O（如`fopen`/`fread`）是C库封装的**带缓冲高阶接口**，通过`FILE*`流抽象减少系统调用，提升小文件操作效率，但牺牲部分灵活性。关键差异在于**缓冲机制**（标准库缓冲优化性能）和**抽象层级**（底层API更接近内核）。

## 5.参考本书给出的示例low_open.c和low_read.c，分别利用底层文件I/O和ANSI标准I/O编写文件复制程序。可任意指定复制程序的使用方法。
```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define BUF_SIZE 100
void error_handling(char* message);
void kernel_IO()
{
    int fd;
    char buf[BUF_SIZE];
    //读
    fd = open("data.txt", O_RDONLY);
    if(fd == -1)
        error_handling("kernel read open() error!");

    ssize_t bytes_read = read(fd, buf, sizeof(buf));
        
    close(fd);
    //写,最后的数字是设置文件权限，没有的话必须用chmod手动设置文件权限才可以读取
    fd = open("to.txt", O_CREAT|O_WRONLY|O_TRUNC, 0644);
    if(fd == -1)
        error_handling("kernel write open() error!");
    printf("kernel copy.\n");

    if(write(fd, buf, bytes_read) == -1)
        error_handling("kernel write() error!");
    close(fd);
}

void ANSI_IO()
{
    FILE* fp;
    char buf[BUF_SIZE];
    //读
    fp = fopen("data.txt", "r");
    if(fp == NULL)
        error_handling("ANSI fopen() error!");
    fread(buf, sizeof(char), BUF_SIZE, fp);
    fclose(fp);

    //写
    fp = fopen("to.txt", "wb");
    if(fp == NULL)
        error_handling("ANSI fopen() error!");
    printf("ANSI copy.\n");

    fwrite(buf, sizeof(char), BUF_SIZE, fp);
    fclose(fp);
}


int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("Usage: %s <1|2>\n", argv[0]);
        printf("1: kernel_IO, 2: ANSI_IO\n");
        return 1;
    }

    if(atoi(argv[1]) == 1)
        kernel_IO();
    else
        ANSI_IO();
    
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

```