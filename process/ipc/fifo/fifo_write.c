#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main()
{
  const char* file = "./test.fifo";
  int ret = mkfifo(file, 0664);
  if(ret < 0 && errno != EEXIST)
  {
    perror("mkfifo error");
    return -1;
  }
  int fd = open(file, O_WRONLY);
  if(fd < 0)
  {
    perror("open error");
    return -1;
  }
  printf("open fifo success\n");
  while(1)
  {
    char buf[1024];
    scanf("%s", buf);
    ret = write(fd, buf, strlen(buf));
    if(ret < 0)
    {
      perror("write error");
      return -1;
    }
    printf("buf = [%s] : %d\n", buf, ret);
  }
  close(fd);
}
