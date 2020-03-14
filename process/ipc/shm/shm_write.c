#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#define IPC_KEY 0x12345678

int main()
{
  int shmid = shmget(IPC_KEY, 32, IPC_CREAT | 0664);
  if(shmid < 0)
  {
    perror("shmget error");
    return -1;
  }
  void* shmstart = shmat(shmid, NULL, 0);
  if(shmstart == (void*)-1)
  {
    perror("shmat error");
    return -1;
  }
  int i = 0;
  while(1)
  {
    sprintf((char*)shmstart, "%s-%d\n", "Misaki",i++);
    sleep(1);
  }
  shmdt(shmstart);
  shmctl(shmid, IPC_RMID, NULL);
}
