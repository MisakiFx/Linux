//dup和dup2重定向函数
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>

/* 重定向
 * int dup(int oldfd);
 * int dup2(int oldfd, int newfd);
 * dup总是用当前最小未使用的fd带替代newfd，其它功能与dup2一致
 * 让文件描述符表中newfd的项中的文件表项指针更改为oldfd项中文件表项指针
 * 若newfd原先有指向文件并且已经打开则关闭，若newfd == oldfd则直接返回newfd
 * Arguments:
 * oldfd:旧文件描述符
 * newfd:新文件描述符
 * Return Value:成功返回新文件描述符，失败返回-1
 * dup和dup2都是原子性的
 * */

int main()
{
  //一个进程执行时自动打开0,1,2三个文件描述符，作为标准输入标准输出标准错误
  //在这里我们重定向的新文件描述符自动更新为3
  //并且文件描述符重定向旧文件描述符依然可以用，且指向文件不变
  int newfd = dup(1);
  std::cout << newfd << std::endl;
  write(newfd, "123\n", 4);
  write(STDOUT_FILENO, "123\n", 4);
}
