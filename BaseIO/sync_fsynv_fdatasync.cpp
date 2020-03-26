//sync syncfs fsync fdatasync函数介绍
#include <unistd.h>
#include <stdio.h>
#include <iostream>
/* 低级IO并不设立高速缓冲区，但是在向磁盘写入数据时会有磁盘缓冲区，用于暂存写入数据，然后按照队列顺序依次写入磁盘
 * 这种IO方式成为延时写
 * 在系统守护进程update进程中会不停调用一个sync函数来将内核中所有修改过的块缓冲区加入队列然后写入磁盘，这就保证了定期刷新缓冲区
 * void sync(void);
 * int syncfs(int fd);
 * sync将所有块缓冲区加入队列，syncfs将指定文件的块缓冲区加入队列，但并不等待磁盘写入结束，成功返回0，失败返回-1
 * 除次之外如果我们需要立刻将某些文件写入磁盘则还有其他函数供我们调用
 * fsync和fdatasync则会立刻将指定文件快缓冲区加入队列，并且等待其成功写入磁盘再返回
 * int fsync(int fd);
 * int fdatasync(int fd);
 * 他们之间的区别在于fsync等待更新完整个文件再返回，包括文件状态信息等，而fdatasync则仅仅等待更新完文件数据
 * 这些函数在同时有大量文件要进行磁盘写入的情况下才会有明显作用
 * */
int main()
{

}
