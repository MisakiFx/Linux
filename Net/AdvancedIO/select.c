#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
  fd_set set;
  FD_ZERO(&set);//清空集合
  FD_SET(0, &set);//将标准输入添加监控
  int maxfd = 0;//最大集合数
  while(1)
  {
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    FD_SET(0, &set);
    int nfds = select(maxfd + 1, &set, NULL, NULL, &tv);
    if(nfds < 0)
    {
      printf("select error\n");
      return -1;
    }
    else if(nfds == 0)
    {
      printf("wait timeout\n");
      continue;
    }
    printf("input.....ok\n");
    for(int i = 0; i <= maxfd; i++)
    {
      char buf[1024];
      int ret = read(i, buf, 1023);
      if(ret < 0)
      {
        printf("read error\n");
        return -1;
      }
      printf("get buf:[%s]\n", buf);
    }
  }
}
