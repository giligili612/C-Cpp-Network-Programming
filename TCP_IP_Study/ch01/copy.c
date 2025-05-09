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
