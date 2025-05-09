# 第二章 套接字类型与协议设置
## 本章学习要求
- 套接字协议的定义与种类，不同套接字协议的特点
- 套接字的创建方法socket，及其参数意义

## socket函数使用及其参数
```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
//创建套接字函数：成功返回文件描述符，失败返回-1
//domain:使用的协议族标志，一般是PF_INET(IPv4协议族)
//      还有PF_INET6(IPv6)、PF_LOCAL(本地通信的Unix协议族)
//      PF_PACKET(底层套接字的协议族)、PF_IPX(IPX Novell协议族)
//type:协议族中的套接字类型。SOCK_STREAM(TCP)、SOCK_DGRAM(UDP)
//protocol:协议的最终选择。大部分情况下前两个参数就够了，一般这个参数会传0。但是如果同一协议族有多个数据传输方式相同的协议时这个参数会起作用。比如IPPROTO_TCP、IPPROTO_UDP等
```
# 课后习题选做
## 1.什么是协议？在收发数据中定义协议有何意义？
协议是为了完成数据交换而提前设定好的约定。<br>
收发数据中定义协议可以规范不同计算机在网络中的通信规则，如语言的语法一样，计算机网络协议就是计算机在网络中交流的语法。

## 2.面向连接的TCP套接字传输特性有3点，请分别说明。
- 可靠连接：只要连接在，数据不会丢失
- 数据按序传输
- 传输的数据不存在数据边界：传送方多次写入，接收方可以一次性接收

## 3.下列哪些是面向消息的套接字的特性？
a.传输数据可能丢失<br>
c.以快速传递为目标<br>
e.与面向连接的套接字不同，不存在连接的概念<br>

## 4.下列数据适合用哪类套接字传输？并给出原因
a.演唱会现场直播的多媒体数据：UDP，要保证直播的**实效性**，若用TCP可能会造成较大的时延，短暂卡顿无大碍<br>
b.某人压缩过的文本文件：要求速度时用UDP，数据完整性交由应用层解决。但更一般的是TCP确保文件**完整无误**<br>
c.网上银行用户与银行之间的数据传递：TCP，保证信息安全可靠无误的传递

## 5.何种类型的套接字不存在数据边界？这类套接字接收数据时需要注意什么？
面向消息的套接字（UDP）不存在数据边界：接收消息与发送消息的次数相同<br>
接收数据时大小有一定的限制，且需考虑数据丢失的情况

## 6.修改代码，让服务器端多次调用write传输数据，让客户端调用1次read进行读取。
```c
//tcp_client.c
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[1024];
    int str_len = 0;
    int idx = 0, read_len = 0;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>\n",argv[0]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    for(int i = 0; i < 3000; i++)
    {
        if(i % 100 == 0)
            printf("Wait time %d\n",i);
    }
    read_len = read(sock, &message, 1024);
    if(read_len == -1)
            error_handling("read() error!");
    str_len += read_len;
    
    char* ptr = message;
    for(int i = 0; i < read_len;)
    {
        printf("Message from server : %s \n", ptr);
        i += strlen(ptr) + 1;
        ptr = message + i;
    }
    
    printf("Function read call count: %d\n", read_len);
    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
```
```c
//tcp_client.c
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[1024];
    int str_len = 0;
    int idx = 0, read_len = 0;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>\n",argv[0]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    for(int i = 0; i < 3000; i++)
    {
        if(i % 100 == 0)
            printf("Wait time %d\n",i);
    }
    read_len = read(sock, &message, 1024);
    if(read_len == -1)
            error_handling("read() error!");
    str_len += read_len;
    //TCP会把多个字符串合并到一起，要读出后续字符串需要跳过每个字符串最后的\0
    char* ptr = message;
    for(int i = 0; i < read_len;)
    {
        printf("Message from server : %s \n", ptr);
        i += strlen(ptr) + 1;
        ptr = message + i;
    }
    
    printf("Function read call count: %d\n", read_len);
    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
```