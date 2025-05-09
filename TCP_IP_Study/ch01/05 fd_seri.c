#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
    int fd1, fd2, fd3;
    fd1 = socket(PF_INET, SOCK_STREAM, 0);
    fd2 = open("test.dat", O_CREAT|O_WRONLY|O_TRUNC);
    fd3 = socket(PF_INET, SOCK_DGRAM, 0);
    
    // 文件描述符从3开始递增，因为0,1,2都已经被分配给标准I/O了
     
    printf("file descriptor 1 : %d\n", fd1); //3
    printf("file descriptor 2 : %d\n", fd2); //4
    printf("file descriptor 3 : %d\n", fd3); //5
    close(fd1); close(fd2); close(fd3);
    return 0;
}