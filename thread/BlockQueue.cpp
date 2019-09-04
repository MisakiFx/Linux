/**
 * 基于互斥锁与条件变量实现一个线程安全的队列
 * 实现生产者与消费者模型
 **/
#include <iostream>
#include <queue>
#include <pthread.h>
#define MAXQ 10
class BlockQueue
{
public:
  BlockQueue(int maxq = MAXQ)
    :_capacity(maxq)
  {
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond_consumer, NULL);
    pthread_cond_init(&_cond_productor, NULL);
  }
  ~BlockQueue()
  {
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_cond_consumer);
    pthread_cond_destroy(&_cond_productor);
  }
  bool QueuePush(int data)
  {
    pthread_mutex_lock(&_mutex);
    while(_queue.size() == _capacity)
    {
      pthread_cond_wait(&_cond_productor, &_mutex);
    }
    _queue.push(data);
    pthread_mutex_unlock(&_mutex);
    pthread_cond_signal(&_cond_consumer);
    return true;
  }
  bool QueuePop(int &data)
  {
    pthread_mutex_lock(&_mutex);
    while(_queue.empty())
    {
      pthread_cond_wait(&_cond_consumer, &_mutex);
    }
    data = _queue.front();
    _queue.pop();
    pthread_mutex_unlock(&_mutex);
    pthread_cond_signal(&_cond_productor);
    return true;
  }
private:
  std::queue<int> _queue;
  int _capacity;
  pthread_mutex_t _mutex;
  pthread_cond_t _cond_productor;
  pthread_cond_t _cond_consumer;
};
void* thr_consumer(void* arg)
{
  BlockQueue* q = (BlockQueue*)arg;
  int data;
  while(1)
  {
    //消费者一直获取数据进行打印
    q->QueuePop(data);
    std::cout << "consumer gets a piece of data--" << data << std::endl;
  }
}
void* thr_productor(void* arg)
{
  BlockQueue* q = (BlockQueue*)arg;
  int data = 0;
  while(1)
  {
    //生产者一直添加数据
    q->QueuePush(data);
    std::cout << "producer produces a data--"  << (data++) << std::endl;
  }
  return NULL;
}
int main()
{
  pthread_t ctid[4], ptid[4];
  int ret, i;
  BlockQueue q;
  for(i = 0; i < 4; i++)
  {
    ret = pthread_create(&ctid[i], NULL, thr_consumer, (void*)&q);
    if(ret != 0)
    {
      std::cerr << "create thread error" << std::endl;
    }
  }
  for(i = 0; i < 4; i++)
  {
    ret = pthread_create(&ptid[i], NULL, thr_productor, (void*)&q);
    if(ret != 0)
    {
      std::cerr << "create thread error" << std::endl;
    }
  }
  for(i = 0; i < 4; i++)
  {
    pthread_join(ctid[i], NULL);
    pthread_join(ptid[i], NULL);
  }
}
