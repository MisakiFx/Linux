//匿名管道
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
  int pipefd[2];
  int ret = pipe(pipefd);
  //创建失败
  if(ret < 0)
  {
    perror("pipe error:");
    return -1;
  }
  int pid = fork();
  if(pid < 0)
  {
    perror("fork error:");
    return -1;
  }
  else if(pid == 0)
  {
    //pause();
    close(pipefd[1]);
    //close(pipefd[0]);
    char buf[1024] = {0};
    //管道中无数据则阻塞读取知道有数据为止
    //写端全部关闭没东西可读返回0
    int ret = read(pipefd[0], buf, 1023);
    printf("buf = %s - %d\n", buf, ret);
  }
  else 
  {
    //sleep(1);
    //close(pipefd[1]);
    close(pipefd[0]);
    const char* ptr = "Misaki";
    //管道自带同步与互斥对管道的操作大小不超过PIPE_BUF，保证操作的原子性
    //互斥保证安全，同步保证合理
    //管道写满了，write也会阻塞，直到有空间为止，大概64k
    //int total = 0;
    //while(1)
    //{
    //  int ret = write(pipefd[1], ptr, strlen(ptr));
    //  total += ret;
    //  printf("total = %d\n", total);
    //}
    //读端全部关闭，触发异常，进程退出
    int ret = write(pipefd[1], ptr, strlen(ptr));
    printf("write %d\n", ret);
    //printf("errno:%d\n", errno);
    //printf("%s\n", strerror(errno));
  }
  //while(1)
  //{
  //  printf("-----%d\n", getpid());
  //  sleep(1);
  //}
}
