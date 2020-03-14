#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

struct sigaction newact;
struct sigaction oldact;
void sigcb(int signum)
{
  printf("signum = %d\n", signum);
  sigaction(signum, &oldact, NULL);
}
int main()
{
  newact.sa_handler = sigcb;
  sigemptyset(&newact.sa_mask);
  sigaction(SIGINT, &newact, &oldact);
  while(1)
  {
    printf("Misaki\n");
    sleep(1);
  }

}
