//介绍open和openat函数
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
/*open函数均为打开文件，若成功返回文件描述符，失败返回-1
 *int open(const char *pathname, int flags);                
 *int open(const char *pathname, int flags, mode_t mode);
 * Arguments:
 * path:打开文件或创建文件的名字，
 * flags:表示选项，用|连接多个选项
 * flags选项宏的定义文件在每个系统中有所不同，Linux中定义在fcntl-linux.h文件中
 * mode参数仅在使用部分选项时才用到，例如O_CREAT在mode中需要给定文件初始权限
 * Rules:
 * 文件描述符分配规则最小未使用
 */
/* int openat(int dirfd, const char *pathname, int flags);
 * int openat(int dirfd, const char *pathname, int flags, mode_t mode);
 * openat函数与open函数功能类似，唯独多出dirfd参数用以区分功能
 * openat函数解决的主要问题是
 * 1、可以让同一进程下的多个线程之间拥有不同的当前工作目录，从而可以使用同样的相对路径打开绝对路径可能不同的文件
 * 2、解决TOCTTOU(Time Of Check To Time Of Use)。如果两个文件相关的系统调用互相依赖，则这个系统是脆弱的
 * openat函数主要特性
 * 1、如果pathname是绝对路径，那么此时dirfd参数毫无意义，功能则与open一致
 * 2、如果pathname是相对路径，且dirfd参数不是特殊宏AT_FDCWD，则将dirfd所在的目录作为此时的当前工作目录，以此打开相对路径的文件
 * 3、如果pathname是相对路径，且dirfd参数未特殊宏AT_FDCWD，则就以当前工作目录打开相对路径的文件，功能与open无异
 */ 
int main()
{
  //测试文件是否截断
  //如果使用一个系统调用如open创建一个新文件的文件名大于NAME_MAX，有的系统会选择截断，而有的系统选择返回-1报错
  //如果_POSIX_NO_TRUNC值为真则返回-1报错，为假则对文件名进行截断，并且成功创建
  std::cout << pathconf(".", _PC_NO_TRUNC) << std::endl;;
  //Linux操作系统下的处理为报错返回，并将errno置为ENAMETOOLONG
}

