//read函数介绍
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

/* size_t read(int fildes, void *buf, size_t nbyte);
 * 从文件中读取数据
 * Arguments:
 * fildes:读取的文件描述符
 * buf:数据存放的目标缓冲区
 * nbyte:最多读取的数据长度，16位无符号整型，一次读取最多为65535个字节
 * Return Value:
 * 返回实际读取的文件长度，大部分情况下目标文件中有多少数据则读取多少数据并且返回读取长度；
 * 如果是管道或者套接字目前暂无数据则会阻塞
 * 如果是普通文件，读到文件结尾返回0
 * 可以设置非阻塞读取，如果暂无数据则不会阻塞而回返回-1并将errno置为EAGAIN
 * 返回值ssize_t是一个带符号整形
 * */
int main()
{
  int fd = open("test.txt", O_CREAT | O_RDWR | O_TRUNC, 0664);
  if(fd < 0)
  {
    perror("error:");
    return -1;
  }
  int ret = write(fd, "Misaki", 7);
  std::cout << ret << std::endl;
  lseek(fd, 0, SEEK_SET);
  char buf[1024] = {0};
  ret = read(fd, buf, 1024);
  std::cout << ret << std::endl;
  std::cout << buf << std::endl;
}
