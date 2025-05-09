#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
//转换字节序函数示例，h是host，n是net，s是short，l是long
//网络中字节序全部都是大端序0x12345678
int main(int argc, char* argv[])
{
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("Host ordered port: %#x \n", host_port);
    printf("Network ordered prot: %#x \n",net_port);
    printf("Host ordered address: %#lx \n",host_addr);
    printf("Network ordered address: %#lx \n",net_addr);

    return 0;
}