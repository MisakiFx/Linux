#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
/* 文件读写中的原子性操作
 * 两个不同的进程操作同一个文件，他们有着不同的文件表项，因此有着不同的文件当前偏移量
 * 假设两个文件都要向末尾追加数据但是没有使用O_APPEND状态标志，而是在每次写入前都使用lseek进行偏移量更改就会出现问题
 * 假设两个进程都将偏移量更改至了末尾1500，然后第一个进程写入了100个数据，第二个进程再写100个数据的时候会变成
 * 从1500处开始写，导致覆盖第一个进程所写数据，并没有完成追加
 * 这主要是因为偏移量更改和文件修改没有形成原子性操作导致的，因此要想在文件末尾追加数据一定要在打开文件时加上O_APPEND状态标志
 * 在有这个标志的情况下每次读取或者修改数据都会将当前偏移量修改至文件末尾才会进行进一步操作，这期间是原子性的
 * */
/* 关于原子性操作这里还有两个函数可以完成原子性的偏移量更改并且读和写
 * ssize_t pread(int fildes, void *buf, size_t nbyte, off_t offset);
 * ssize_t pwrite(int fildes, const void *buf, size_t nbyte,off_t offset);
 * 这两个函数与read和write参数功能以及返回值一致，主要区别在第四个参数offset
 * 这两个函数会将偏移量置为offset在进行读写操作，期间是原子性的，并且不可打断。操作完成后也不会修改原有的偏移量的值。
 * */

int main()
{
  int fd = open("test.txt", O_CREAT | O_RDWR, 0664);
  if(fd < 0)
  {
    perror("open error:");
    return -1;
  }
  char buf[4096];
  lseek(fd, 1, SEEK_SET);
  //从这里可以看出pread是将偏移量置为offset，而不是加上offset
  int ret = pread(fd, buf, 4096, 1);
  std::cout << ret << std::endl;
  buf[ret] = '\0';
  std::cout << buf;
  //事实打印出来的当前偏移量并没有发生改变
  std::cout << lseek(fd, 0, SEEK_CUR) << std::endl;;
}
