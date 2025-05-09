#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    struct sockaddr_in addr1, addr2;
    char* str_ptr;
    char str_arr[20];
    // 书上的源代码把第一个0省略了
    addr1.sin_addr.s_addr = htonl(0x01020304);
    addr2.sin_addr.s_addr = htonl(0x01010101);
    // 该函数已将网络字节序转换为字符串，并返回字符串地址，这个生成的字符串时函数内部生成的，若再次调用则新的字符串会把旧的字符串覆盖
    // 若之后还要调用该函数，则需要现在把它转换好的字符串保存下来
    str_ptr = inet_ntoa(addr1.sin_addr);
    strcpy(str_arr, str_ptr);
    printf("Dotted-Decimal notation1: %s \n", str_ptr);

    inet_ntoa(addr2.sin_addr);
    printf("Dotted-Decimal notation2: %s \n", str_ptr);
    printf("Dotted-Decimal notation3: %s \n", str_arr);
    return 0;
}