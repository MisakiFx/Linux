//lseek函数，负责修改文件当前偏移量
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/* off_t lseek(int fd, off_t offset, int whence);
 * Arguments:
 * fd:操作的文件描述符
 * whence:可以有三种参数，SEEK_SET，SEEK_CUR，SEEK_END，分别代表相对位置文件开头，当前文件偏移量，文件结尾位置
 * offset:表示移动距离，offset可正可负
 * Return Value:成功返回更改后的文件当前偏移量，失败返回-1，如果当前fd是一个管道，套接字等不可修改的会将errno置为ESPIPE
 */
/* 1、在有些设备上文件当前偏移量是可以为负的。
 * 2、lseek不不引起IO操作，文件的当前偏移量存在内核中，lseek仅仅是修改它的值。
 * 3、文件偏移量是可以大于文件长度的，在这种情况下再进行写会形成文件空洞，未写入的空洞部分置为\0。
 * */
void Test1()
{
  int fd = open("test.txt", O_CREAT | O_TRUNC | O_RDWR, 0664);
  if(fd == -1)
  {
    perror("error:");
    return;
  }
  //通过以下这种方法可以获取当前的偏移量
  off_t curOffset = -1;
  curOffset = lseek(fd, 0, SEEK_CUR);
  //打印0，可知文件默认打开偏移量为0
  std::cout << curOffset << std::endl;
}
void Test2()
{
  std::string str1 = "123456\n";
  std::cout << str1.size() << std::endl;
  std::cout << str1;
}
int main()
{
  Test2();
}

