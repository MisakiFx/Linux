/*实现条件变量的基本使用*/                                           
/*吃面前提有人吃面，如果没有线程的面，等待老板做出来
 *老板做出来面就要唤醒顾客
 *老板不会做太多的面，老板只会提前做一碗面         
 *如果已经有面做出来，但是没人吃，不会再做（等待）
 *顾客吃完面后，唤醒老板的等待*/
/*条件变量的使用练习*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
pthread_mutex_t mutex;
pthread_cond_t hall_foodie;
pthread_cond_t kitchen_chef;
int isHavaNoodles = 0;//默认没有面条
void* thr_chef(void* arg)
{
  while(1)
  {
    //没面则做面，有面没人吃则等待
    pthread_mutex_lock(&mutex);
    while(isHavaNoodles == 1)
    {
      pthread_cond_wait(&kitchen_chef, &mutex);
    }
    printf("chef made a noodle\n");
    isHavaNoodles = 1;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&hall_foodie);
  }
}
void* thr_foodie(void* arg)
{
  while(1)
  {
    //有面则吃面，没面则等待
    pthread_mutex_lock(&mutex);
    while(isHavaNoodles == 0)
    {
      pthread_cond_wait(&hall_foodie, &mutex);
    }
    printf("emmm,delicious!\n");
    isHavaNoodles = 0;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&kitchen_chef);
  }
}
int main()
{
  int ret;
  pthread_t ctid[4], etid[4];
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&hall_foodie, NULL);
  pthread_cond_init(&kitchen_chef, NULL);
  for(int i = 0; i < 4; i++) 
  {
    ret = pthread_create(&ctid[i], NULL, thr_chef, NULL);
    if(ret != 0)
    {
      printf("creat chef error\n");
      return -1;
    }
  }
  for(int i = 0; i < 4; i++) 
  {
    ret = pthread_create(&etid[i], NULL, thr_foodie, NULL);
    if(ret != 0)
    {
      printf("creat foodie error\n");
      return -1;
    }
  }
  for(int i = 0; i < 4; i++) 
  pthread_join(ctid[i], NULL);
  for(int i = 0; i < 4; i++) 
  pthread_join(etid[i], NULL);
  pthread_cond_destroy(&hall_foodie);
  pthread_cond_destroy(&kitchen_chef);
}
