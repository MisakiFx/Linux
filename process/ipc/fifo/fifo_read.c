#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
  const char* file = "./test.fifo";
  int ret = mkfifo(file, 0664);
  if(ret < 0 && errno != EEXIST)
  {
    perror("mkfifo error");
    return -1;
  }
  int fd = open(file, O_RDONLY);
  if(fd < 0)
  {
    perror("open error");
    return -1;
  }
  printf("open fifo success\n");
  while(1)
  {
    char buf[1024];
    ret = read(fd, buf, 1023);
    if(ret < 0)
    {
      perror("read error");
      return -1;
    }
    if(ret == 0)
    {
      printf("wirte close\n");
      return -1;
    }
    printf("buf = [%s] : %d\n", buf, ret);
  }
  close(fd);
}
