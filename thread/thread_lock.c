/*互斥锁的练习*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int ticket = 100;
pthread_mutex_t mutex;
void* ticket_scalper(void* arg)
{
  int id = (int)arg;
  while(1)
  {
    //加锁要在临界资源访问之前
    //int pthread_mutex_lock(pthread_mutex_t* mutex);阻塞加锁
    //int pthread_mutex_trylock(pthread_mutex_t* mutex);非阻塞加锁，加不上锁就返回
    pthread_mutex_lock(&mutex);
    if(ticket > 0)
    {
      printf("scalper:%d--get a ticket:%d\n", id, ticket);
      ticket--;
      usleep(1000);
    }
    else 
    {
      //解锁
      pthread_mutex_unlock(&mutex);
      pthread_exit(0);
    }
    //解锁
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}
int main()
{
  int i = 0;
  int ret;
  pthread_t tid[4];
  //初始化互斥锁
  //int pthread_mutex_init(pthread_mutex_t *restrict mutex,
  //         const pthread_mutexattr_t *restrict attr);
  //             
  pthread_mutex_init(&mutex, NULL);
  //创建线程
  for(i = 0; i < 4; i++)
  {
    ret = pthread_create(&tid[i], NULL, ticket_scalper, (void*)i);
    if(ret != 0)
    {
      perror("thread creat error:");
      return -1;
    }
  }
  for(i = 0; i < 4; i++)
  {
    pthread_join(tid[i], NULL);
  }
  //销毁互斥锁
  //int pthread_mutex_destroy(pthread_mutex_t *mutex);
  pthread_mutex_destroy(&mutex);
}

