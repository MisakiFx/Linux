#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
  kill(getpid(), 3);
  while(1)
  {
    printf("Misaki\n");
    sleep(1);
  }
}
