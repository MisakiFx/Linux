/*条件变量的使用练习*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
int isHavaNoodles = 0;//默认没有面条
void* thr_chef(void* arg)
{
  while(1)
  {
    //没面则做面，有面没人吃则等待
    if(isHavaNoodles == 1)
    {
      continue;
    }
    printf("chef made a noodle\n");
    isHavaNoodles = 1;
  }
}
void* thr_foodie(void* arg)
{
  while(1)
  {
    //有面则吃面，没面则等待
    if(isHavaNoodles == 0)
    {
      continue;
    }
    printf("emmm,delicious!\n");
    isHavaNoodles = 0;
  }
}
int main()
{
  int ret;
  pthread_t ctid, etid;
  ret = pthread_create(&ctid, NULL, thr_chef, NULL);
  if(ret != 0)
  {
    printf("creat chef error\n");
    return -1;
  }
  ret = pthread_create(&etid, NULL, thr_foodie, NULL);
  if(ret != 0)
  {
    printf("creat foodie error\n");
    return -1;
  }
  pthread_join(ctid, NULL);
  pthread_join(etid, NULL);
}
