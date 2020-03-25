//read和write函数介绍
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
/* 关于低级IO缓冲区大小的选取，在Linux ext4文件系统上，一个磁盘块的大小为4k，所以一次至少读取一个磁盘块大小的IO效率是最高的
 * */
#define BUFSIZE 4096
/* size_t read(int fildes, void *buf, size_t nbyte);
 * 从文件中读取数据
 * Arguments:
 * fildes:读取的文件描述符
 * buf:数据存放的目标缓冲区
 * nbyte:最多读取的数据长度，16位无符号整型，一次读取最多为65535个字节
 * Return Value:
 * 返回实际读取的数据长度，大部分情况下目标文件中有多少数据则读取多少数据并且返回读取长度；
 * 如果是管道或者套接字目前暂无数据则会阻塞
 * 如果是普通文件，读到文件结尾返回0
 * 可以设置非阻塞读取，如果暂无数据则不会阻塞而回返回-1并将errno置为EAGAIN
 * 返回值ssize_t是一个带符号整形
 * */
/* ssize_t write(int fildes, const void *buf, size_t nbyte);
 * 向文件中写入数据
 * Argumentes:
 * fildes:文件描述符
 * buf:写入数据存放的缓冲区
 * nbyte:写入的最长数据长度
 * Return Value:
 * 返回实际写入的数据长度，如果数据长度小于nbyte则在后补0；如果文件剩余容量小于nbyte则返回能写入的最大数据长度
 * */
/* read和write参数和返回值都类似，但是对于不同的文件描述符例如pipe，socket等都有不同的返回方法或者异常处理方法
 * */
void Test1()
{
  int fd = open("test.txt", O_CREAT | O_RDWR | O_TRUNC, 0664);
  if(fd < 0)
  {
    perror("error:");
    return;
  }
  int ret = write(fd, "Misaki", 7);
  std::cout << ret << std::endl;
  lseek(fd, 0, SEEK_SET);
  char buf[1024] = {0};
  ret = read(fd, buf, 1024);
  std::cout << ret << std::endl;
  std::cout << buf << std::endl;
}
bool Test2()
{
  int n = 0;
  char buf[BUFSIZE];
  while((n = read(STDIN_FILENO, buf, 4096)) > 0)
  {
    if(write(STDOUT_FILENO, buf, n) != n)
    {
      perror("write error:");
      return false;
    }
  }
  if(n < 0)
  {
    perror("read error:");
    return false;
  }
  return true;
}
int main()
{
  if(Test2() == false)
  {
    std::cerr << "copy error" << std::endl;
    return -1;
  }
}
