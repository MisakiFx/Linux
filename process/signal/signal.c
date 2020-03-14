#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigcb(int signum)
{
  printf("signum = %d\n", signum);
}
int main()
{
  signal(SIGINT, sigcb);//忽略
  signal(SIGQUIT, sigcb);//忽略
  while(1)
  {
    printf("Misaki\n");
    sleep(1);
  }
}
