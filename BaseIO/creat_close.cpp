#include <iostream>
#include <fcntl.h>
#include <unistd.h>
/* int creat(const char *pathname, mode_t mode);
 * 用于新建一个文件，功能与open(pathname, O_CREAT | O_WRONLY | O_TRUNC, mode)完全一致
 * 成功返回文件描述符，失败返回-1
 */ 
/* int close(int fd);
 * close用于关闭一个文件，成功返回0，失败返回-1
 */ 
//当一个进程结束时，系统会自动关闭其打开的所有文件，以及记录锁
int main()
{
  int fd1 = open("test1.txt", O_CREAT | O_WRONLY | O_TRUNC, 0664);
  int fd2 = creat("test2.txt", 0664);
  close(fd1);
  close(fd2);
}
