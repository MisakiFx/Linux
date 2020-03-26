//fcntl函数介绍
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
/* int fcntl(int fd, int cmd, ...);
 * fcntl函数可以更改已经打开的文件的属性
 * Arguments:
 * fd:文件描述符
 * cmd:执行命令
 * ...:不定参数，后面有可能会根据cmd的不同有着不同的需要传递的参数
 * Return Value:返回值根据cmd的不同也不同，但是失败都会返回-1，大部分设置为主的模式成功会返回0
 * 常用cmd:
 * F_DUPFD:赋值文件描述符，dup底层就是用这个参数进行实现的，它会将第三个参数起最小未使用的描述符复制为fd所指文件
 * F_DUPFD_CLOEXEC:这里涉及一个文件描述符标志，FD_CLOEXEC，这也是唯一一个文件描述符标志，当被定义了这个文件描述符标志的文件
 * 当当前进程在exec进程替换时会自动关闭这个文件，防止子进程一直占用，多用于只需要父进程可以使用这个文件而子进程关闭这个文件的文件上
 * FD_CLOEXEC也可以通过F_SETFD模式进行设置，F_DUPFD_CLOEXEC则与F_DUPFD功能以及参数类似，不同的是会自动为newfd设置FD_CLOEXEC标志
 * F_GETFD:获得对应于fd的文件描述符标志FD_CLOEXEC作为返回值返回
 * F_SETFD:对于fd设置新的文件描述符标志，新标志作为第三个参数传入
 * F_GETFL:对于fd获得文件状态标志，例如O_RDWR之类的称为文件状态标志，但是一个文件中的O_RDWR,O_WRONLY,O_RDONLY,O_EXEC,o_SEARCH是互斥的
 * 因此可以使用O_ACCMODE获得访问方式屏蔽位
 * F_SETFL:对于fd设置文件状态标志
 * */

//模拟dup以及dup2，这里并未实现原子性操作
int my_dup(int oldfd)
{
  return fcntl(oldfd, F_DUPFD, 0);
}
int my_dup2(int oldfd, int newfd)
{
  close(newfd);
  return fcntl(oldfd, F_DUPFD, newfd);
}
//F_SETFD和F_GETFD使用
int GetFd(int fd)
{
  int FD = fcntl(fd, F_GETFD);
  return FD;
}
int SetFd(int fd)
{
  //在设置标志为时不论是F_SETFD还是F_SETFL都要先获得之前的标志位然后与其进行相与，在设置，不然会将之前的设置清零
  int flags = fcntl(fd, F_GETFD);
  return fcntl(fd, F_SETFD, flags | FD_CLOEXEC);
}
void Test()
{
  int fd = open("test.txt", O_CREAT | O_RDWR, 0664);
  if(fd < 0)
  {
    perror("open error:");
    return;
  }
  std::cout << SetFd(fd) << std::endl;
  std::cout << GetFd(fd) << std::endl;
}
//打印文件状态标志
void GetState(int fd)
{
  std::cout << fd << std::endl;
  int flags = fcntl(fd, F_GETFL);
  if(flags < 0)
  {
    perror("fcntl error:");
    return;
  }
  //用屏蔽字获取当前状态标志
  switch(flags & O_ACCMODE)
  {
    case O_WRONLY:
      std::cout << "write only" << std::endl;
      break;
    case O_RDONLY:
      std::cout << "read only" << std::endl;
      break;
    case O_RDWR:
      std::cout << "read write" << std::endl;
      break;
    default:
      std::cerr << "unknow mode" << std::endl;
      break;
  }
  if(flags & O_NONBLOCK)
  {
    std::cout << "nonblock" << std::endl;
  }
  if(flags & O_APPEND)
  {
    std::cout << "append" << std::endl;
  }
  //一个文件描述符就算设置了SYNC同时写系统也不一定一定会按照预期进行同时写，因此程序员有必要调用fsync()函数
  if(flags & O_SYNC)
  {
    std::cout << "sync" << std::endl;
  }
}

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    std::cerr << "use ./main <fd> << std::endl";
    return -1;
  }
  GetState(atoi(argv[1]));
}
